#pragma once

#include "DrawAutomaticUniform.h"
#include "Uniform.h"
#include "Matrix.h"

namespace argeo
{
	class NormalMatrixUniform : public DrawAutomaticUniform
	{
	public:
		NormalMatrixUniform(IUniform* uniform);
		~NormalMatrixUniform() { }

		void set(UniformState* uniform_state);

	private:
		Uniform<mat3>* m_uniform;
	};
}

