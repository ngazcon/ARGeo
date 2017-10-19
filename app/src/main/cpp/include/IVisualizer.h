#pragma once

#include "Entity.h"
#include "EngineClock.h"

namespace argeo
{
	class IVisualizer
	{
	public:
		virtual void update(const EngineClockTick& tick) = 0;
	};
}
