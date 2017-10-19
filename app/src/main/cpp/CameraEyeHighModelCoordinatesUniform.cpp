#include "CameraEyeHighModelCoordinatesUniform.h"

#include "UniformState.h"
#include "DrawCommand.h"

namespace argeo
{
	CameraEyeHighModelCoordinatesUniform::CameraEyeHighModelCoordinatesUniform(IUniform* uniform)
		: m_uniform(static_cast<Uniform<vec3>*>(uniform))
	{ }

	void CameraEyeHighModelCoordinatesUniform::set(UniformState* uniform_state)
	{
		m_uniform->set_value(uniform_state->get_camera_eye_high_model_coordinates());
	}
}
