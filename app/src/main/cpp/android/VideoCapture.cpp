#include "include/VideoCapture.h"
#include "RectangleTessellator.h"
#include "include/VideoCapturePreviewShaders.h"
#include "UniformSampler.h"
#include "include/JNIHelper.h"

#include <algorithm>

namespace argeo
{
	VideoCapture::VideoCapture()
		: m_frame_available_count(0)
		, m_resources_created(false)
		, m_running(false)
		, m_current_vertical_view_angle(0.0)
		, m_current_aspect_ratio(4.0 / 3.0)
		, m_current_preview_size(0, 0)
		, m_viewport_size(0, 0)
		, m_orientation(0)
		, m_draw_command(PrimitiveType::Triangles)
		, NativeObject()
	{ }

	VideoCapture::VideoCapture(JNIEnv* jni)
		: NativeObject(jni)
		, m_current_vertical_view_angle(0.0)
		, m_current_aspect_ratio(4.0 / 3.0)
		, m_current_preview_size(0, 0)
		, m_viewport_size(0, 0)
		, m_orientation(0)
		, m_draw_command(PrimitiveType::Triangles)
	{ 
		initialize(jni);
	}

	VideoCapture::~VideoCapture()
	{ }

	void VideoCapture::initialize(JNIEnv* jni)
	{
		setClass(jni);
		cacheConstructor(jni);
		
		cacheMethod(
			jni,
			"start",
			kTypeVoid,
			NULL);

		cacheMethod(
			jni,
			"stop",
			kTypeVoid,
			NULL);

		cacheMethod(
			jni,
			"setSurfaceTexture",
			kTypeVoid,
			"Landroid/graphics/SurfaceTexture;",
			NULL);

		cacheMethod(
			jni,
			"stop",
			kTypeVoid,
			NULL);

		addNativeMethod(
			"frameAvailable", 
			(void*)&VideoCapture::jni_frame_available,
			kTypeVoid,
			NULL);
		
		addNativeMethod(
			"setCurrentAspectRatio",
			(void*)&VideoCapture::jni_set_current_aspect_ratio,
			kTypeVoid,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"getCurrentAspectRatio",
			(void*)&VideoCapture::jni_get_current_aspect_ratio,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"setCurrentVerticalViewAngle",
			(void*)&VideoCapture::jni_set_current_vertical_view_angle,
			kTypeVoid,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"getCurrentVerticalViewAngle",
			(void*)&VideoCapture::jni_get_current_vertical_view_angle,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"isRunning",
			(void*)&VideoCapture::jni_is_running,
			kTypeBool,
			NULL);

		addNativeMethod(
			"setCurrentPreviewSize",
			(void*)&VideoCapture::jni_set_current_preview_size,
			kTypeVoid,
			kTypeDouble,
			kTypeDouble,
			NULL);

		registerNativeMethods(jni);
	}

	void VideoCapture::mapFields()
	{ }

	void VideoCapture::nativeDestroy(JNIEnv *env, jobject java_this)
	{
		VideoCapture *object = gClasses.getNativeInstance<VideoCapture>(env, java_this);
		if (object != NULL) 
		{
			object->destroy(env, java_this);
		}
	}

	Rectangle VideoCapture::get_viewport_size() const
	{
		return m_viewport_size;
	}

	Rectangle VideoCapture::get_current_preview_size() const
	{
		return m_current_preview_size;
	}

