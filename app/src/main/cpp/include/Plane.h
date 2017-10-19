#pragma once

#include "Vector.h"

namespace argeo
{
	class Plane
	{
		///
		/// Represents a plane in the 3D space.
		///

	public:

		///
		/// Construct a plane with the given normal and distance.
		/// @constructor
		/// @param {vec3d} normal The normal of the plane.
		/// @param {double} distance The distance to origin of the plane.
		///
		Plane(
			const vec3d& normal    = vec3d::unit_x(),
			const double& distance = 0.0);

		///
		/// Construct a plane with the given normal and distance.
		/// @constructor
		/// @param {vec4d} normal The normal of the plane and distance to origin of the plane.
		///
		Plane(
			const vec4d& normal_and_distance);

		///
		/// Get the distance to the origin.
		/// @return {double} Distance to origin of this plane.
		///
		double get_distance() const;

		///
		/// Get the normal.
		/// @return {vec3d} Normal of this plane.
		///
		vec3d get_normal() const;

		///
		/// Get the distance to a point.
		/// @param {vec3d} point The point.
		/// @return {double} Distance from this plane to the given point.
		///
		double get_point_distance(const vec3d& point) const;


		///
		/// Construct a plane from a point and a normal.
		/// @param {vec3d} point The point.
		/// @param {vec3d} normal The normal.
		/// @return {Plane} The resulting plane.
		///
		static Plane from_point_normal(
			const vec3d& point,
			const vec3d& normal);

	private:
		vec3d  m_normal;
		double m_distance;
	};
}
