#include "CameraEyeUniform.h"

#include "DrawCommand.h"

namespace argeo
{
	CameraEyeUniform::CameraEyeUniform(IUniform* uniform)
		: m_uniform(static_cast<Uniform<vec3>*>(uniform))
	{ }

	void CameraEyeUniform::set(UniformState* uniform_state)
	{
		m_uniform->set_value(to_vec3(uniform_state->get_camera_eye()));
	}
}
