#pragma once

#include "DrawAutomaticUniform.h"
#include "Uniform.h"
#include "Matrix.h"

namespace argeo
{
	class ModelZToClipCoordinatesUniform : public DrawAutomaticUniform
	{
	public:
		ModelZToClipCoordinatesUniform(IUniform* uniform);
		~ModelZToClipCoordinatesUniform() { }

		void set(UniformState* uniform_state);

	private:
		Uniform<mat42>* m_uniform;
	};
}
