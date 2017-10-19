#include "SunDirectionECUniform.h"


#include "UniformState.h"
#include "DrawCommand.h"

namespace argeo
{
	SunDirectionECUniform::SunDirectionECUniform(IUniform* uniform)
		: m_uniform(static_cast<Uniform<vec3>*>(uniform))
	{ }

	void SunDirectionECUniform::set(UniformState* uniform_state)
	{
		m_uniform->set_value(to_vec3(uniform_state->get_sun_direction_eye_coordinates()));
	}
}

