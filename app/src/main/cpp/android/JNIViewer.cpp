#include "include/JNIViewer.h"
#include "include/JNIEntityCollection.h"
#include "include/JNIEntity.h"
#include "include/JNIGeocentric3D.h"
#include "include/JNIScene.h"
#include "include/JNIDevicePoseCameraController.h"
#include "include/JNIScreenSpaceCameraController.h"

#include <android/native_window_jni.h>

namespace argeo
{
	JNIViewer::JNIViewer(Viewer* viewer)
		: NativeObjectWrapper(viewer)
	{ }

	JNIViewer::JNIViewer(
		JNIEnv *env,
		Viewer* viewer)
		: NativeObjectWrapper(env, viewer)
	{
		initialize(env);
	}

	void JNIViewer::initialize(JNIEnv *env)
	{
		setClass(env);
		cacheConstructor(env);

		addNativeMethod(
			"dispose",
			(void*)&JNIViewer::dispose,
			kTypeVoid,
			NULL);

		addNativeMethod(
			"initialize",
			(void*)&JNIViewer::java_initialize,
			kTypeVoid,
			NULL);

		addNativeMethod(
			"addStateListener",
			(void*)&JNIViewer::add_state_listener,
			kTypeBool,
			MAKE_CANONICAL_NAME(PACKAGE, ViewerStateListener),
			NULL);

		addNativeMethod(
			"removeStateListener",
			(void*)&JNIViewer::remove_state_listener,
			kTypeBool,
			MAKE_CANONICAL_NAME(PACKAGE, ViewerStateListener),
			NULL);

		addNativeMethod(
			"startRenderLoop",
			(void*)&JNIViewer::start_render_loop,
			kTypeVoid,
			NULL);

		addNativeMethod(
			"stopRenderLoop",
			(void*)&JNIViewer::stop_render_loop,
			kTypeVoid,
			NULL);

		addNativeMethod(
			"executeOnRenderLoop",
			(void*)&JNIViewer::execute_on_render_loop,
			kTypeVoid,
			MAKE_CANONICAL_NAME(PACKAGE, RenderLoopRunnable),
			NULL);

		addNativeMethod(
			"isRunning",
			(void*)&JNIViewer::is_running,
			kTypeBool,
			NULL);

		addNativeMethod(
			"nativeGetEntities",
			(void*)&JNIViewer::get_entities,
			MAKE_CANONICAL_NAME(PACKAGE, EntityCollection),
			NULL);

		addNativeMethod(
			"nativeGetScene",
			(void*)&JNIViewer::get_scene,
			MAKE_CANONICAL_NAME(PACKAGE, Scene),
			NULL);

		addNativeMethod(
			"getScreenSpaceCameraController",
			(void*)&JNIViewer::get_screen_space_camera_controller,
			MAKE_CANONICAL_NAME(PACKAGE, ScreenSpaceCameraController),
			NULL);

		addNativeMethod(
			"getDevicePoseCameraController",
			(void*)&JNIViewer::get_device_pose_camera_controller,
			MAKE_CANONICAL_NAME(PACKAGE, DevicePoseCameraController),
			NULL);

		addNativeMethod(
			"pickPosition",
			(void*)&JNIViewer::pick_position,
			MAKE_CANONICAL_NAME(PACKAGE, Geocentric3D),
			kTypeInt,
			kTypeInt,
			NULL);

		addNativeMethod(
			"pickEntity",
			(void*)&JNIViewer::pick_entity,
			MAKE_CANONICAL_NAME(PACKAGE, Entity),
			kTypeInt,
			kTypeInt,
			NULL);

		registerNativeMethods(env);
	}

	void JNIViewer::mapFields()
	{ }

	void JNIViewer::dispose(JNIEnv *env, jobject java_this)
	{
		JNIViewer *object = gClasses.getNativeInstance<JNIViewer>(env, java_this);
		if (object != NULL)
		{
			object->destroy(env, java_this);
		}
	}

	void JNIViewer::java_initialize(
		JNIEnv* jni, 
		jobject java_this)
	{
		JNIViewer* object = gClasses.newInstance<JNIViewer>(jni, java_this);
		object->setNativeObject(new Viewer());
		object->persist(jni, java_this);
	}

	jobject JNIViewer::create(
		JNIEnv* jni,
		Viewer* viewer)
	{
		JNIViewer* object = gClasses.newInstance<JNIViewer>(jni, nullptr);
		object->setNativeObject(viewer);
		jobject java_this = object->toJavaObject(jni);
		object->persist(jni, java_this);
		return java_this;
	}

	jboolean JNIViewer::is_running(
		JNIEnv* jni,
		jobject java_this)
	{
		JNIViewer* object = gClasses.getNativeInstance<JNIViewer>(jni, java_this);
		return object->getNativeObject()->is_running();
	}

	
	jobject JNIViewer::get_device_pose_camera_controller(
		JNIEnv* jni,
		jobject java_this)
	{
		JNIViewer* object = gClasses.getNativeInstance<JNIViewer>(jni, java_this);
		return JNIDevicePoseCameraController::create(jni, object->getNativeObject()->get_device_pose_camera_controller());
	}

