#pragma once

#include <string>

namespace argeo
{
	static const std::string pass_through =
R"(
uniform sampler2D u_texture;

in  vec2 fsTextureCoordinates;
out vec4 fragColor;

void main() 
{
    fragColor = texture(u_texture, fsTextureCoordinates);
}
)";
}