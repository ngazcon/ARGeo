#pragma once

#include "MapProjection.h"

namespace argeo
{
	class GeographicProjection : 
		public MapProjection
	{
	public:
		
		///
		/// A simple map projection where longitude and latitude are linearly mapped to X and Y by multiplying
		/// them by the {@link Ellipsoid#maximumRadius}.  This projection
		/// is commonly known as geographic, equirectangular, equidistant cylindrical, or plate carrée.  It
		/// is also known as EPSG:4326.
		/// @alias GeographicProjection
		/// @constructor
		/// @param {Ellipsoid} [ellipsoid=Ellipsoid.WGS84] The ellipsoid.
		/// @see WebMercatorProjection
		///
		GeographicProjection(const Ellipsoid& ellipsoid = Ellipsoid::wgs84());

		~GeographicProjection();

		
		/**
		* Unprojects a set of projected {@link Cartesian3} coordinates, in meters, to {@link Cartographic}
		* coordinates, in radians.  Longitude and Latitude are the X and Y coordinates, respectively,
		* divided by the maximum radius of the ellipsoid.  Height is the unmodified Z coordinate.
		* @param {Cartesian3} cartesian The Cartesian position to unproject with height (z) in meters.
		* @param {Cartographic} [result] An instance into which to copy the result.  If this parameter is
		*        undefined, a new instance is created and returned.

		* @returns {Cartographic} The unprojected coordinates.  If the result parameter is not undefined, the
		*          coordinates are copied there and that instance is returned.  Otherwise, a new instance is
		*          created and returned.
		*/
		Geodetic3D unproject(const Geocentric3D& geocentric) const;

		/**

		* Projects a set of {@link Cartographic} coordinates, in radians, to map coordinates, in meters.

		* X and Y are the longitude and latitude, respectively, multiplied by the maximum radius of the

		* ellipsoid.  Z is the unmodified height.

		*

		* @param {Cartographic} cartographic The coordinates to project.

		* @param {Cartesian3} [result] An instance into which to copy the result.  If this parameter is

		*        undefined, a new instance is created and returned.

		* @returns {Cartesian3} The projected coordinates.  If the result parameter is not undefined, the

		*          coordinates are copied there and that instance is returned.  Otherwise, a new instance is

		*          created and returned.

		*/
		Geocentric3D project(const Geodetic3D& geodetic) const;

		Ellipsoid get_ellipsoid() const;

	private:
		Ellipsoid m_ellipsoid;
		double m_semi_major_axis;
		double m_one_over_semi_major_axis;

	};
}
