#pragma once

#include "DrawAutomaticUniform.h"
#include "Uniform.h"
#include "Matrix.h"

namespace argeo
{
	class ModelViewRelativeToEyeMatrixUniform : public DrawAutomaticUniform
	{
	public:
		ModelViewRelativeToEyeMatrixUniform(IUniform* uniform);
		~ModelViewRelativeToEyeMatrixUniform() { }

		void set(UniformState* uniform_state);

	private:
		Uniform<mat4>* m_uniform;
	};
}
