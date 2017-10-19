#include "UniformFloatVector4ArrayGL.h"

#include "GL.h"

namespace argeo
{
	UniformFloatVector4ArrayGL::UniformFloatVector4ArrayGL(std::string name, int location, int resource_size, ICleanableObserver* observer)
		: UniformArray< vec4 >(name, UniformType::FloatVector4, location, resource_size, observer)
	{
		std::vector<vec4> values(resource_size);

		std::fill_n(
			reinterpret_cast<vec4*>(values.data()),
			resource_size,
			vec4(0.0, 0.0, 0.0, 0.0));

		set_values(values);
	}


	UniformFloatVector4ArrayGL::~UniformFloatVector4ArrayGL()
	{ }

	void UniformFloatVector4ArrayGL::clean()
	{
		// Set the uniform value.
		glUniform4fv(
			m_location,
			m_size,
			reinterpret_cast<GLfloat*>(m_values.data()));

		// Set as clean.
		m_dirty = false;
	}
}