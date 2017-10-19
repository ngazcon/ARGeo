#pragma once

namespace argeo
{
	static const char depth_plane_vs[] =
		R"(
in  vec4 position;
out vec4 positionEC;

void main()
{
	 positionEC  = og_modelViewMatrix  * position;
     gl_Position = og_projectionMatrix * positionEC;
})";

	static const char depth_plane_fs[] =
		R"(
in vec4  positionEC;
out vec4 fragColor;

void main()
{
	// TODO: make arbitrary ellipsoid
    og_ellipsoid ellipsoid = og_getWgs84EllipsoidEC();

    vec3 direction = normalize(positionEC.xyz);
    og_ray ray     = og_ray(vec3(0.0), direction);
    
    og_raySegment intersection = og_rayEllipsoidIntersectionInterval(ray, ellipsoid);
    if (!og_isEmpty(intersection))
    {
        fragColor = vec4(1.0, 1.0, 0.0, 1.0);
    }
    else
    {
        discard;
    }
})";

}

