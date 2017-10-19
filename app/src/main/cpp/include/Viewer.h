#pragma once

#include "Scene.h"
#include "SurfaceWindow.h"
#include "Entity.h"
#include "EntityCollection.h"

#include "IVisualizer.h"
#include "ViewerStateListener.h"

#include <thread>
#include <unordered_set>
#include <memory>
#include <vector>
#include <algorithm>
#include <mutex>

namespace argeo
{
	class DevicePoseCameraController;
	class ScreenSpaceCameraController;

	typedef std::function<void(Context* context)> RunOnRenderLoopHandler;

	class Viewer 
		: public SurfaceStateListener		
	{
	public:
		Viewer();
		~Viewer();
		
		std::thread run_async();
		void run();
		void stop();
		void tick();
		void loop();
		
		bool add_state_listener(ViewerStateListener* listener);
		bool remove_state_listener(ViewerStateListener* listener);

		EntityCollection* get_entities();

		void on_surface_created(void* reserved = nullptr);
		void on_render_frame(RenderFrameEventArgs args);

		void execute_on_render_loop(const RunOnRenderLoopHandler& handler);

		SurfaceWindow* get_surface();
		
		DevicePoseCameraController* get_device_pose_camera_controller();
		ScreenSpaceCameraController* get_screen_space_camera_controller();

		Scene*  get_scene();
		bool    is_running();
		vec3d   pick_position(const vec2d& window_position);
		Entity* pick_entity(const vec2d& window_position);

	private:
		bool m_running;
		int64_t m_last_time;
		double  m_update_rate;

		std::vector<IVisualizer*> m_visualizers;

		std::mutex m_run_on_loop_mutex;
		std::vector<RunOnRenderLoopHandler> m_run_on_render_loop_handlers;

		std::unique_ptr<DevicePoseCameraController> m_device_pose_camera_controller;
		std::unique_ptr<ScreenSpaceCameraController> m_screen_space_camera_controller;

		std::unique_ptr<SurfaceWindow> m_surface;
		std::unique_ptr<EntityCollection> m_entities;
		std::unique_ptr<Scene> m_scene;

		std::unordered_set<ViewerStateListener*> m_state_listeners;
	};

}