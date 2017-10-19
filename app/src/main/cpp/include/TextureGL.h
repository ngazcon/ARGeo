#pragma once

#include "TypeConverterGL.h"

namespace argeo
{
	class TextureGL
	{
	public:

		virtual void bind() const = 0;
		virtual unsigned int get_handle() const = 0;

		static void unbind(TextureTarget texture_target);
	};
}