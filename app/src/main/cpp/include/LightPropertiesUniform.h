#pragma once

#include "DrawAutomaticUniform.h"
#include "Uniform.h"
#include "Vector.h"

namespace argeo
{
	class LightPropertiesUniform : public DrawAutomaticUniform
	{
	public:
		LightPropertiesUniform(IUniform* uniform);
		~LightPropertiesUniform() { }

		void set(UniformState* uniform_state);

	private:
		Uniform<vec4>* m_uniform;
	};
}
