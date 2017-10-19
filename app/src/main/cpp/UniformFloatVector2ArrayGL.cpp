#include "UniformFloatVector2ArrayGL.h"

#include "GL.h"

namespace argeo
{
	UniformFloatVector2ArrayGL::UniformFloatVector2ArrayGL(std::string name, int location, int resource_size, ICleanableObserver* observer)
		: UniformArray< vec2 >(name, UniformType::FloatVector2, location, resource_size, observer)
	{
		std::vector<vec2> values(resource_size);

		std::fill_n(
			reinterpret_cast<vec2*>(values.data()),
			resource_size,
			vec2(0.0, 0.0));

		set_values(values);
	}


	UniformFloatVector2ArrayGL::~UniformFloatVector2ArrayGL()
	{ }

	void UniformFloatVector2ArrayGL::clean()
	{
		// Set the uniform value.
		glUniform2fv(
			m_location,
			m_size,
			reinterpret_cast<GLfloat*>(m_values.data()));

		// Set as clean.
		m_dirty = false;
	}
}