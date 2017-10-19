#include "UniformFloatMatrix42GL.h"

#include "GL.h"

namespace argeo
{
	UniformFloatMatrix42GL::UniformFloatMatrix42GL(std::string name, int location, ICleanableObserver* observer)
		: Uniform< mat42 >(name, UniformType::FloatMatrix44, location, observer)
	{ }


	UniformFloatMatrix42GL::~UniformFloatMatrix42GL()
	{ }

	void UniformFloatMatrix42GL::clean()
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