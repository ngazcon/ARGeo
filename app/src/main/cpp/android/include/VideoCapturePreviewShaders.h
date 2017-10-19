#pragma once

namespace argeo
{
	static const char video_capture_preview_vs[] =
		R"(

in vec2 position;

uniform mat4 u_transformation;
uniform mat3 u_orientation;
uniform vec2 u_ratios;

out vec2 v_texture_coord;

void main()
{
	vec3 position3D = vec3(position, 0.0f);

	vec3 rotatedAndNormalizedPosition3D = 
			(u_orientation * 
			 position3D + vec3(1.0f)) * 0.5f;

	v_texture_coord  = (u_transformation * vec4(rotatedAndNormalizedPosition3D, 1.0f)).xy;
	v_texture_coord *=  u_ratios;

	gl_Position      = vec4(position3D, 1.0f);
    //gl_Position.xy  *= u_ratios;
}

)";

	static const char video_capture_preview_fs[] =
R"(
uniform samplerExternalOES sampler;

in  vec2 v_texture_coord;
out vec4 texelOutput;

void main()
{
	texelOutput = texture(sampler, v_texture_coord);
}
)";

}

