#pragma once

#include "Context.h"

namespace argeo
{
	class Surface
	{
	public:
		virtual unsigned int get_width() const = 0;
		virtual unsigned int get_height() const = 0;

		virtual Context* get_context() = 0;
	};
}
