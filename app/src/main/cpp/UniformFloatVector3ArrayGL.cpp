#include "UniformFloatVector3ArrayGL.h"

#include "GL.h"

namespace argeo
{
	UniformFloatVector3ArrayGL::UniformFloatVector3ArrayGL(std::string name, int location, int resource_size, ICleanableObserver* observer)
		: UniformArray< vec3 >(name, UniformType::FloatVector3, location, resource_size, observer)
	{ 
		std::vector<vec3> values(resource_size);

		std::fill_n(
			reinterpret_cast<vec3*>(values.data()),
			resource_size,
			vec3(0.0, 0.0, 0.0));

		set_values(values);
	}


	UniformFloatVector3ArrayGL::~UniformFloatVector3ArrayGL()
	{ }

	void UniformFloatVector3ArrayGL::clean()
	{
		// Set the uniform value.
		glUniform3fv(
			m_location,
			m_size,
			reinterpret_cast<GLfloat*>(m_values.data()));

		// Set as clean.
		m_dirty = false;
	}
}