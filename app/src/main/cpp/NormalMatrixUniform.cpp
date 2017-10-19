#include "NormalMatrixUniform.h"

#include "UniformState.h"
#include "DrawCommand.h"

namespace argeo
{
	NormalMatrixUniform::NormalMatrixUniform(IUniform* uniform)
		: m_uniform(static_cast<Uniform<mat3>*>(uniform))
	{ }

	void NormalMatrixUniform::set(UniformState* uniform_state)
	{
		m_uniform->set_value(to_mat3(uniform_state->get_normal_matrix()));
	}
}
