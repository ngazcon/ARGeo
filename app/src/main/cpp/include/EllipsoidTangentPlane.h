#pragma once

#include "Geocentric2D.h"
#include "Geocentric3D.h"
#include "Ellipsoid.h"
#include "Plane.h"

#include <cmath>

namespace argeo
{
	class EllipsoidTangentPlane
	{
	public:

		///
		/// A plane tangent to the provided ellipsoid at the provided origin.
		/// If origin is not on the surface of the ellipsoid, it's surface projection will be used.
		/// If origin is at the center of the ellipsoid, an exception will be thrown.
		/// @constructor
		/// @param {Geodetic3D} origin The point on the surface of the ellipsoid where the tangent plane touches.
		/// @param {Ellipsoid} [ellipsoid=Ellipsoid::wgs84()] The ellipsoid to use.
		/// @exception {std::invalid_argument} origin must not be at the center of the ellipsoid.
		///
		EllipsoidTangentPlane(
			const Geodetic3D& origin,
			const Ellipsoid&  ellipsoid = Ellipsoid::wgs84());

		///
		/// A plane tangent to the provided ellipsoid at the provided origin.
		/// If origin is not on the surface of the ellipsoid, it's surface projection will be used.
		/// If origin is at the center of the ellipsoid, an exception will be thrown.
		/// @constructor
		/// @param {vec3d} origin The point on the surface of the ellipsoid where the tangent plane touches.
		/// @param {Ellipsoid} [ellipsoid=Ellipsoid::wgs84()] The ellipsoid to use.
		/// @exception {std::invalid_argument} origin must not be at the center of the ellipsoid.
		///
		EllipsoidTangentPlane(
			const Geocentric3D& origin,
			const Ellipsoid& ellipsoid = Ellipsoid::wgs84());


		///
		/// @destructor
		///
		~EllipsoidTangentPlane();

		///
		/// Computes the projection of the provided 3D position onto the 2D plane, radially outward from the {@link EllipsoidTangentPlane.ellipsoid} coordinate system origin.
		/// @param {vec3d} point The point to project.
		/// @returns {vec2d} The projected point. std::nan if there is no intersection point
		///
		Geocentric2D project_point_onto_plane(const Geocentric3D& point);

		///
		/// Computes the projection of the provided 3D positions onto the 2D plane (where possible), radially outward from the global origin.
		/// The resulting array may be shorter than the input array - if a single projection is impossible it will not be included.
		/// @param {std::vector<vec3d>} points The array of points to project.
		/// @returns {std::vector<vec2d>} The projected points.
		/// 
		std::vector<Geocentric2D> project_points_onto_plane(std::vector<Geocentric3D> points);

		///
		/// Computes the projection of the provided 3D position onto the 2D plane, along the plane normal.
		/// @param {vec3d} point The point to project.
		/// @returns {vec2d} The projected point. std::nan if there is no intersection point
		///
		Geocentric2D project_point_to_nearest_onto_plane(const Geocentric3D& point);

		///
		/// Computes the projection of the provided 3D positions onto the 2D plane, along the plane normal.
		/// The resulting array may be shorter than the input array - if a single projection is impossible it will not be included.
		/// @param {std::vector<vec3d>} points The array of points to project.
		/// @returns {std::vector<vec2d>} The projected points.
		/// 
		std::vector<Geocentric2D> project_points_to_nearest_onto_plane(std::vector<Geocentric3D> points);

		///
		/// Get the underlying plane.
		/// @returns {Plane} The underlying plane.
		/// 
		Plane get_plane() const;

		///
		/// Get the x axis.
		/// @returns {Geocentric3D} The x axis.
		/// 
		Geocentric3D get_x_axis() const;

		///
		/// Get the y axis.
		/// @returns {Geocentric3D} The y axis.
		/// 
		Geocentric3D get_y_axis() const;

		///
		/// Get the normal of the plane.
		/// @returns {Geocentric3D} The normal of the plane.
		/// 
		Geocentric3D get_normal() const;

		///
		/// Get the origin of the plane.
		/// @returns {Geocentric3D} The origin of the plane.
		/// 
		Geocentric3D get_origin() const;

	private:
		Ellipsoid    m_ellipsoid;
		Geocentric3D m_origin;
					 
		Geocentric3D m_x_axis;
		Geocentric3D m_y_axis;

		Plane     m_plane;
	};
}
