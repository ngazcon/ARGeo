#pragma once

#include "Scene.h"

namespace argeo
{
	class CameraMoveListener
	{
	public:

		virtual void on_move_start(Camera* camera) = 0;
		virtual void on_move_end(Camera* camera) = 0;
	};
}
