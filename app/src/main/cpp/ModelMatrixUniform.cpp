#include "ModelMatrixUniform.h"

#include "UniformState.h"
#include "DrawCommand.h"

namespace argeo
{
	ModelMatrixUniform::ModelMatrixUniform(IUniform* uniform)
		: m_uniform(static_cast<Uniform<mat4>*>(uniform))
	{ }

	void ModelMatrixUniform::set(UniformState* uniform_state)
	{
		m_uniform->set_value(to_mat4(uniform_state->get_model_matrix()));
	}
}
