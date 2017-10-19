#pragma once

namespace argeo
{
	static const char billboard_vs[] =
		R"(
in vec3 positionHigh;
in vec3 positionLow;

in float origin;

in vec4 textureCoordinatesAndImageSize;
in vec4 pickColor;
in vec4 color;

in vec4 directionAndPixelOffset;

out vec2 fsTextureCoordinates;
#ifdef RENDER_FOR_PICK
out vec4 fsPickColor;
#else
out vec4 fsColor;
#endif
		
void main()
{
	const float originScales[3] = float[](0.0, 1.0, -1.0);
	int horizontalOrigin  = (int(origin) & 3);         // bits 0-1
	int verticalOrigin    = (int(origin) & 12) >> 2;   // bits 2-3
		
	// Get the window coordinates of the center position.
	vec4 positionEC = og_modelViewRelativeToEyeMatrix * 
					  og_translateRelativeToEye(positionHigh, positionLow);
	vec4 center     = og_eyeToWindowCoordinates(positionEC);
		
	vec2 halfSize = textureCoordinatesAndImageSize.zw * og_highResolutionSnapScale;
	halfSize     *= directionAndPixelOffset.xy;

	center.xy  += halfSize;
	center.xy  += (vec2(originScales[horizontalOrigin], originScales[verticalOrigin]) * halfSize);
	center.xy  += (directionAndPixelOffset.zw * og_highResolutionSnapScale);
		
	// Translate the vector to the corner quad.
	vec4 pos = vec4(
		center.xy,
		-center.z,
		1.0);
		
	// Outputs.
	gl_Position = og_viewportOrthographicMatrix * pos;
	
#ifdef RENDER_FOR_PICK
	fsPickColor	= pickColor;
#else					
	fsColor		= color;
#endif

	fsTextureCoordinates = textureCoordinatesAndImageSize.xy;
}
		
)";

	static const char billboard_fs[] =
R"(

in vec2 fsTextureCoordinates;
#ifdef RENDER_FOR_PICK
in vec4 fsPickColor;
#else
in vec4 fsColor;
#endif

out vec4 fragmentColor;

uniform sampler2D u_atlas;

void main()
{
#ifdef RENDER_FOR_PICK
	vec4 vertexColor = vec4(1.0, 1.0, 1.0, 1.0);
#else
	vec4 vertexColor = fsColor;
#endif

	vec4 color = texture(u_atlas, fsTextureCoordinates) * vertexColor;

	if (color.a == 0.0)
	{
		discard;
	}

#ifdef RENDER_FOR_PICK
	fragmentColor = fsPickColor;
#else
	fragmentColor = color;
#endif
}
)";

	
}

