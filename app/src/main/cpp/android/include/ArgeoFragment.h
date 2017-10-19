#pragma once

#include "TouchEvent.h"
#include "Viewer.h"
#include "InputManager.h"

#include <android/native_window.h> 
#include <memory>

namespace argeo
{	
	class ArgeoFragment
	{
	public:
		ArgeoFragment();
	
		bool on_drag(
			const TouchEvent& event,
			const DragState& drag_state);

		bool on_rotate(
			const TouchEvent& event,
			const float& radians);

		bool on_scale(
			const TouchEvent& event,
			const float& factor);

		void on_surface_created(ANativeWindow* window);
		void on_surface_destroyed();

		Viewer* get_viewer();

	private:
		ANativeWindow* m_window;
		std::unique_ptr<Viewer> m_viewer;
	};
}
