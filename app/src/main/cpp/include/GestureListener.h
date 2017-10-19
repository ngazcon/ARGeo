#pragma once

#include "TouchEvent.h"
#include "GestureListener.h"

namespace argeo
{
	enum class DragState : unsigned int
	{
		Start = 0,
		Move  = 1,
		End   = 2,
	};

	class GestureListener
	{
	public:

		virtual bool on_drag_event(
			const TouchEvent& event,
			const DragState& state)
		{ 
			return true;
		};

		virtual bool on_scale_event(
			const TouchEvent& event,
			const float& factor)
		{
			return true;
		}

		virtual bool on_rotate_event(
			const TouchEvent& event,
			const float& radians)
		{
			return true;
		}

		virtual bool on_tap_event(const TouchEvent& event)
		{
			return true;
		};

		virtual bool on_double_tap_event(const TouchEvent& event)
		{
			return true;
		};
	};
}
