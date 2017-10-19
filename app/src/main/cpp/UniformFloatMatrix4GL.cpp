#include "UniformFloatMatrix4GL.h"

#include "GL.h"

namespace argeo
{
	UniformFloatMatrix4GL::UniformFloatMatrix4GL(std::string name, int location, ICleanableObserver* observer)
		: Uniform< mat4 >(name, UniformType::FloatMatrix44, location, observer)
	{
		set_value(mat4::identity());
	}


	UniformFloatMatrix4GL::~UniformFloatMatrix4GL()
	{ }

	void UniformFloatMatrix4GL::clean()
	{
		// Set the uniform value.
		glUniformMatrix4fv(
			m_location,
			1,
			false,
			m_value.data);

		// Set as clean.
		m_dirty = false;
	}
}