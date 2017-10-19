#pragma once

#include "DrawAutomaticUniform.h"
#include "Uniform.h"
#include "Vector.h"

namespace argeo
{
	class CurrentFrustumUniform : public DrawAutomaticUniform
	{
	public:
		CurrentFrustumUniform(IUniform* uniform);
		~CurrentFrustumUniform() { }

		void set(UniformState* uniform_state);

	private:
		Uniform<vec2>* m_uniform;
	};
}
