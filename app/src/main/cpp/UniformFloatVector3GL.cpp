#include "UniformFloatVector3GL.h"

#include "GL.h"

namespace argeo
{
	UniformFloatVector3GL::UniformFloatVector3GL(std::string name, int location, ICleanableObserver* observer)
		: Uniform< vec3 >(name, UniformType::FloatVector3, location, observer)
	{
		set_value(vec3(0.0, 0.0, 0.0));
	}


	UniformFloatVector3GL::~UniformFloatVector3GL()
	{ }

	void UniformFloatVector3GL::clean()
	{
		// Set the uniform value.
		glUniform3f(
			m_location,
			m_value.x,
			m_value.y,
			m_value.z);

		// Set as clean.
		m_dirty = false;
	}
}