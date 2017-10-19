#include "UniformIntArrayGL.h"

#include "GL.h"

namespace argeo
{
	UniformIntArrayGL::UniformIntArrayGL(
		std::string name,
		int location, 
		int resource_size, 
		UniformType type,
		ICleanableObserver* observer)
		: UniformArray<int>(name, type, location, resource_size, observer)
	{ 
		std::vector<int> values(resource_size);

		std::fill_n(
			reinterpret_cast<int*>(values.data()),
			resource_size,
			0);

		set_values(values);
	}

	UniformIntArrayGL::~UniformIntArrayGL()
	{ }

	void UniformIntArrayGL::clean()
	{
		// Set the uniform value.
		glUniform1iv(
			m_location,
			m_size,
			static_cast<GLint*>(m_values.data()));

		// Set as clean.
		m_dirty = false;
	}
}