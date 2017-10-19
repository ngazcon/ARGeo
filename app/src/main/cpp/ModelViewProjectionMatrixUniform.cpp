#include "ModelViewProjectionMatrixUniform.h"

#include "UniformState.h"
#include "DrawCommand.h"

namespace argeo
{
	ModelViewProjectionMatrixUniform::ModelViewProjectionMatrixUniform(IUniform* uniform)
		: m_uniform(static_cast<Uniform<mat4>*>(uniform))
	{ }

	void ModelViewProjectionMatrixUniform::set(UniformState* uniform_state)
	{
		m_uniform->set_value(to_mat4(uniform_state->get_model_view_projection_matrix()));
	}
}