	jobject JNIViewer::get_screen_space_camera_controller(
		JNIEnv* jni,
		jobject java_this)
	{
		JNIViewer* object = gClasses.getNativeInstance<JNIViewer>(jni, java_this);
		return JNIScreenSpaceCameraController::create(jni, object->getNativeObject()->get_screen_space_camera_controller());
	}

	jobject JNIViewer::get_entities(
		JNIEnv* jni,
		jobject java_this)
	{
		JNIViewer* object = gClasses.getNativeInstance<JNIViewer>(jni, java_this);
		return JNIEntityCollection::create(jni, object->getNativeObject()->get_entities());
	}

	jobject JNIViewer::get_scene(
		JNIEnv* jni,
		jobject java_this)
	{
		JNIViewer* object = gClasses.getNativeInstance<JNIViewer>(jni, java_this);
		return JNIScene::create(jni, object->getNativeObject()->get_scene());
	}

	jobject JNIViewer::pick_position(
		JNIEnv* jni,
		jobject java_this,
		jint window_x,
		jint window_y)
	{
		JNIViewer* object = gClasses.getNativeInstance<JNIViewer>(jni, java_this);
		vec3d position = object->getNativeObject()
			->pick_position(vec2d(window_x, window_y));
		return JNIGeocentric3D::create(jni, position);
	}

	jobject JNIViewer::pick_entity(
		JNIEnv* jni,
		jobject java_this,
		jint window_x,
		jint window_y)
	{
		JNIViewer* object = gClasses.getNativeInstance<JNIViewer>(jni, java_this);
		Entity* entity = object->getNativeObject()
			->pick_entity(vec2d(window_x, window_y));
		return JNIEntity::create(jni, entity);
	}

	void JNIViewer::stop_render_loop(
		JNIEnv* env,
		jobject java_this)
	{
		JNIViewer* object = gClasses.getNativeInstance<JNIViewer>(env, java_this);
		object->getNativeObject()->stop();
	}

	void JNIViewer::start_render_loop(
		JNIEnv* env,
		jobject java_this)
	{
		JNIViewer* object = gClasses.getNativeInstance<JNIViewer>(env, java_this);

		Viewer*  viewer  = object->getNativeObject();
		Surface* surface = viewer->get_surface();
		Context* context = surface->get_context();

		object->m_direct_video = std::unique_ptr<VideoCapture>(gClasses.newInstance<VideoCapture>(env, nullptr));
		object->m_direct_video->set_viewport_size(Rectangle(
			0,
			0,
			context->get_drawing_surface_width(),
			context->get_drawing_surface_height()));

		viewer->get_scene()->set_camera_preview(object->m_direct_video.get());
		viewer->run_async();
	}

	jboolean JNIViewer::add_state_listener(
		JNIEnv* jni,
		jobject java_this,
		jobject java_listener)
	{
		JNIViewer* object = gClasses.getNativeInstance<JNIViewer>(jni, java_this);

		if (object->m_listeners.find(java_listener) == object->m_listeners.end())
		{
			java_listener = jni->NewGlobalRef(java_listener);
			JNIViewerStateListener* native_listener = new JNIViewerStateListener(java_listener);
			object->getNativeObject()->add_state_listener(native_listener);
			object->m_listeners[java_listener] = native_listener;
			return true;
		}
		
		return false;
	}
	
	jboolean JNIViewer::remove_state_listener(
		JNIEnv* jni,
		jobject java_this,
		jobject java_listener)
	{
		JNIViewer* object = gClasses.getNativeInstance<JNIViewer>(jni, java_this);
		
		if (object->m_listeners.find(java_listener) != object->m_listeners.end())
		{
			JNIViewerStateListener* native_listener = object->m_listeners[java_listener];
			object->getNativeObject()->remove_state_listener(native_listener);
			object->m_listeners.erase(java_listener);
			jni->DeleteGlobalRef(java_listener);
			delete native_listener;			
			return true;
		}

		return false;
	}

	void JNIViewer::execute_on_render_loop(
		JNIEnv* jni,
		jobject java_this,
		jobject java_runnable)
	{
		JNIViewer* object = gClasses.getNativeInstance<JNIViewer>(jni, java_this);

		java_runnable = jni->NewGlobalRef(java_runnable);

		object->getNativeObject()
			->execute_on_render_loop([&, java_runnable](Context* context)
		{
			JNIEnv* jni;
			jint status = JNIHelper::get_instance().attach_current_thread(&jni);

			jclass clazz = jni->GetObjectClass(java_runnable);
			jni->CallVoidMethod(
				java_runnable,
				jni->GetMethodID(
					clazz,
					"run",
					"()V"));

			jni->DeleteGlobalRef(java_runnable);
			JNIHelper::get_instance().detach_current_thread(status);
		});
	}
}