#pragma once

#include "Matrix.h"
#include "Geocentric3D.h"
#include "Geodetic2D.h"
#include "Geodetic3D.h"

#include <vector>

namespace argeo
{
	class Ellipsoid
	{
		///
		/// A class representing an ellipsoid.
		///

	private:
		static const vec3d WGS84_RADII;

	public:

		///
		/// Construct an ellipsoid using Three semi-axes.
		/// @constructor
		/// @param {double} x The x semi-axis.
		/// @param {double} y The y semi-axis.
		/// @param {double} z The z semi-axis.
		/// @exception {std::out_of_range} x, y and z must be greater than 0.
		///
		Ellipsoid(
			double x, 
			double y,
			double z);

		///
		/// Construct an ellipsoid using Three semi-axes.
		/// @constructor
		/// @param {vec3d} radii A vector containing the three semi-axes.
		/// @exception {std::out_of_range} radii elements must be greater than 0.
		///
		Ellipsoid(const vec3d& radii);

		///
		/// @destructor
		///
		~Ellipsoid();
		
		///
		/// Get the ellipsoid's semi-axes radii.
		/// @return {vec3d} A vector containing the three semi-axes.
		///
		const vec3d get_radii() const;

		///
		/// Get the ellipsoid's semi-axes radii squared.
		/// @return {vec3d} A vector containing the three semi-axes squared.
		///
		const vec3d get_radii_squared() const;

		///
		/// Get one over the ellipsoid's semi-axes radii squared.
		/// @return {vec3d} A vector containing one over the three semi-axes squared.
		///
		const vec3d get_one_over_radii_squared() const;

		///
		/// Get the mimimum ellipsoid's semi-axis radi.
		/// @return {double} The minimum ellipsoid's radi.
		///
		double get_minimum_radius() const;

		///
		/// Get the maximum ellipsoid's semi-axis radi.
		/// @return {double} The maximum ellipsoid's radi.
		///
		double get_maximum_radius() const;

		///
		/// Converts an ellipoid centered position to a geodetic2D position.
		/// @param {vec3d} position_on_ellipsoid The position on the ellipsoid.
		/// @return {Geodetic2D} The geodetic position over the ellipsoid.
		///
		Geodetic2D to_geodetic2D(const Geocentric3D& geocentric) const;

		/// Converts an ellipoid centered position to a geodetic3D position.
		/// @param {Geocentric3D} geocentric The geocentric position.
		/// @return {Geodetic3D} The geodetic position over the ellipsoid.
		///
		Geodetic3D to_geodetic3D(const Geocentric3D& geocentric) const;
		
		///
		/// Converts an ellipoid's geodetic position to a ellipsoid centered position.
		/// @param {Geodetic2D} geodetic The geodetic position over the ellipsoid. 
		/// @return {vec3d} The position on the ellipsoid.
		///
		Geocentric3D to_geocentric3D(const Geodetic2D& geodetic) const;

		///
		/// Converts an ellipoid's geodetic position to a ellipsoid centered position.
		/// @param {Geodetic3D} geodetic The geodetic position over the ellipsoid. 
		/// @return {Geocentric3D} The position on the ellipsoid.
		///
		Geocentric3D to_geocentric3D(const Geodetic3D& geodetic) const;

		///
		/// Converts a list of ellipoid's geodetic position to a list of ellipsoid centered position.
		/// @param {std::vector<Geodetic3D>} geodetics The geodetic position over the ellipsoid. 
		/// @return {std::vector<Geocentric3D>} A list with the geocentric positions.
		///
		std::vector<Geocentric3D> to_geocentric3D(std::vector<Geodetic3D> geodetics) const;

		///
		/// Get the scale of an ellipoid centered position
		/// @param {Geodetic3D} geodetic The geodetic position over the ellipsoid. 
		/// @return {Geocentric3D} The scale of an ellipoid's geodetic position.
		///
		Geocentric3D scale_to_geodetic_surface(const Geodetic3D& geodetic) const;

		///
		/// Get the scale of an ellipoid's geodetic position.
		/// @param {Geocentric3D} position_on_ellipsoid The position on the ellipsoid.
		/// @return {Geocentric3D} The scale of an ellipoid's geodetic position.
		///
		Geocentric3D scale_to_geodetic_surface(const Geocentric3D& position) const;

		///
		/// Scales the provided Cartesian position along the geodetic surface normal
		/// so that it is on the surface of this ellipsoid
		/// @param {vec3d} position_on_ellipsoid The position on the ellipsoid.
		/// @return {Geodetic3D} The geodetic position over the ellipsoid.
		///	
		Geocentric3D scale_to_geocentric_surface(const Geocentric3D& position) const;
		
		///
		/// Get the geodetic surface normal.
		/// @param {Geodetic3D} geodetic The ellipsoid's geodetic position.
		/// @return {vec3d} The geodetic surface normal.
		///	
		Geocentric3D geodetic_surface_normal(const Geodetic3D& geodetic) const;

		///
		/// Get the geodetic surface normal.
		/// @param {vec3d} position_on_ellipsoid The position on the ellipsoid.
		/// @return {vec3d} The geodetic surface normal.
		///	
		Geocentric3D geodetic_surface_normal(const Geocentric3D& position_on_Ellipsoid) const;

        Geocentric3D get_surface_normal_intersection_with_z_axis(
                const Geocentric3D& position,
                const double& buffer = 0.0);

		static Ellipsoid wgs84();
		static Ellipsoid scaled_wgs84();
		static Ellipsoid unit_sphere();

	private:
		vec3d m_radii;
		vec3d m_radii_squared;
		vec3d m_radii_to_the_fourth;
		vec3d m_one_over_radii_squared;
		vec3d m_one_over_radii;
        double m_center_tolerance_squared;
        double m_squared_x_over_squared_z;
	};
}
