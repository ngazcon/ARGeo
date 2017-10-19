#include "ModelZToClipCoordinatesUniform.h"

#include "UniformState.h"
#include "DrawCommand.h"

namespace argeo
{
	ModelZToClipCoordinatesUniform::ModelZToClipCoordinatesUniform(IUniform* uniform)
		: m_uniform(static_cast<Uniform<mat42>*>(uniform))
	{ }

	void ModelZToClipCoordinatesUniform::set(UniformState* uniform_state)
	{
		m_uniform->set_value(uniform_state->get_model_z_to_clip_coordinates());
	}
}
