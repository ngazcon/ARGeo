#pragma once

#include "DrawAutomaticUniform.h"
#include "Uniform.h"
#include "Matrix.h"

namespace argeo
{
	class ModelMatrixUniform : public DrawAutomaticUniform
	{
	public:
		ModelMatrixUniform(IUniform* uniform);
		~ModelMatrixUniform() { }

		void set(UniformState* uniform_state);

	private:
		Uniform<mat4>* m_uniform;
	};
}
