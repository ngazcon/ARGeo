#pragma once

#include <string>

namespace argeo
{
	static const std::string polyline_glow_material =
R"(
uniform vec4 color;
uniform float glowPower;

in float v_width;

og_material og_getMaterial(og_materialInput materialInput)
{
    og_material material = og_getDefaultMaterial(materialInput);

    vec2 st    = materialInput.st;
    float glow = glowPower / abs(st.t - 0.5) - (glowPower / 0.5);

    material.emission = max(vec3(glow - 1.0 + color.rgb), color.rgb);
    material.alpha = clamp(0.0, 1.0, glow) * color.a;

    return material;
}
)";


	static const std::string polyline_outline_material = 
R"(
uniform vec4  color;
uniform vec4  outlineColor;
uniform float outlineWidth;

in float v_width;

og_material og_getMaterial(og_materialInput materialInput)
{
    og_material material = og_getDefaultMaterial(materialInput);
    
    vec2 st = materialInput.st;
    float halfInteriorWidth =  0.5 * (v_width - outlineWidth) / v_width;
    float b = step(0.5 - halfInteriorWidth, st.t);
    b *= 1.0 - step(0.5 + halfInteriorWidth, st.t);
    
    // Find the distance from the closest separator (region between two colors)
    float d1 = abs(st.t - (0.5 - halfInteriorWidth));
    float d2 = abs(st.t - (0.5 + halfInteriorWidth));
    float dist = min(d1, d2);
    
    vec4 currentColor = mix(outlineColor, color, b);
    vec4 outColor     = og_antialias(outlineColor, color, currentColor, dist);
    
    material.diffuse = outColor.rgb;
    material.alpha = outColor.a;
    
    return material;
}
)";

}
