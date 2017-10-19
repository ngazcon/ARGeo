#pragma once

#include "DrawAutomaticUniform.h"
#include "Uniform.h"
#include "Matrix.h"

namespace argeo
{
	class ViewportUniform : public DrawAutomaticUniform
	{
	public:
		ViewportUniform(IUniform* uniform);
		~ViewportUniform() { }

		void set(UniformState* uniform_state);

	private:
		Uniform<vec4>* m_uniform;
	};
}
