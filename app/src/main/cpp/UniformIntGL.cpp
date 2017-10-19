#include "UniformIntGL.h"

#include "GL.h"

namespace argeo
{
	UniformIntGL::UniformIntGL(std::string name, int location, UniformType type, ICleanableObserver* observer)
		: Uniform<int>(name, type, location, observer)
	{
		set_value(0);
	}

	UniformIntGL::~UniformIntGL()
	{ }

	void UniformIntGL::clean()
	{
		// Set the uniform value.
		glUniform1i(
			m_location,
			m_value);

		// Set as clean.
		m_dirty = false;
	}
}