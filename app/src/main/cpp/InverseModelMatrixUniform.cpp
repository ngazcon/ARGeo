#include "InverseModelMatrixUniform.h"

#include "UniformState.h"
#include "DrawCommand.h"

namespace argeo
{
	InverseModelMatrixUniform::InverseModelMatrixUniform(IUniform* uniform)
		: m_uniform(static_cast<Uniform<mat4>*>(uniform))
	{ }

	void InverseModelMatrixUniform::set(UniformState* uniform_state)
	{
		m_uniform->set_value(to_mat4(uniform_state->get_inverse_model_matrix()));
	}
}
