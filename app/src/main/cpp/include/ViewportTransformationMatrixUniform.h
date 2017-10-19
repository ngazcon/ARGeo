#pragma once

#include "DrawAutomaticUniform.h"
#include "Uniform.h"
#include "Matrix.h"

namespace argeo
{
	class ViewportTransformationMatrixUniform : public DrawAutomaticUniform
	{
	public:
		ViewportTransformationMatrixUniform(IUniform* uniform);
		~ViewportTransformationMatrixUniform() { }

		void set(UniformState* uniform_state);

	private:
		Uniform<mat4>* m_uniform;
	};
}
