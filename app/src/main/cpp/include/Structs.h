#pragma once

#include <string>

namespace argeo
{
	/** DOC_TBA
	*
	* @name og_ellipsoid
	* @glslStruct
	*/
	static std::string ellipsoid =
R"(
struct og_ellipsoid
{
	vec3 center;
	vec3 radii;
	vec3 inverseRadii;
	vec3 inverseRadiiSquared;
};
)";

	/**
	* Holds material information that can be used for lighting. Returned by all og_getMaterial functions.
	*
	* @name og_material
	* @glslStruct
	*
	* @property {vec3} diffuse Incoming light that scatters evenly in all directions.
	* @property {float} specular Intensity of incoming light reflecting in a single direction.
	* @property {float} shininess The sharpness of the specular reflection.  Higher values create a smaller, more focused specular highlight.
	* @property {vec3} normal Surface's normal in eye coordinates. It is used for effects such as normal mapping. The default is the surface's unmodified normal.
	* @property {vec3} emission Light emitted by the material equally in all directions. The default is vec3(0.0), which emits no light.
	* @property {float} alpha Opacity of this material. 0.0 is completely transparent; 1.0 is completely opaque.
	*/
	static std::string material =
R"(
struct og_material
{
	vec3  ambient;
	vec3  diffuse;
	vec3  specular;
	vec3  normal;
	vec3  emission;
	float shininess;
	float refraction;
	float alpha;
};
)";


	/**
	* Used as input to every material's og_getMaterial function.
	*
	* @name og_materialInput
	* @glslStruct
	*
	* @property {float} s 1D texture coordinates.
	* @property {vec2} st 2D texture coordinates.
	* @property {vec3} str 3D texture coordinates.
	* @property {vec3} normalEC Unperturbed surface normal in eye coordinates.
	* @property {mat3} tangentToEyeMatrix Matrix for converting a tangent space normal to eye space.
	* @property {vec3} positionToEyeEC Vector from the fragment to the eye in eye coordinates.  The magnitude is the distance in meters from the fragment to the eye.
	*/
	static std::string material_input =
R"(
struct og_materialInput
{
	float s;
	vec2 st;
	vec3 str;
	vec3 normalEC;
	mat3 tangentToEyeMatrix;
	vec3 positionToEyeEC;
};
)";


	static std::string ray =
		R"(
struct og_ray
{
    vec3 origin;
    vec3 direction;
};
)";

	static std::string ray_segment =
		R"(
struct og_raySegment
{
    float start;
    float stop;
};

//const og_raySegment og_emptyRaySegment = og_raySegment(-og_infinity, -og_infinity);
//const og_raySegment og_fullRaySegment  = og_raySegment(0.0, og_infinity);
)";

}