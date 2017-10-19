#include "ViewportOrthographicMatrixUniform.h"

#include "UniformState.h"
#include "DrawCommand.h"
#include "Context.h"

namespace argeo
{
	ViewportOrthographicMatrixUniform::ViewportOrthographicMatrixUniform(IUniform* uniform)
		: m_uniform(static_cast<Uniform<mat4>*>(uniform))
	{ }

	void ViewportOrthographicMatrixUniform::set(UniformState* uniform_state)
	{
		m_uniform->set_value(to_mat4(
			uniform_state->get_viewport_orthographic_matrix()));
	}
}
