#include "CameraLightPositionUniform.h"

#include "UniformState.h"
#include "DrawCommand.h"

namespace argeo
{
	CameraLightPositionUniform::CameraLightPositionUniform(IUniform* uniform)
		: m_uniform(static_cast<Uniform<vec3>*>(uniform))
	{
	}

	void CameraLightPositionUniform::set(UniformState* uniform_state)
	{
		m_uniform->set_value(to_vec3(uniform_state->get_camera_light_position()));
	}
}
