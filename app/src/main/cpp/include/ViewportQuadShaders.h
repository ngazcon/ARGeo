#pragma once

namespace argeo
{
	static const char viewport_quad_vs[] =
R"(
	in vec4 position;
	in vec2 textureCoordinates;

	out vec2 fsTextureCoordinates;

	void main()
	{
		fsTextureCoordinates = textureCoordinates;
		gl_Position		     = position;
	}
)";

	static const char viewport_quad_fs[] =
R"(
in  vec2 fsTextureCoordinates;
out vec4 fragmentColor;

uniform sampler2D og_texture0;

void main()
{
	fragmentColor = texture(og_texture0, fsTextureCoordinates);
}
)";

}

