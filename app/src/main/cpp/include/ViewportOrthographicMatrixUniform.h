#pragma once

#include "DrawAutomaticUniform.h"
#include "Uniform.h"
#include "Matrix.h"

namespace argeo
{
	class ViewportOrthographicMatrixUniform : public DrawAutomaticUniform
	{
	public:
		ViewportOrthographicMatrixUniform(IUniform* uniform);
		~ViewportOrthographicMatrixUniform() { }

		void set(UniformState* uniform_state);

	private:
		Uniform<mat4>* m_uniform;
	};
}
