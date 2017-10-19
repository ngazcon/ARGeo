namespace argeo
{
	static const char clipmap_compute_normals_vs[] =
R"(
in vec2 position;

uniform vec2 u_updateSize;
uniform vec2 u_origin;
uniform vec2 u_oneOverHeightMapSize;
		
out vec2 fsTextureCoordinates;

void main()
{
	vec2 coordinates     = position * u_updateSize + u_origin;
	gl_Position          = og_viewportOrthographicMatrix * vec4(coordinates, 0.0, 1.0);
	fsTextureCoordinates = coordinates * u_oneOverHeightMapSize;
}
)";

	static const char clipmap_compute_normals_fs[] =
R"(
in vec2 fsTextureCoordinates;
in vec2 fsPosition;

out vec3 normalOutput;

uniform float u_heightExaggeration;
uniform float u_postDelta;
uniform vec2  u_oneOverHeightMapSize;
uniform vec2  u_maxMinHeights;

uniform sampler2D u_heightMapSampler;

void main()
{
/*
	float top   = og_denormalizeFloat(
		og_unpackFloatFromVec4(texture(u_heightMapSampler, fsTextureCoordinates + vec2(0.0,  u_oneOverHeightMapSize.y))),
		u_maxMinHeights.x,
		u_maxMinHeights.y);

	float bottom   = og_denormalizeFloat(
		og_unpackFloatFromVec4(texture(u_heightMapSampler, fsTextureCoordinates + vec2(0.0,  -u_oneOverHeightMapSize.y))),
		u_maxMinHeights.x,
		u_maxMinHeights.y);

	float left   = og_denormalizeFloat(
		og_unpackFloatFromVec4(texture(u_heightMapSampler, fsTextureCoordinates + vec2(-u_oneOverHeightMapSize.x, 0.0))),
		u_maxMinHeights.x,
		u_maxMinHeights.y);

	float right   = og_denormalizeFloat(
		og_unpackFloatFromVec4(texture(u_heightMapSampler, fsTextureCoordinates + vec2(u_oneOverHeightMapSize.x, 0.0))),
		u_maxMinHeights.x,
		u_maxMinHeights.y);

    vec2 xy  =
            vec2(left  - right, bottom - top) *
            u_heightExaggeration;

	vec3 normal  = vec3(
			xy,
			2.0f   * u_postDelta);

    normalOutput  = normalize(normal) * 0.5f + vec3(0.5f);

*/

    float upperLeft   = og_denormalizeFloat(
		og_unpackFloatFromVec4(texture(u_heightMapSampler, fsTextureCoordinates + vec2(-u_oneOverHeightMapSize.x, u_oneOverHeightMapSize.y))),
		u_maxMinHeights.x,
		u_maxMinHeights.y) * u_heightExaggeration;

    float upperCenter   = og_denormalizeFloat(
		og_unpackFloatFromVec4(texture(u_heightMapSampler, fsTextureCoordinates + vec2(0.0, u_oneOverHeightMapSize.y))),
		u_maxMinHeights.x,
		u_maxMinHeights.y) * u_heightExaggeration;

    float upperRight  = og_denormalizeFloat(
		og_unpackFloatFromVec4(texture(u_heightMapSampler, fsTextureCoordinates + vec2(u_oneOverHeightMapSize.x, u_oneOverHeightMapSize.y))),
		u_maxMinHeights.x,
		u_maxMinHeights.y) * u_heightExaggeration;

	float left   = og_denormalizeFloat(
		og_unpackFloatFromVec4(texture(u_heightMapSampler, fsTextureCoordinates + vec2(-u_oneOverHeightMapSize.x, 0.0))),
		u_maxMinHeights.x,
		u_maxMinHeights.y) * u_heightExaggeration;

	float right   = og_denormalizeFloat(
		og_unpackFloatFromVec4(texture(u_heightMapSampler, fsTextureCoordinates + vec2(u_oneOverHeightMapSize.x, 0.0))),
		u_maxMinHeights.x,
		u_maxMinHeights.y) * u_heightExaggeration;

	float lowerLeft   = og_denormalizeFloat(
		og_unpackFloatFromVec4(texture(u_heightMapSampler, fsTextureCoordinates + vec2(-u_oneOverHeightMapSize.x,  -u_oneOverHeightMapSize.y))),
		u_maxMinHeights.x,
		u_maxMinHeights.y) * u_heightExaggeration;

    float lowerCenter  = og_denormalizeFloat(
		og_unpackFloatFromVec4(texture(u_heightMapSampler, fsTextureCoordinates + vec2(0.0,  -u_oneOverHeightMapSize.y))),
		u_maxMinHeights.x,
		u_maxMinHeights.y) * u_heightExaggeration;

    float lowerRight   = og_denormalizeFloat(
		og_unpackFloatFromVec4(texture(u_heightMapSampler, fsTextureCoordinates + vec2(u_oneOverHeightMapSize.x,  -u_oneOverHeightMapSize.y))),
		u_maxMinHeights.x,
		u_maxMinHeights.y) * u_heightExaggeration;


    float x = upperRight + (2.0 * right) + lowerRight - upperLeft - (2.0 * left) - lowerLeft;
    float y = lowerLeft + (2.0 * lowerCenter) + lowerRight - upperLeft - (2.0 * upperCenter) - upperRight;

    if (u_postDelta < 0.0) {
            y = -1.0;
    }

    vec3 normal   = vec3(-x, y, 1.0);
    normalOutput  = normalize(normal) * 0.5f + vec3(0.5f);
}
)";
}

