#pragma once

#include "DrawAutomaticUniform.h"
#include "Uniform.h"
#include "Matrix.h"

namespace argeo
{
	class ModelViewProjectionMatrixUniform : public DrawAutomaticUniform
	{
	public:
		ModelViewProjectionMatrixUniform(IUniform* uniform);
		~ModelViewProjectionMatrixUniform() { }

		void set(UniformState* uniform_state);

	private:
		Uniform<mat4>* m_uniform;
	};
}
