#pragma once

#include "FragmentOutputs.h"
#include "ShaderProgram.h"

#include <string>

namespace argeo
{
	class FragmentOutputsGL : public FragmentOutputs
	{
	public:
		FragmentOutputsGL(ShaderProgram* program);
		~FragmentOutputsGL();

		int get_fragment_output(std::string name) const;

	private:
		ShaderProgram* m_program;
	};
}
