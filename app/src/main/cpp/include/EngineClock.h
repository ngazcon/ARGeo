#pragma once

#include <chrono>

namespace argeo
{
	typedef std::chrono::system_clock EngineClock;
	typedef EngineClock::time_point   EngineClockTick;
}

