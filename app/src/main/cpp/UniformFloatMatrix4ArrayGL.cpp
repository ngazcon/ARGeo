#include "UniformFloatMatrix4ArrayGL.h"

#include "GL.h"

namespace argeo
{
	UniformFloatMatrix4ArrayGL::UniformFloatMatrix4ArrayGL(std::string name, int location, int resource_size, ICleanableObserver* observer)
		: UniformArray< mat4 >(name, UniformType::FloatMatrix44, location, resource_size, observer)
	{ 
		std::vector<mat4> values(resource_size);

		std::fill_n(
			reinterpret_cast<mat4*>(values.data()),
			resource_size,
			mat4::identity());

		set_values(values);
	}


	UniformFloatMatrix4ArrayGL::~UniformFloatMatrix4ArrayGL()
	{ }

	void UniformFloatMatrix4ArrayGL::clean()
	{
		// Set the uniform value.
		glUniformMatrix4fv(
			m_location,
			m_size,
			false,
			reinterpret_cast<GLfloat*>(m_values.data()));

		// Set as clean.
		m_dirty = false;
	}
}