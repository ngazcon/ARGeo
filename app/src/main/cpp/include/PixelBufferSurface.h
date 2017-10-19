#pragma once

#include "Surface.h"

namespace argeo
{
	class PixelBufferSurface : public Surface
	{
	public:

		virtual unsigned int  get_width() const = 0;
		virtual void set_width(const unsigned int& value) = 0;

		virtual unsigned int  get_height() const = 0;
		virtual void set_height(const unsigned int& value) = 0;

		virtual Context* get_context() = 0;
	};
}
