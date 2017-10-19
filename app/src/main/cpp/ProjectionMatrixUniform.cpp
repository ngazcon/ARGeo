#include "ProjectionMatrixUniform.h"

#include "UniformState.h"
#include "DrawCommand.h"

namespace argeo
{
	ProjectionMatrixUniform::ProjectionMatrixUniform(IUniform* uniform)
		: m_uniform(static_cast<Uniform<mat4>*>(uniform))
	{ }

	void ProjectionMatrixUniform::set(UniformState* uniform_state)
	{
		m_uniform->set_value(to_mat4(uniform_state->get_projection_matrix()));
	}
}
