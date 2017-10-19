#include "FragmentOutputsGL.h"
#include "ShaderProgramGL.h"

#include "GL.h"

namespace argeo
{
	FragmentOutputsGL::FragmentOutputsGL(ShaderProgram* program)
		: m_program(program)
	{ }


	FragmentOutputsGL::~FragmentOutputsGL()
	{ }

	int FragmentOutputsGL::get_fragment_output(std::string name) const
	{
		ShaderProgramGL* program = static_cast<ShaderProgramGL*>(m_program);

		int i = glGetFragDataLocation(
			program->get_handle(),
			name.c_str());

		if (i == -1)
		{
			throw std::invalid_argument(name);
		}

		return i;
	}
}