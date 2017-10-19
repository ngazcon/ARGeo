#pragma once

#include "DrawAutomaticUniform.h"
#include "Uniform.h"
#include "Matrix.h"

namespace argeo
{
	class ProjectionMatrixUniform : public DrawAutomaticUniform
	{
	public:
		ProjectionMatrixUniform(IUniform* uniform);
		~ProjectionMatrixUniform() { }

		void set(UniformState* uniform_state);

	private:
		Uniform<mat4>* m_uniform;
	};
}

