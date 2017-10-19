#include "InputManager.h"
#include "include/GestureDetector.h"

namespace argeo
{
	std::unordered_set<KeyboardListener*> InputManager::g_keyboard_listeners;
	std::unordered_set<GestureListener*>  InputManager::g_gesture_listeners;

	bool InputManager::process_touch_event(const TouchEvent& event)
	{
		static DragDetector drag_detector;
		static TapDetector tap_detector;
		static DoubletapDetector double_tap_detector;

		GESTURE_STATE double_tap_state = double_tap_detector.Detect(
			static_cast<const AInputEvent*>(event.inner_event));
		
		GESTURE_STATE tap_state = tap_detector.Detect(
			static_cast<const AInputEvent*>(event.inner_event));

		GESTURE_STATE drag_state = drag_detector.Detect(
			static_cast<const AInputEvent*>(event.inner_event));

		//Double tap detector has a priority over other detectors
		if (double_tap_state == GESTURE_STATE_ACTION)
		{
			return on_double_tap_event(event);
		}
		else 
		{
			if (tap_state == GESTURE_STATE_ACTION)
			{
				return on_tap_event(event);
			}

			//Handle drag state
			if (drag_state & GESTURE_STATE_START)
			{
				return on_drag_event(event, DragState::Start);
			}
			else if (drag_state & GESTURE_STATE_MOVE)
			{
				return on_drag_event(event, DragState::Move);
			}
			else if (drag_state & GESTURE_STATE_END)
			{
				return on_drag_event(event, DragState::End);
			}
		}	

		return false;
	}

	bool InputManager::on_scale_event(
		const TouchEvent& event,
		const float& factor)
	{
		for (auto listener : g_gesture_listeners)
		{
			listener->on_scale_event(
				event,
				factor);
		}
		return true;

	}

	bool InputManager::on_rotate_event(
		const TouchEvent& event,
		const float& radians)
	{
		for (auto listener : g_gesture_listeners)
		{
			listener->on_rotate_event(
				event,
				radians);
		}
		return true;
	}

	bool InputManager::on_drag_event(
		const TouchEvent& event,
		const DragState& state)
	{
		for (auto listener : g_gesture_listeners)
		{
			listener->on_drag_event(
				event,
				state);
		}
		return true;
	}

	bool InputManager::on_tap_event(const TouchEvent& event)
	{
		for (auto listener : g_gesture_listeners)
		{
			listener->on_tap_event(event);
		}
		return true;
	}

	bool InputManager::on_double_tap_event(const TouchEvent& event)
	{
		for (auto listener : g_gesture_listeners)
		{
			listener->on_double_tap_event(event);
		}
		return true;
	}

	bool InputManager::add_gesture_listener(GestureListener* listener)
	{
		return g_gesture_listeners.insert(listener).second;
	}

	bool InputManager::remove_gesture_listener(GestureListener* listener)
	{
		return g_gesture_listeners.erase(listener);
	}

	bool InputManager::process_key_event(const KeyboardKeyEventArgs& event)
	{
		for (auto listener : g_keyboard_listeners)
		{
			listener->on_key_event(event);
		}
		return true;
	}

	bool InputManager::add_keyboard_listener(KeyboardListener* listener)
	{
		return g_keyboard_listeners.insert(listener).second;
	}

	bool InputManager::remove_keyboard_listener(KeyboardListener* listener)
	{
		return g_keyboard_listeners.erase(listener);
	}
}