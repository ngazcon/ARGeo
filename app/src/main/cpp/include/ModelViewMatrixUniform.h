#pragma once

#include "DrawAutomaticUniform.h"
#include "Uniform.h"
#include "Matrix.h"

namespace argeo
{
	class ModelViewMatrixUniform : public DrawAutomaticUniform
	{
	public:
		ModelViewMatrixUniform(IUniform* uniform);
		~ModelViewMatrixUniform() { }

		void set(UniformState* uniform_state);

	private:
		Uniform<mat4>* m_uniform;
	};
}
