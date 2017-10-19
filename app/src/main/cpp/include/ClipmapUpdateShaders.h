#pragma once

namespace argeo
{
	static const char clipmap_update_vs[] =
R"(
in vec2 position;

out vec2 fsTextureCoordinates;

uniform vec2 u_oneOverTextureSize;
uniform vec2 u_sourceOrigin;
uniform vec2 u_updateSize;
uniform vec2 u_destinationOffset;

void main()
{
	vec2 scaledPosition    = position * u_updateSize;

	gl_Position			   =
		 og_viewportOrthographicMatrix * 
		 vec4(scaledPosition + u_destinationOffset, 0.0f, 1.0f);

	fsTextureCoordinates   = (scaledPosition + u_sourceOrigin) * u_oneOverTextureSize;
}
)";

	static const char clipmap_update_fs[] =
R"(
in  vec2 fsTextureCoordinates;

uniform sampler2D u_heightMapSampler;
uniform vec2      u_maxMinHeights;

out vec4 texelOutput;

void main()
{
	float texel = og_normalizeFloat(
			texture(u_heightMapSampler, fsTextureCoordinates).r,
			u_maxMinHeights.x,
			u_maxMinHeights.y);

	texelOutput = og_packFloatToVec4(texel);
}
)";
}

