#pragma once

namespace argeo
{
	static const char common_vs[] =
		R"(
in vec3 positionHigh;
in vec3 positionLow;
in vec3 normal;

out vec3 fsPositionEC;
out vec3 fsNormalEC;
	
#ifdef HAS_TEXTURE_COORDINATES
	in  vec2 textureCoordinates;
	out vec2 fsTextureCoordinates;
#endif

#ifdef APPLY_NORMAL_MAPPING
	in  vec3 tangent;
	in  vec3 bitangent;
	out vec3 fsTangentEC;
	out vec3 fsBitangentEC;
#endif

void main()																										 
{		
	vec4 positionEC			= og_modelViewRelativeToEyeMatrix *
							  og_translateRelativeToEye(positionHigh, positionLow);

	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	fsPositionEC			= - positionEC.xyz;

	fsNormalEC				= og_normalMatrix * normal;

#ifdef APPLY_NORMAL_MAPPING
	fsTangentEC				= og_normalMatrix * tangent;
	fsBitangentEC			= og_normalMatrix * bitangent;
#endif

#ifdef HAS_TEXTURE_COORDINATES
	fsTextureCoordinates    = textureCoordinates;
#endif

	gl_Position				= og_projectionMatrix * positionEC;
}

)";

	static const char common_fs[] =
		R"(
in vec3 fsPositionEC;
in vec3 fsNormalEC;

#ifdef HAS_TEXTURE_COORDINATES
	in vec2 fsTextureCoordinates;
#endif

#ifdef APPLY_NORMAL_MAPPING
	in vec3 fsTangentEC;
	in vec3 fsBitangentEC;	
#endif

layout (location = 0) out vec4 fragColor;

void main()
{
	vec3 positionToEyeEC		 =  normalize(fsPositionEC);
	vec3 positionToLightEC		 =  normalize(og_sunDirectionEC + fsPositionEC);
	vec3 normalEC				 =  normalize(fsNormalEC);

	og_materialInput materialInput;
	materialInput.positionToEyeEC    = positionToEyeEC;
	materialInput.normalEC		     = normalEC;

#ifdef HAS_TEXTURE_COORDINATES
	materialInput.st			     = fsTextureCoordinates;
#endif

#ifdef APPLY_NORMAL_MAPPING
	materialInput.tangentToEyeMatrix = og_tangentToEyeSpaceMatrix(
			 fsNormalEC, 
			 fsTangentEC,
			 fsBitangentEC);

#endif

	// Get the material
	og_material material			 = og_getMaterial(materialInput);

#ifdef APPLY_NORMAL_MAPPING
	// Calculate the normal in tangent space.
	material.normal					 = materialInput.tangentToEyeMatrix * material.normal;
	material.normal				     = normalize(material.normal);

	// Calculate the light and eye position in tangent eye.
	positionToEyeEC   *= materialInput.tangentToEyeMatrix;
	positionToLightEC *= materialInput.tangentToEyeMatrix;
#endif

	fragColor = og_phong(
			positionToEyeEC, 
			positionToLightEC,
			material);
}
)";
}