	void VideoCapture::create_resources(
		JNIEnv* jni,
		Context* context)
	{
		m_render_resources.shader_program = context->get_shader_cache()->get_shader_program(
			video_capture_preview_vs,
			video_capture_preview_fs);
		m_uniform_map["u_ratios"] = [&](IUniform* uniform)
		{
			vec2 ratios(1.0f);

			//double viewport_aspect_ratio = (double) m_viewport_size.get_width()        / (double) m_viewport_size.get_height();
			//double preview_aspect_ratio  = (double) m_current_preview_size.get_width() / (double) m_current_preview_size.get_height();
			//
			//double delta_height = (double) m_viewport_size.get_height() - (double) m_current_preview_size.get_height();
			//double delta_width  = (double) m_viewport_size.get_width()  - (double) m_current_preview_size.get_width();
			//
			//if (delta_height >= delta_width)
			//{
			//	ratios.x = 1.0f;
			//	ratios.y = viewport_aspect_ratio / preview_aspect_ratio;
			//}
			//else
			//{
			//	ratios.y = 1.0f;
			//	ratios.x = 1.0f / (viewport_aspect_ratio / preview_aspect_ratio);
			//}

			//if (m_orientation == 90.0f)
			//{
			//	ratios.y = m_viewport_size.get_width()  * 1.0f / m_current_preview_size.get_height();
			//	ratios.x = m_viewport_size.get_height() * 1.0f / m_current_preview_size.get_width();
			//}
			//else
			//{
			//	ratios.y = m_viewport_size.get_height()  * 1.0f / m_current_preview_size.get_height();
			//	ratios.x = m_viewport_size.get_width()   * 1.0f / m_current_preview_size.get_width();
			//}
			//
			//double ratio = std::max(ratios.x, ratios.y);

			static_cast<Uniform<vec2>*>(uniform)->set_value(ratios);
		};

		m_uniform_map["u_orientation"] = [&](IUniform* uniform)
		{
			static_cast<Uniform<mat3>*>(uniform)->set_value(to_mat3(mat_rotate_y(m_orientation)));
		};

		m_uniform_map["u_transformation"] = [&](IUniform* uniform)
		{
			static_cast<Uniform<mat4>*>(uniform)->set_value(m_transform);
		};

		m_uniform_map["sampler"] = [&](IUniform* uniform)
		{
			static_cast<UniformSampler*>(uniform)->set_texture(m_render_resources.texture.get());
		};

		//Create full scene quad buffer
		std::unique_ptr<Mesh> mesh = RectangleTessellator::compute(
			RectangleD(vec2d(-1.0, -1.0), vec2d(1.0, 1.0)),
			1,
			1);

		m_render_resources.vertex_array = context->create_vertex_array(
			mesh.get(),
			m_render_resources.shader_program->get_attributes(),
			BufferHint::StaticDraw);

		m_render_resources.render_state = RenderState::from_cache();

		m_draw_command.set_primitive_type(mesh->get_primitive_type());
		m_draw_command.set_vertex_array(m_render_resources.vertex_array.get());
		m_draw_command.set_shader_program(m_render_resources.shader_program);
		m_draw_command.set_render_state(m_render_resources.render_state);
		m_draw_command.set_uniform_map(&m_uniform_map);
		m_draw_command.set_pass(CommandPass::CameraPreview);

		create_external_texture();
		create_surface_texture(jni);
		create_camera(jni);

		m_resources_created = true;
	}

	void VideoCapture::create_external_texture()
	{
		m_render_resources.texture = std::unique_ptr<TextureExternal>(new TextureExternal());
	}

	void VideoCapture::create_surface_texture(JNIEnv* jenv)
	{
		m_surface_texture_class  = jenv->FindClass("android/graphics/SurfaceTexture");

		jmethodID constructor_id = jenv->GetMethodID(
			m_surface_texture_class,
			"<init>",
			"(I)V");

		m_surface_texture = jenv->NewObject(
			m_surface_texture_class,
			constructor_id,
			static_cast<jint>(m_render_resources.texture->get_handle()));
		m_surface_texture = jenv->NewGlobalRef(m_surface_texture);

		m_surface_texture_detach_context_mid = jenv->GetMethodID(
			m_surface_texture_class,
			"detachFromGLContext",
			"()V");

		m_surface_texture_attach_context_mid = jenv->GetMethodID(
			m_surface_texture_class,
			"attachToGLContext",
			"(I)V");

		m_surface_texture_update_texture_mid = jenv->GetMethodID(
			m_surface_texture_class,
			"updateTexImage",
			"()V");

		m_surface_texture_get_transform_mid = jenv->GetMethodID(
			m_surface_texture_class,
			"getTransformMatrix",
			"([F)V");
	}

	void VideoCapture::create_camera(JNIEnv* jenv)
	{
		m_java_this = jenv->NewGlobalRef(toJavaObject(jenv));
		
		jenv->CallVoidMethod(
			m_java_this,
			getMethod("setSurfaceTexture"),
			m_surface_texture);
	}
	
	void VideoCapture::start(JNIEnv* jenv)
	{
		jenv->CallVoidMethod(
			m_java_this,
			getMethod("start"));
		m_running = true;
	}

	void VideoCapture::stop(JNIEnv* jenv)
	{
		jenv->CallVoidMethod(
			m_java_this,
			getMethod("stop"));
		m_running = false;
	}

	Texture2D* VideoCapture::get_texture()
	{
		return m_render_resources.texture.get();
	}

