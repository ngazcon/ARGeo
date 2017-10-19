#pragma once

#include "Plane.h"
#include "Intersect.h"
#include "Matrix.h"
#include "Interval.h"

#include <memory>

namespace argeo
{
	class BoundingVolume
	{
	public:
		
		///
		/// Check if the given plane intersect this volume.
		/// @param {Plane} plane The plane.
		/// @return {Intersect} Intersect::Outside if the plane doesn't intersect this volume, Intersect::Inside if the plane is contained by this volume
		/// Intersect::Intersect if the plane cuts this volume.
		///
		virtual Intersect intersect(const Plane& plane) const = 0;


		///
		/// Get the resulting volume by appling an affine transform to this volume.
		/// @param {mat4d} affine_matrix The affine transformation.
		/// @result {std::unique_ptr<BoundingVolume>} The resulting volume.
		///
		virtual std::unique_ptr<BoundingVolume> transform(const mat4d& affine_matrix) = 0;


		virtual Interval compute_plane_distances(
			const vec3d& position,
			const vec3d& direction) = 0;

		virtual BoundingVolume* clone(BoundingVolume* result = nullptr) = 0;
	};
}
