#pragma once

#include "DrawAutomaticUniform.h"
#include "Uniform.h"
#include "Vector.h"

namespace argeo
{
	class CameraEyeHighModelCoordinatesUniform : public DrawAutomaticUniform
	{
	public:
		CameraEyeHighModelCoordinatesUniform(IUniform* uniform);
		~CameraEyeHighModelCoordinatesUniform() { }

		void set(UniformState* uniform_state);

	private:
		Uniform<vec3>* m_uniform;
	};
}


