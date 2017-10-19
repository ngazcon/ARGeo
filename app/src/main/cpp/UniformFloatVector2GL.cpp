#include "UniformFloatVector2GL.h"

#include "GL.h"

namespace argeo
{
	UniformFloatVector2GL::UniformFloatVector2GL(std::string name, int location, ICleanableObserver* observer)
		: Uniform<vec2>(name, UniformType::FloatVector2, location, observer)
	{ 
		set_value(vec2(0.0, 0.0));
	}


	UniformFloatVector2GL::~UniformFloatVector2GL()
	{ }

	void UniformFloatVector2GL::clean()
	{
		// Set the uniform value.
		glUniform2f(
			m_location,
			m_value.x,
			m_value.y);

		// Set as clean.
		m_dirty = false;
	}
}