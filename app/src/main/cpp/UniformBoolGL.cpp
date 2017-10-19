#include "UniformBoolGL.h"

#include "GL.h"

namespace argeo
{
	UniformBoolGL::UniformBoolGL(std::string name, int location, ICleanableObserver* observer)
		: Uniform<bool>(name, UniformType::Bool, location, observer)
	{ 
		set_value(false);
	}


	UniformBoolGL::~UniformBoolGL()
	{ }

	void UniformBoolGL::clean()
	{
		// Set the uniform value.
		glUniform1i(
			m_location,
			m_value ? 1 : 0);

			
		// Set as clean.
		m_dirty = false;
	}
}