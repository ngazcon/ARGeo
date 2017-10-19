#pragma once

#include "DrawAutomaticUniform.h"
#include "Uniform.h"
#include "Matrix.h"

namespace argeo
{
	class MorphTimeUniform : public DrawAutomaticUniform
	{
	public:
		MorphTimeUniform(IUniform* uniform);
		~MorphTimeUniform() { }

		void set(UniformState* uniform_state);

	private:
		Uniform<float>* m_uniform;
	};
}
