#include "UniformFloatGL.h"

#include "GL.h"

namespace argeo
{
	UniformFloatGL::UniformFloatGL(std::string name, int location, ICleanableObserver* observer)
		: Uniform< float >(name, UniformType::Float, location, observer)
	{
		set_value(0.0);
	}


	UniformFloatGL::~UniformFloatGL()
	{ }

	void UniformFloatGL::clean()
	{
		// Set the uniform value.
		glUniform1f(
			m_location,
			m_value);

		// Set as clean.
		m_dirty = false;
	}
}