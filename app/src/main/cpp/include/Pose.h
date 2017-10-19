#pragma once

#include "Quaternion.h"
#include "Vector.h"

namespace argeo
{
	struct Pose
	{
		Pose(
			const vec3d& translation      = vec3d(0.0),
			const quaternion& orientation = quaternion::identity())
			: translation(translation)
			, orientation(orientation)
		{ }

		// In meters
		vec3d      translation;
		quaternion orientation;
	};
}