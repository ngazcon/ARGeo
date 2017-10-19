#pragma once

#include "Scene.h"

namespace argeo
{
	class CameraChangedListener
	{
	public:

		virtual void on_changed(
			const double& percentage,
			Camera* camera) = 0;
	};
}
