#pragma once

#include "UniformState.h"

namespace argeo
{
	class DrawAutomaticUniform
	{
	public:

		virtual void set(UniformState* uniform_state) = 0;
	};
}
