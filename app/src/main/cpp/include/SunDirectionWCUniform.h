#pragma once

#include "DrawAutomaticUniform.h"
#include "Uniform.h"
#include "Matrix.h"

namespace argeo
{
	class SunDirectionWCUniform : public DrawAutomaticUniform
	{
	public:
		SunDirectionWCUniform(IUniform* uniform);
		~SunDirectionWCUniform() { }

		void set(UniformState* uniform_state);

	private:
		Uniform<vec3>* m_uniform;
	};
}
