#pragma once

#include "Vector.h"

namespace argeo
{
	struct Ray
	{
		///
		/// Represents a Ray in 3D.
		/// The ray has a origin and a direction.
		///

		///
		/// Construct a ray with the given origin and direction.
		/// @constructor
		/// @param {vec3d} [origin = vec3d::zero()] The origin of the ray.
		/// @param {vec3d} [direction = vec3d::zero()] The direction of the ray.
		///
		Ray(
			const vec3d& origin    = vec3d::zero(),
			const vec3d& direction = vec3d::unit_x())
			: origin(origin)
			, direction(direction)
		{ }

		vec3d origin;
		vec3d direction;
	};
}
