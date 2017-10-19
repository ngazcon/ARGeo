#pragma once

#include "DrawAutomaticUniform.h"
#include "Uniform.h"
#include "Vector.h"

namespace argeo
{
	class CameraLightPositionUniform : public DrawAutomaticUniform
	{
	public:
		CameraLightPositionUniform(IUniform* uniform);
		~CameraLightPositionUniform() { }

		void set(UniformState* uniform_state);

	private:
		Uniform<vec3>* m_uniform;
	};
}
