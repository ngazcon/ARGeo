#pragma once

#include "Camera.h"

namespace argeo
{
	///
	/// A controller changes the state of the associated camera in each frame update.
	///
	class ICameraController
	{
	public:

		///
		/// Update the camera state.
		///
		virtual void update(Camera* Camera) = 0;
	};
}
