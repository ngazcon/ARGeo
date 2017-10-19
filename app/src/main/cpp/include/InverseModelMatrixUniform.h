#pragma once

#include "DrawAutomaticUniform.h"
#include "Uniform.h"
#include "Matrix.h"

namespace argeo
{
	class InverseModelMatrixUniform : public DrawAutomaticUniform
	{
	public:
		InverseModelMatrixUniform(IUniform* uniform);
		~InverseModelMatrixUniform() { }

		void set(UniformState* uniform_state);

	private:
		Uniform<mat4>* m_uniform;
	};
}
