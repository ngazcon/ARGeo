#include "LightPropertiesUniform.h"

#include "UniformState.h"
#include "DrawCommand.h"

namespace argeo
{
	LightPropertiesUniform::LightPropertiesUniform(IUniform* uniform)
		: m_uniform(static_cast<Uniform<vec4>*>(uniform))
	{
	}

	void LightPropertiesUniform::set(UniformState* uniform_state)
	{
		m_uniform->set_value(vec4(
			uniform_state->get_diffuse_intensity(),
			uniform_state->get_specular_intensity(),
			uniform_state->get_ambient_intensity(),
			uniform_state->get_shininess()));
	}
}
