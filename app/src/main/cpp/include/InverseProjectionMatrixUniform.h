#pragma once

#include "DrawAutomaticUniform.h"
#include "Uniform.h"
#include "Matrix.h"

namespace argeo
{
	class InverseProjectionMatrixUniform : public DrawAutomaticUniform
	{
	public:
		InverseProjectionMatrixUniform(IUniform* uniform);
		~InverseProjectionMatrixUniform() { }

		void set(UniformState* uniform_state);

	private:
		Uniform<mat4>* m_uniform;
	};
}

