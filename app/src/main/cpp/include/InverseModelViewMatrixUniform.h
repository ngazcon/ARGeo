#pragma once

#include "DrawAutomaticUniform.h"
#include "Uniform.h"
#include "Matrix.h"

namespace argeo
{
	class InverseModelViewMatrixUniform : public DrawAutomaticUniform
	{
	public:
		InverseModelViewMatrixUniform(IUniform* uniform);
		~InverseModelViewMatrixUniform() { }

		void set(UniformState* uniform_state);

	private:
		Uniform<mat4>* m_uniform;
	};
}
