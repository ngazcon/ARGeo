#include "ModelViewRelativeToEyeMatrixUniform.h"

#include "UniformState.h"
#include "DrawCommand.h"

namespace argeo
{
	ModelViewRelativeToEyeMatrixUniform::ModelViewRelativeToEyeMatrixUniform(IUniform* uniform)
		: m_uniform(static_cast<Uniform<mat4>*>(uniform))
	{
	}

	void ModelViewRelativeToEyeMatrixUniform::set(UniformState* uniform_state)
	{
		m_uniform->set_value(
			to_mat4(uniform_state->get_model_view_matrix_relative_to_eye()));
	}
}
