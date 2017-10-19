#include "CameraEyeLowModelCoordinatesUniform.h"

#include "UniformState.h"
#include "DrawCommand.h"

namespace argeo
{
	CameraEyeLowModelCoordinatesUniform::CameraEyeLowModelCoordinatesUniform(IUniform* uniform)
		: m_uniform(static_cast<Uniform<vec3>*>(uniform))
	{ }

	void CameraEyeLowModelCoordinatesUniform::set(UniformState* uniform_state)
	{
		m_uniform->set_value(uniform_state->get_camera_eye_low_model_coordinates());
	}
}
