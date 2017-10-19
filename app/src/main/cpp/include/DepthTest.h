#pragma once

#include "DepthTestFunction.h"

namespace argeo
{
	class DepthTest
	{
	public:

		DepthTest(
			bool enabled			   = false,
			DepthTestFunction function = DepthTestFunction::LessOrEqual)
			: enabled(enabled)
			, function(function)
		{ }

		bool enabled;
		DepthTestFunction function;
	};
}