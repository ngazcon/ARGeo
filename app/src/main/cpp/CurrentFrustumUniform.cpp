#include "CurrentFrustumUniform.h"

#include "UniformState.h"
#include "DrawCommand.h"

namespace argeo
{
	CurrentFrustumUniform::CurrentFrustumUniform(IUniform* uniform)
		: m_uniform(static_cast<Uniform<vec2>*>(uniform))
	{ }

	void CurrentFrustumUniform::set(UniformState* uniform_state)
	{
		m_uniform->set_value(uniform_state->get_current_frustum());
	}
}
