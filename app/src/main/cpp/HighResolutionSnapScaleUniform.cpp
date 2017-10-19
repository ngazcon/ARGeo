#include "HighResolutionSnapScaleUniform.h"

#include "UniformState.h"
#include "DrawCommand.h"

namespace argeo
{
	HighResolutionSnapScaleUniform::HighResolutionSnapScaleUniform(IUniform* uniform)
		: m_uniform(static_cast<Uniform<float>*>(uniform))
	{ }

	void HighResolutionSnapScaleUniform::set(UniformState* uniform_state)
	{
		m_uniform->set_value((float)uniform_state->get_high_resolution_snap_scale());
	}
}
