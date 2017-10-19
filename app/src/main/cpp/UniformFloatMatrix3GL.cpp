#include "UniformFloatMatrix3GL.h"

#include "GL.h"

namespace argeo
{
	UniformFloatMatrix3GL::UniformFloatMatrix3GL(std::string name, int location, ICleanableObserver* observer)
		: Uniform< mat3 >(name, UniformType::FloatMatrix33, location, observer)
	{
		set_value(mat3::identity());
	}


	UniformFloatMatrix3GL::~UniformFloatMatrix3GL()
	{ }

	void UniformFloatMatrix3GL::clean()
	{
		// Set the uniform value.
		glUniformMatrix3fv(
			m_location,
			1,
			false,
			reinterpret_cast<GLfloat*>(m_value.data));

		// Set as clean.
		m_dirty = false;
	}
}