#include "UniformFloatVector4GL.h"

#include "GL.h"

namespace argeo
{
	UniformFloatVector4GL::UniformFloatVector4GL(std::string name, int location, ICleanableObserver* observer)
		: Uniform< vec4 >(name, UniformType::FloatVector4, location, observer)
	{
		set_value(vec4(0.0, 0.0, 0.0, 0.0));
	}


	UniformFloatVector4GL::~UniformFloatVector4GL()
	{ }

	void UniformFloatVector4GL::clean()
	{
		// Set the uniform value.
		glUniform4f(
			m_location,
			m_value.x,
			m_value.y,
			m_value.z,
			m_value.w);

		// Set as clean.
		m_dirty = false;
	}
}