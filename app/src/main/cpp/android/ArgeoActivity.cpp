#include "include/ArgeoActivity.h"

namespace argeo
{
	ArgeoActivity::ArgeoActivity()
		: m_viewer(new Viewer())
	{ }

	bool ArgeoActivity::on_drag(
		const TouchEvent& event,
		const DragState& drag_state)
	{
		return InputManager::on_drag_event(event, drag_state);
	}

	bool ArgeoActivity::on_rotate(
		const TouchEvent& event,
		const float& radians)
	{
		return InputManager::on_rotate_event(event, radians);
	}

	bool ArgeoActivity::on_scale(
		const TouchEvent& event,
		const float& factor)
	{
		return InputManager::on_scale_event(event, factor);
	}

	void ArgeoActivity::on_surface_created(ANativeWindow* window)
	{
		m_window = window;
		m_viewer->on_surface_created(window);
	}

	void ArgeoActivity::on_surface_destroyed()
	{
		ANativeWindow_release(m_window);
	}

	Viewer* ArgeoActivity::get_viewer()
	{
		return m_viewer.get();
	}
}