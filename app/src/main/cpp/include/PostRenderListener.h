#pragma once

#include "Scene.h"

namespace argeo
{
	class PostRenderListener
	{
	public:

		virtual void post_render(Scene* scene, EngineClockTick time) = 0;
	};
}
