#include "UniformFloatArrayGL.h"

#include "GL.h"

namespace argeo
{
	UniformFloatArrayGL::UniformFloatArrayGL(std::string name, int location, int resource_size, ICleanableObserver* observer)
		: UniformArray< float >(name, UniformType::Float, location, resource_size, observer)
	{ 
		std::vector<float> values(resource_size);

		std::fill_n(
			reinterpret_cast<float*>(values.data()),
			resource_size,
			0.0);

		set_values(values);
	}


	UniformFloatArrayGL::~UniformFloatArrayGL()
	{
	}

	void UniformFloatArrayGL::clean()
	{
		// Set the uniform value.
		glUniform1fv(
			m_location,
			m_size,
			static_cast< GLfloat* >(m_values.data()));

		// Set as clean.
		m_dirty = false;
	}
}