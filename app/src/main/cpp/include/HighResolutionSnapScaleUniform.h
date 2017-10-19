#pragma once

#include "DrawAutomaticUniform.h"
#include "Uniform.h"
#include "Vector.h"

namespace argeo
{
	class HighResolutionSnapScaleUniform : public DrawAutomaticUniform
	{
	public:
		HighResolutionSnapScaleUniform(IUniform* uniform);
		~HighResolutionSnapScaleUniform() { }

		void set(UniformState* uniform_state);

	private:
		Uniform<float>* m_uniform;
	};
}
