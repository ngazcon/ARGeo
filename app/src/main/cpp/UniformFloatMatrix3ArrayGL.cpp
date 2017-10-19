#include "UniformFloatMatrix3ArrayGL.h"

#include "GL.h"

namespace argeo
{
	UniformFloatMatrix3ArrayGL::UniformFloatMatrix3ArrayGL(std::string name, int location, int resource_size, ICleanableObserver* observer)
		: UniformArray< mat3 >(name, UniformType::FloatMatrix33, location, resource_size, observer)
	{ 
		std::vector<mat3> values(resource_size);

		std::fill_n(
			reinterpret_cast<mat3*>(values.data()),
			resource_size,
			mat3::identity());

		set_values(values);
	}


	UniformFloatMatrix3ArrayGL::~UniformFloatMatrix3ArrayGL()
	{ }

	void UniformFloatMatrix3ArrayGL::clean()
	{
		// Set the uniform value.
		glUniformMatrix3fv(
			m_location,
			m_size,
			false,
			reinterpret_cast<GLfloat*>(m_values.data()));

		// Set as clean.
		m_dirty = false;
	}
}