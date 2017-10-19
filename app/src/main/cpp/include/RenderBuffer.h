#pragma once

#include "RenderBufferFormat.h"
#include "RenderBufferDescription.h"

namespace argeo
{
	class RenderBuffer
	{
	public:
		virtual RenderBufferDescription get_description() const = 0;
	};
}
