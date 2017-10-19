#pragma once

#include "DrawAutomaticUniform.h"
#include "Uniform.h"
#include "Vector.h"

namespace argeo
{
	class CameraEyeLowModelCoordinatesUniform : public DrawAutomaticUniform
	{
	public:
		CameraEyeLowModelCoordinatesUniform(IUniform* uniform);
		~CameraEyeLowModelCoordinatesUniform() { }

		void set(UniformState* uniform_state);

	private:
		Uniform<vec3>* m_uniform;
	};
}


