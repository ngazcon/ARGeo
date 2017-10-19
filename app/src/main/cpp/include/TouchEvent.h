#pragma once

#include "Point.h"
#include "Action.h"
#include "ActionButton.h"

#include <cstdint>

namespace argeo
{
	struct TouchEvent
	{
		TouchEvent(
			const Point& point,
			const Action& action,
			const int64_t& event_time,
			const ActionButton& button = ActionButton::Left,
			const void* inner_event    = nullptr)
			: inner_event(inner_event)
			, point(point)
			, action(action)
			, button(button)
			, event_time(event_time)
		{ }

		const Point point;
		const Action action;
		const ActionButton button;
		const int64_t event_time;
		const void* inner_event;		
	};
}
