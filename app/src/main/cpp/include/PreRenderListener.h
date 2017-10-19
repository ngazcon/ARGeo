#pragma once

#include "Scene.h"

namespace argeo
{
	class PreRenderListener
	{
	public:

		virtual void pre_render(Scene* scene, EngineClockTick time) = 0;
	};
}
