#include "TextureGL.h"

#include "GL.h"

namespace argeo
{
	void TextureGL::unbind(TextureTarget texture_target)
	{
		glBindTexture(
			TypeConverterGL::ToEnum(texture_target),
			0);
	}	
}