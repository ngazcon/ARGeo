#include "InverseProjectionMatrixUniform.h"

#include "UniformState.h"
#include "DrawCommand.h"

namespace argeo
{
	InverseProjectionMatrixUniform::InverseProjectionMatrixUniform(IUniform* uniform)
		: m_uniform(static_cast<Uniform<mat4>*>(uniform))
	{ }

	void InverseProjectionMatrixUniform::set(UniformState* uniform_state)
	{
		m_uniform->set_value(to_mat4(mat_inverse(uniform_state->get_projection_matrix())));
	}
}
