#include "UniformBoolArrayGL.h"

#include "GL.h"

namespace argeo
{
	UniformBoolArrayGL::UniformBoolArrayGL(std::string name, int location, int resource_size, ICleanableObserver* observer)
		: UniformArray< int >(name, UniformType::Bool, location, resource_size, observer)
	{ 
		std::vector<int> values(resource_size);

		std::fill_n(
			reinterpret_cast<int*>(values.data()),
			resource_size,
			false);

		set_values(values);
	}


	UniformBoolArrayGL::~UniformBoolArrayGL()
	{ }

	void UniformBoolArrayGL::clean()
	{
		// Set the uniform value.
		glUniform1iv(
			m_location,
			m_size,
			reinterpret_cast<GLint*>(m_values.data()));

		// Set as clean.
		m_dirty = false;
	}
}