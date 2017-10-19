#pragma once

#include "GestureListener.h"
#include "KeyboardListener.h"
#include "TouchEvent.h"


#include <unordered_set>

namespace argeo
{
	class InputManager
	{
	public:
		static bool process_touch_event(const TouchEvent& event);

		static bool add_gesture_listener(GestureListener* listener);
		static bool remove_gesture_listener(GestureListener* listener);

		static bool process_key_event(const KeyboardKeyEventArgs& event);

		static bool add_keyboard_listener(KeyboardListener* listener);
		static bool remove_keyboard_listener(KeyboardListener* listener);

		static bool on_drag_event(
			const TouchEvent& event,
			const DragState& state);

		static bool on_scale_event(
			const TouchEvent& event,
			const float& factor);

		static bool on_rotate_event(
			const TouchEvent& event,
			const float& radians);

		static bool on_tap_event(const TouchEvent& event);
		static bool on_double_tap_event(const TouchEvent& event);

	private:
		static std::unordered_set<KeyboardListener*> g_keyboard_listeners;
		static std::unordered_set<GestureListener*> g_gesture_listeners;
	};
}
