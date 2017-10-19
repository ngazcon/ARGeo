#pragma once

#include "Rectangle.h"

namespace argeo
{
	class ScissorTest
	{
	public:

		ScissorTest(
			bool enabled	    = false,
			Rectangle rectangle = Rectangle(0, 0, 0, 0))
			: enabled(enabled)
			, rectangle(rectangle)
		{ }

		bool enabled;
		Rectangle rectangle;
	};
}
