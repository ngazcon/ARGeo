#include "include/ArgeoNativeActivity.h"
#include "InputManager.h"
#include "include/Application.h"
#include "Device.h"
#include "include/JNIHelper.h"
#include "include/Engine.h"
#include "include/ArgeoTest.h"
#include "ModelGraphics.h"

#include "EllipsoidTransformations.h"
#include "BillboardGraphics.h"

namespace argeo
{
	namespace jni
	{
		ArgeoNativeActivity::ArgeoNativeActivity()
			: m_event_loop(this)
			, m_resources_created(false)
			, m_viewer(new Viewer())
		{ }


		ArgeoNativeActivity::~ArgeoNativeActivity()
		{ }

		void ArgeoNativeActivity::run()
		{
			m_event_loop.run();
		}

		status ArgeoNativeActivity::on_activate()
		{
			return STATUS_OK;
		}

		void ArgeoNativeActivity::on_deactivate()
		{ }

		status ArgeoNativeActivity::on_step()
		{
			return STATUS_OK;
		}

		Viewer* ArgeoNativeActivity::get_viewer()
		{
			return m_viewer.get();
		}

		void ArgeoNativeActivity::on_create_window()
		{
			m_viewer->on_surface_created(Application::get_android_app()->window);
			m_viewer->get_scene()->add_pre_render_listener(this);
			m_viewer->run_async();
		}

		void ArgeoNativeActivity::pre_render(
			Scene* scene,
			EngineClockTick time)
		{
			if (!m_resources_created)
			{
				load_resources();
			}
		}

		void ArgeoNativeActivity::load_resources()
		{
			JNIEnv* jenv;
			JavaVM* jvm = JNIHelper::get_instance().get_java_vm();
			jvm->AttachCurrentThread(&jenv, nullptr);

			jclass native_activity_class = jenv->GetObjectClass(
				Application::get_android_app()->activity->clazz);

			jmethodID method = jenv->GetMethodID(
				native_activity_class,
				"onGLInitialized",
				"()V");

			jenv->CallVoidMethod(
				Application::get_android_app()->activity->clazz,
				method);

			Context* context = m_viewer->get_surface()->get_context();

			m_direct_video = std::unique_ptr<VideoCapture>(gClasses.newInstance<VideoCapture>(jenv, nullptr));
			m_direct_video->set_viewport_size(Rectangle(
				0,
				0,
				context->get_drawing_surface_height(),
				context->get_drawing_surface_width()));

			jvm->DetachCurrentThread();

			Entity* entity = new Entity(
				"pepe",
				new BillboardGraphics(
					"icons/032.png"));

			Entity* ernesto = new Entity(
				"ernesto",
				new BillboardGraphics(
					"icons/032.png"));

			Entity* ernesto2 = new Entity(
				"ernesto2",
				new BillboardGraphics(
					"icons/ernesto.jpg"));

			entity->set_position(
				EllipsoidTransformations::geocentric3D_from_geodetic2D(Geodetic2D(
					ArgeoMath::to_radians(-62.271632),
					ArgeoMath::to_radians(-38.715719))));

			ernesto->set_position(
				EllipsoidTransformations::geocentric3D_from_geodetic2D(Geodetic2D(
					ArgeoMath::to_radians(-62.233226),
					ArgeoMath::to_radians(-38.668368))));

			ernesto2->set_position(
				EllipsoidTransformations::geocentric3D_from_geodetic2D(Geodetic2D(
					ArgeoMath::to_radians(-62.241529),
					ArgeoMath::to_radians(-38.699719))));

			m_viewer->get_entities()->add(entity);
			m_viewer->get_entities()->add(ernesto);
			m_viewer->get_entities()->add(ernesto2);

			m_viewer->get_scene()->set_camera_preview(m_direct_video.get());
			m_resources_created = true;
		}

		void ArgeoNativeActivity::unload_resources()
		{
			JNIEnv* jenv;
			JavaVM* jvm = JNIHelper::get_instance().get_java_vm();
			jvm->AttachCurrentThread(&jenv, nullptr);

			jclass native_activity_class = jenv->GetObjectClass(
				Application::get_android_app()->activity->clazz);

			jmethodID method = jenv->GetMethodID(
				native_activity_class,
				"onGLInitialized",
				"()V");

			jenv->CallVoidMethod(
				Application::get_android_app()->activity->clazz,
				method);

			jvm->DetachCurrentThread();

			m_resources_created = false;
		}
	}
}


using namespace argeo;
using namespace argeo::jni;

static ArgeoNativeActivity* m_activity;

/**
* This is the main entry point of a native application that is using
* android_native_app_glue.  It runs in its own thread, with its own
* event loop for receiving input events and doing other things.
*/
void android_main(struct android_app* state)
{
	app_dummy();

	Application::set_android_app(state);

	JNIEnv* jni;
	JavaVM* jvm = state->activity->vm;
	jvm->AttachCurrentThread(&jni, nullptr);

	Engine::init(
		jni,
		state->activity->clazz);

	jvm->DetachCurrentThread();

	m_activity = new ArgeoTest();
	m_activity->run();
}