	void VideoCapture::update(FrameState* frame_state)
	{
		Context* context = frame_state->get_context();

		JNIEnv* jni;
		jint status = JNIHelper::get_instance().attach_current_thread(&jni);

		if (!m_resources_created)
		{
			create_resources(
				jni,
				context);

			if (!m_running)
			{
				start(jni);
			}
		}


		update_texture(jni);

		JNIHelper::get_instance().detach_current_thread(status);

		frame_state->get_command_queue().push_back(&m_draw_command);
	}

	void VideoCapture::update_texture(JNIEnv* jenv)
	{
		std::unique_lock<std::mutex> lock(m_update_mutex);
		if (m_frame_available_count > 0)
		{
			jenv->CallVoidMethod(
				m_surface_texture,
				m_surface_texture_update_texture_mid);

			jfloatArray transformation = jenv->NewFloatArray(16);
			jenv->CallVoidMethod(
				m_surface_texture,
				m_surface_texture_get_transform_mid,
				transformation);

			float* native_values = static_cast<float*>(jenv->GetFloatArrayElements(
				transformation,
				0));

			std::copy_n(
				native_values,
				16,
				m_transform.data);

			jenv->ReleaseFloatArrayElements(
				transformation,
				native_values,
				0);

			jenv->DeleteLocalRef(transformation);

			m_frame_available_count--;
		}
	}

	void VideoCapture::frame_available()
	{
		std::unique_lock<std::mutex> lock(m_update_mutex);
		m_frame_available_count++;
	}

	mat4 VideoCapture::get_transform() const
	{
		return m_transform;
	}

	unsigned int VideoCapture::get_texture_id() const
	{
		return m_render_resources.texture->get_handle();
	}

	bool VideoCapture::is_running() const
	{
		return m_running;
	}

	void VideoCapture::set_orientation(float value)
	{
		m_orientation = value;
	}

	void VideoCapture::set_viewport_size(Rectangle value)
	{
		m_viewport_size = value;
	}

	void VideoCapture::set_current_preview_size(Rectangle value)
	{
		m_current_preview_size = value;
	}

	void VideoCapture::set_current_aspect_ratio(const double& value)
	{
		m_current_aspect_ratio = value;
	}

	double VideoCapture::get_current_aspect_ratio() const
	{
		return m_current_aspect_ratio;
	}

	void VideoCapture::set_current_vertical_view_angle(const double& value)
	{
		m_current_vertical_view_angle = value;
	}

	double VideoCapture::get_current_vertical_view_angle() const
	{
		return m_current_vertical_view_angle;
	}


	void VideoCapture::jni_frame_available(
		JNIEnv* env,
		jobject java_this)
	{
		VideoCapture* object = gClasses.getNativeInstance<VideoCapture>(env, java_this);
		object->frame_available();
	}

	void VideoCapture::jni_set_current_preview_size(
		JNIEnv* env,
		jobject java_this,
		jdouble width,
		jdouble height)
	{
		VideoCapture* object = gClasses.getNativeInstance<VideoCapture>(env, java_this);
		object->set_current_preview_size(Rectangle(0, 0, width, height));
	}

	void VideoCapture::jni_set_current_aspect_ratio(
		JNIEnv* env,
		jobject java_this,
		jdouble value)
	{
		VideoCapture* object = gClasses.getNativeInstance<VideoCapture>(env, java_this);
		object->set_current_aspect_ratio(value);
	}

	jdouble VideoCapture::jni_get_current_aspect_ratio(
		JNIEnv* env,
		jobject java_this)
	{
		VideoCapture* object = gClasses.getNativeInstance<VideoCapture>(env, java_this);
		return object->get_current_aspect_ratio();
	}

	void VideoCapture::jni_set_current_vertical_view_angle(
		JNIEnv* env,
		jobject java_this,
		jdouble value)
	{
		VideoCapture* object = gClasses.getNativeInstance<VideoCapture>(env, java_this);
		object->set_current_vertical_view_angle(value);
	}

	jdouble VideoCapture::jni_get_current_vertical_view_angle(
		JNIEnv* env,
		jobject java_this)
	{
		VideoCapture* object = gClasses.getNativeInstance<VideoCapture>(env, java_this);
		return object->get_current_vertical_view_angle();
	}

	jboolean VideoCapture::jni_is_running(
		JNIEnv* env,
		jobject java_this)
	{
		VideoCapture* object = gClasses.getNativeInstance<VideoCapture>(env, java_this);
		return object->is_running();
	}
}