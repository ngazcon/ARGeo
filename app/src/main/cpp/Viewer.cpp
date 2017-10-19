#include "Viewer.h"
#include "Device.h"

#include "BillboardVisualizer.h"
#include "ModelVisualizer.h"
#include "PolylineVisualizer.h"
#include "PlaneVisualizer.h"
#include "android/include/JNIHelper.h"

#include "ScreenSpaceCameraController.h"
#include "android/include/DevicePoseCameraController.h"

namespace argeo
{
	Viewer::Viewer()
		: m_running(false)
		, m_last_time(0)
		, m_update_rate(30)
		, m_screen_space_camera_controller(nullptr)
		, m_device_pose_camera_controller(nullptr)
		, m_entities(new EntityCollection())
	{  }

	Viewer::~Viewer()
	{ }

	void Viewer::on_surface_created(void* reserved)
	{
		m_surface = Device::create_surface_window(reserved);
		m_scene = std::unique_ptr<Scene>(
			new Scene(m_surface->get_context()));

		m_surface->add_surface_state_listener(this);

		m_visualizers = std::vector<IVisualizer*>
		{
			new ModelVisualizer(m_entities.get(), m_scene.get()),
			new BillboardVisualizer(m_entities.get(), m_scene.get()),
			new PolylineVisualizer(m_entities.get(), m_scene.get()),
			new PlaneVisualizer(m_entities.get(), m_scene.get())
		};

#ifdef __ANDROID__
		m_screen_space_camera_controller = std::unique_ptr<ScreenSpaceCameraController>(new ScreenSpaceCameraController(
			get_scene()->get_terrain()));

		m_device_pose_camera_controller = std::unique_ptr<DevicePoseCameraController>(new DevicePoseCameraController(
			get_scene()->get_terrain()));
#elif WIN32
		m_default_camera_controller = std::unique_ptr<GesturesCameraController>(new GesturesCameraController(
			get_scene()->get_terrain(),
			get_surface()));
#endif
		m_surface->get_context()->detach();
	}

	Scene* Viewer::get_scene()
	{
		return m_scene.get();
	}

	SurfaceWindow* Viewer::get_surface()
	{
		return m_surface.get();
	}

	bool Viewer::add_state_listener(ViewerStateListener* listener)
	{
		return m_state_listeners.insert(listener).second;
	}

	bool Viewer::remove_state_listener(ViewerStateListener* listener)
	{
		return m_state_listeners.erase(listener);
	}

	std::thread Viewer::run_async()
	{
		m_running = true;
		std::thread render_thread(&Viewer::loop, this);
		render_thread.detach();
		return render_thread;
	}

	void Viewer::run()
	{
		m_running = true;
		loop();
	}

	void Viewer::execute_on_render_loop(const RunOnRenderLoopHandler& handler)
	{
		std::unique_lock<std::mutex> lock(m_run_on_loop_mutex);
		m_run_on_render_loop_handlers.push_back(handler);
	}

	void Viewer::stop()
	{
		m_running = false;
	}

	void Viewer::loop()
	{
		try
		{
			m_surface->get_context()->attach();

				
			for(ViewerStateListener* listener : m_state_listeners)
			{
				listener->on_start_render_loop();
			}

			while (m_running)
			{
				tick();			
			}			

			for (ViewerStateListener* listener : m_state_listeners)
			{
				listener->on_stop_render_loop();
			}

			m_surface->get_context()->detach();
		}
		catch (...)                                                     
		{   
			JNIEnv* jni;
			JavaVM* jvm = JNIHelper::get_instance().get_java_vm();
			jvm->AttachCurrentThread(&jni, 0);
			JNIHelper::get_instance().rethrow_cpp_exception_as_java_exception(jni);
			jvm->DetachCurrentThread();
		}
	}

	void Viewer::tick()
	{
		int64_t current_time = EngineClock::now().time_since_epoch().count();

		if ((current_time - m_last_time) >= (1.0 / m_update_rate))
		{
			m_last_time = current_time;
			m_surface->update();
		}
	}

	void Viewer::on_render_frame(RenderFrameEventArgs args)
	{
		EngineClockTick tick = EngineClock::now();

		Camera* camera = m_scene->get_camera();

		m_device_pose_camera_controller->update(camera);
		m_screen_space_camera_controller->update(camera);

		for (IVisualizer* visualizer : m_visualizers)
		{
			visualizer->update(tick);
		}

		m_scene->render(tick);

        std::vector<RunOnRenderLoopHandler> handlers;
        {
            std::unique_lock<std::mutex> lock(m_run_on_loop_mutex);
            handlers = m_run_on_render_loop_handlers;
            m_run_on_render_loop_handlers.clear();
        }

        for (RunOnRenderLoopHandler handler : handlers)
        {
            handler(m_scene->get_context());
        }

	}

	Entity* Viewer::pick_entity(const vec2d& window_position)
	{
		IPickable* pickable = m_scene->pick_primitive(window_position);
		Entity* entity      = nullptr;
		if (pickable != nullptr)
		{
			void* owner = pickable->get_owner();
			if (std::strcmp(typeid(*owner).name(),  typeid(Entity).name()) == 0)
			{
				entity = static_cast<Entity*>(owner);
			}
		}

		return entity;
	}

	vec3d Viewer::pick_position(const vec2d& window_position)
	{
		return m_scene->pick_position(window_position);
	}

	bool Viewer::is_running()
	{
		return m_running;
	}

	EntityCollection* Viewer::get_entities()
	{
		return m_entities.get();
	}

	DevicePoseCameraController* Viewer::get_device_pose_camera_controller()
	{
		return m_device_pose_camera_controller.get();
	}

	ScreenSpaceCameraController* Viewer::get_screen_space_camera_controller()
	{
		return m_screen_space_camera_controller.get();
	}
}
