#pragma once

#include "DrawAutomaticUniform.h"
#include "Uniform.h"
#include "Vector.h"

namespace argeo
{
	class CameraEyeUniform : public DrawAutomaticUniform
	{
	public:
		CameraEyeUniform(IUniform* uniform);
		~CameraEyeUniform() { }

		void set(UniformState* uniform_state);

	private:
		Uniform<vec3>* m_uniform;
	};
}
