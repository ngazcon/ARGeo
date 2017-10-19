#pragma once

#include "BoundingVolume.h"
#include "EllipsoidTangentPlane.h"
#include "Matrix.h"

#include "Ellipsoid.h"
#include "GeodeticExtent.h"

namespace argeo
{
	class OrientedBoundingBox : public BoundingVolume
	{
	public:

		///
		/// Creates an instance of an OrientedBoundingBox.
		/// An OrientedBoundingBox of some object is a closed and convex cuboid. It can provide a tighter bounding volume than {@link BoundingSphere} or {@link AxisAlignedBoundingBox} in many cases.
		/// @constructor
		///
		/// @param {vec3d} [center=Cartesian3.ZERO] The center of the box.
		/// @param {mat3d} [halfAxes=Matrix3.ZERO] The three orthogonal half-axes of the bounding box.
		/// Equivalently, the transformation matrix, to rotate and scale a 2x2x2 cube centered at the origin.
		///
		///
		/// @example
		/// // Create an OrientedBoundingBox using a transformation matrix, a position where the box will be translated, and a scale.
		/// vec3d center   = new Cesium.Cartesian3(1.0, 0.0, 0.0);
		/// mat3d halfAxes = Cesium.Matrix3.fromScale(new Cesium.Cartesian3(1.0, 3.0, 2.0), new Cesium.Matrix3());
		///
		/// var obb = new Cesium.OrientedBoundingBox(center, halfAxes);
		///
		OrientedBoundingBox(
			const vec3d& center    = vec3d(),
			const mat3d& half_axes = mat3d());


		///
		/// @destructor
		///
		~OrientedBoundingBox();

		///
		/// Computes an OrientedBoundingBox that bounds a {@link GeodeticExtent} on the surface of an {@link Ellipsoid}.
		/// There are no guarantees about the orientation of the bounding box.
		///
		/// @param {GeodeticExtent} rectangle The cartographic rectangle on the surface of the ellipsoid.
		/// @param {double} [min_height=0.0] The minimum height (elevation) within the tile.
		/// @param {double} [max_height=0.0] The maximum height (elevation) within the tile.
		/// @param {Ellipsoid} [ellipsoid=Ellipsoid::wgs84()] The ellipsoid on which the rectangle is defined.
		/// @returns {OrientedBoundingBox} The modified result parameter or a new OrientedBoundingBox instance if none was provided.
		///
		/// @exception {std::invalid_argument} the extent width must be between 0 and pi.
		/// @exception {std::invalid_argument} the extent height must be between 0 and pi.
		/// @exception {std::invalid_argument} ellipsoid must be an ellipsoid of revolution (<code>radii.x == radii.y</code>)
		///
		static std::unique_ptr<OrientedBoundingBox> from_rectangle(
			const GeodeticExtent& extent,
			const double& min_height   = 0.0,
			const double& max_height   = 0.0,
			const Ellipsoid& ellipsoid = Ellipsoid::wgs84());


		///
		/// Computes an OrientedBoundingBox given extents in the east-north-up space of the tangent plane.
		///
		/// @param {double} minimum_x Minimum X extent in tangent plane space.
		/// @param {double} maximum_x Maximum X extent in tangent plane space.
		/// @param {double} minimum_y Minimum Y extent in tangent plane space.
		/// @param {double} maximum_y Maximum Y extent in tangent plane space.
		/// @param {double} minimum_z Minimum Z extent in tangent plane space.
		/// @param {double} maximum_z Maximum Z extent in tangent plane space.
		/// @returns {OrientedBoundingBox} The modified result parameter or a new OrientedBoundingBox instance if one was not provided.
		///
		static std::unique_ptr<OrientedBoundingBox> from_tangent_plane_extents(
			EllipsoidTangentPlane tangent_plane, 
			const double& minimum_x, 
			const double& maximum_x,
			const double& minimum_y, 
			const double& maximum_y,
			const double& minimum_z,
			const double& maximum_z);

		///
		/// @copydoc BoundingVolume::intersect(const Plane&)
		///
		Intersect intersect(const Plane& plane) const;

		///
		/// @copydoc BoundingVolume::transform(const mat4d&)
		///
		std::unique_ptr<BoundingVolume> transform(const mat4d& affine_matrix);


        void set_center(const vec3d& value);
        void set_half_axes(const mat3d& value);

		Interval compute_plane_distances(
			const vec3d& position,
			const vec3d& direction);

		BoundingVolume* clone(BoundingVolume* result = nullptr);

	private:
		mat3d m_half_axes;
		vec3d m_center;

	};
}
