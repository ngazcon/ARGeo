#pragma once

#include "BoundingVolume.h"
#include "Vector.h"
#include "Matrix.h"
#include "GeodeticExtent.h"
#include "MapProjection.h"

#include <memory>
#include <vector>

namespace argeo
{
	class BoundingSphere : public BoundingVolume
	{
	public:

		///
		/// Construct a sphere that encloses a volume.
		/// @constructor
		/// @param {vec3d} center The center of the sphere.
		/// @param {double} radius The radius of the sphere.
		///
		BoundingSphere(
			const vec3d&  center = vec3d::zero(),
			const double& radius = 0.0);

		///
		/// @destructor
		///
		~BoundingSphere();

		///
		/// @copydoc BoundingVolume::intersect(const Plane&)
		///
		Intersect intersect(const Plane& plane) const;
		
		///
		/// @copydoc BoundingVolume::transform(const mat4d&)
		///
		std::unique_ptr<BoundingVolume> transform(const mat4d& affine_matrix);
		
		///
		/// Get the resulting volume by appling an affine transform to this volume without
		/// taking into account the scale.
		/// @param {mat4d} affine_matrix The affine transformation.
		/// @result {std::unique_ptr<BoundingVolume>} The resulting volume.
		///
		std::unique_ptr<BoundingVolume> transform_without_scale(const mat4d& affine_matrix);

		///
		/// Computes a bounding sphere from the corner points of an axis-aligned bounding box.  The sphere
		/// tighly and fully encompases the box.
		/// @param {vec3d} [corner] The minimum height over the rectangle.
		/// @param {vec3d} [oppositeCorner] The maximum height over the rectangle.
		/// @return {std::unique_ptr<BoundingSphere>} The modified result parameter or a new BoundingSphere instance if none was provided.
		///
		static std::unique_ptr<BoundingSphere> from_corner_points(
			const vec3d& corner,
			const vec3d& opposite_corner);

		///
		///Computes a bounding sphere by enlarging the provided sphere to contain the provided point.
		///@param {Cartesian3} point A point to enclose in a bounding sphere.
		///@returns {BoundingSphere} The modified result parameter or a new BoundingSphere instance if none was provided.
		///
		std::unique_ptr<BoundingSphere> expand(const vec3d& point);

		///
		///Computes a tight-fitting bounding sphere enclosing a list of 3D Cartesian points.
		///The bounding sphere is computed by running two algorithms, a naive algorithm and
		///Ritter's algorithm. The smaller of the two spheres is used to ensure a tight fit.
		///
		///@param {Cartesian3[]} positions An array of points that the bounding sphere will enclose.  Each point must have <code>x</code>, <code>y</code>, and <code>z</code> properties.
		///@returns {BoundingSphere} The modified result parameter or a new BoundingSphere instance if one was not provided.
		///@see {@link http://blogs.agi.com/insight3d/index.php/2008/02/04/a-bounding/|Bounding Sphere computation article}
		///
		static std::unique_ptr<BoundingSphere> from_points(const std::vector<vec3d>& points);

		Interval compute_plane_distances(
			const vec3d& position,
			const vec3d& direction);

        double distance_squared_to(const vec3d& point);

		static std::unique_ptr<BoundingSphere> from_rectangle_with_heights_2D(
				const GeodeticExtent& extent,
				MapProjection* projection,
				const double& minimum_height,
                const double& maximum_height);

        static std::unique_ptr<BoundingSphere> from_rectangle_3D(
                const GeodeticExtent& extent,
                const Ellipsoid& ellipsoid   = Ellipsoid::wgs84(),
                const double& surface_height = 0.0);

		///
		/// Get the center point of this sphere.
		/// @result {vec3d} The center point.
		///
		vec3d get_center() const;

		///
		/// Get the radius of this sphere.
		/// @result {double} The radius.
		///
		double get_radius() const;

		void set_center(const vec3d& value);

		void set_radius(const double& value);

		bool equals(const BoundingSphere& other) const;

        BoundingVolume* clone(BoundingVolume* result = nullptr);

	private:
		vec3d  m_center;
		double m_radius;
	};
}
