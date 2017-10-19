#pragma once

namespace argeo
{
	static const char clipmap_upsample_vs[] =
R"(
in vec2 position;

out vec2 fsTextureCoordinates;

uniform vec2 u_sourceOrigin;
uniform vec2 u_updateSize;
uniform vec2 u_destinationOffset;
uniform vec2 u_oneOverTextureSize;

void main()
{
	vec2 scaledPosition    = position * u_updateSize;

	gl_Position			   =
			og_viewportOrthographicMatrix * 
			vec4(scaledPosition + u_destinationOffset, 0.0f, 1.0f);

	fsTextureCoordinates   = (scaledPosition * 0.5f + u_sourceOrigin) * u_oneOverTextureSize;
})";

	static const char clipmap_upsample_fs[] =
		R"(
in  vec2 fsTextureCoordinates;

uniform sampler2D u_heightMapSampler;

out vec4 texelOutput;

void main()
{
	texelOutput = texture(u_heightMapSampler, fsTextureCoordinates);
})";

}

