#include "ViewportTransformationMatrixUniform.h"

#include "Context.h"
#include "UniformState.h"
#include "DrawCommand.h"

namespace argeo
{
	ViewportTransformationMatrixUniform::ViewportTransformationMatrixUniform(IUniform* uniform)
		: m_uniform(static_cast<Uniform<mat4>*>(uniform))
	{ }

	void ViewportTransformationMatrixUniform::set(UniformState* uniform_state)
	{
		m_uniform->set_value(to_mat4(uniform_state->get_viewport_transformation_matrix()));
	}
}