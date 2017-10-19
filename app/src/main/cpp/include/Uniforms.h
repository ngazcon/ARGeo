#pragma once

#include <string>

namespace argeo
{
	static const std::string uniforms =
		R"(
uniform mat3 og_normalMatrix;

uniform mat4 og_modelViewProjectionMatrix;
uniform mat4 og_modelViewMatrix;
uniform mat4 og_modelMatrix;
uniform mat4 og_viewMatrix;
uniform mat4 og_projectionMatrix;

uniform mat4 og_inverseModelViewMatrix;
uniform mat4 og_inverseModelMatrix;
uniform mat4 og_inverseProjectionMatrix;

uniform mat4 og_modelViewRelativeToEyeMatrix;

uniform mat4 og_viewportOrthographicMatrix;
uniform mat4 og_viewportTransformationMatrix;

uniform vec2 og_currentFrustum;
uniform vec4 og_viewport;

uniform vec3 og_cameraEye;
uniform vec3 og_cameraEyeHighMC;
uniform vec3 og_cameraEyeLowMC;

uniform float og_highResolutionSnapScale;
uniform float og_morphTime;

uniform vec3 og_sunDirectionWC;
uniform vec3 og_sunDirectionEC;
uniform vec4 og_diffuseSpecularAmbientShininess;
)";

}
