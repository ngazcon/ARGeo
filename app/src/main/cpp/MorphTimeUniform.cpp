#include "MorphTimeUniform.h"
#include "DrawCommand.h"

namespace argeo
{
	MorphTimeUniform::MorphTimeUniform(IUniform* uniform)
		: m_uniform(static_cast<Uniform<float>*>(uniform))
	{ }

	void MorphTimeUniform::set(UniformState* uniform_state)
	{
		m_uniform->set_value(uniform_state->get_morph_time());
	}
}
