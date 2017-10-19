#include "ModelViewProjectionRelativeToEyeMatrixUniform.h"

#include "UniformState.h"
#include "DrawCommand.h"

namespace argeo
{
	ModelViewProjectionRelativeToEyeMatrixUniform::ModelViewProjectionRelativeToEyeMatrixUniform(IUniform* uniform)
		: m_uniform(static_cast<Uniform<mat4>*>(uniform))
	{ }

	void ModelViewProjectionRelativeToEyeMatrixUniform::set(UniformState* uniform_state)
	{
		m_uniform->set_value(
			to_mat4(uniform_state->get_model_view_projection_matrix_relative_to_eye()));
	}
}
