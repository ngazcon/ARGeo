#pragma once

namespace argeo
{
	static const char polyline_vs[] =
R"(
in vec3 positionHigh;
in vec3 positionLow;

in vec3 prevPositionHigh;
in vec3 prevPositionLow;

in vec3 nextPositionHigh;
in vec3 nextPositionLow;

in vec4 texCoordExpandWidthAndShow;
in vec4 pickColor;

out vec2  v_st;
out float v_width;

out vec4 og_pickColor;

void main()
{
	float texCoord  = texCoordExpandWidthAndShow.x;
	float expandDir = texCoordExpandWidthAndShow.y;
	float width     = abs(texCoordExpandWidthAndShow.z) + 0.5;
	float show      = texCoordExpandWidthAndShow.w;
	bool usePrev    = texCoordExpandWidthAndShow.z < 0.0;
    
	vec4 p, prev, next;
	p    = og_translateRelativeToEye(positionHigh, positionLow);
	prev = og_translateRelativeToEye(prevPositionHigh, prevPositionLow);
	next = og_translateRelativeToEye(nextPositionHigh, nextPositionLow);
    
	vec4 positionWC = getPolylineWindowCoordinates(p, prev, next, expandDir, width, usePrev);
	gl_Position     = og_viewportOrthographicMatrix * positionWC * show;

	og_pickColor    = pickColor;
	v_st		    = vec2(texCoord, clamp(expandDir, 0.0, 1.0));
	v_width		    = width;
}
)";

	static const char polyline_outline_vs[] =
R"(
in vec3 positionHigh;
in vec3 positionLow;

in vec3 prevPositionHigh;
in vec3 prevPositionLow;

in vec3 nextPositionHigh;
in vec3 nextPositionLow;

in vec4 texCoordExpandWidthAndShow;
in vec4 pickColor;

uniform float outlineWidth;

out vec2  v_st;
out float v_width;

out vec4 og_pickColor;

void main()
{
	float texCoord  = texCoordExpandWidthAndShow.x;
	float expandDir = texCoordExpandWidthAndShow.y;
	float width     = abs(texCoordExpandWidthAndShow.z) + 0.5 + 20.0;
	float show      = texCoordExpandWidthAndShow.w;
	bool usePrev    = texCoordExpandWidthAndShow.z < 0.0;
    
	vec4 p, prev, next;
	p    = og_translateRelativeToEye(positionHigh, positionLow);
	prev = og_translateRelativeToEye(prevPositionHigh, prevPositionLow);
	next = og_translateRelativeToEye(nextPositionHigh, nextPositionLow);
    
	vec4 positionWC = getPolylineWindowCoordinates(p, prev, next, expandDir, width, usePrev);
	gl_Position     = og_viewportOrthographicMatrix * positionWC * show;

	og_pickColor    = pickColor;
	v_st		    = vec2(texCoord, clamp(expandDir, 0.0, 1.0));
	v_width		    = width;
}
)";

	static const char polyline_fs[] =
R"(
out vec4 fragColor;
			
in vec2  v_st;

void main()
{
	og_materialInput materialInput;
    
    materialInput.s   = v_st.s;
    materialInput.st  = v_st;
    materialInput.str = vec3(v_st, 0.0);
    
    og_material material = og_getMaterial(materialInput);
    fragColor = vec4(material.diffuse + material.emission, material.alpha);
}
)";

static const char polyline_outline_fs[] =
R"(
out vec4 fragColor;
			
void main()
{
    fragColor = vec4(0.0, 1.0, 0.0, 1.0);
}
)";

}

