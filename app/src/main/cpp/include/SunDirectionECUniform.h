#pragma once

#include "DrawAutomaticUniform.h"
#include "Uniform.h"
#include "Matrix.h"

namespace argeo
{
	class SunDirectionECUniform : public DrawAutomaticUniform
	{
	public:
		SunDirectionECUniform(IUniform* uniform);
		~SunDirectionECUniform() { }

		void set(UniformState* uniform_state);

	private:
		Uniform<vec3>* m_uniform;
	};
}
