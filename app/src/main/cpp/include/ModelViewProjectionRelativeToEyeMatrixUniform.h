#pragma once

#include "DrawAutomaticUniform.h"
#include "Uniform.h"
#include "Vector.h"

namespace argeo
{
	class ModelViewProjectionRelativeToEyeMatrixUniform : public DrawAutomaticUniform
	{
	public:
		ModelViewProjectionRelativeToEyeMatrixUniform(IUniform* uniform);
		~ModelViewProjectionRelativeToEyeMatrixUniform() { }

		void set(UniformState* uniform_state);

	private:
		Uniform<mat4>* m_uniform;
	};
}
