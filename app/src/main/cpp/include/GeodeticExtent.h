#pragma once

#include "Geodetic2D.h"
#include "Ellipsoid.h"

#include <vector>


namespace argeo
{
	class GeodeticExtent
	{
		///
		/// A class representing a geodetic extent in an ellipsoid.
		/// The extent is given by a four positions:
		/// North: The northenmost latitude.
		/// South: The southernmost latitude.
		/// East: The easternmost longitude.
		/// West: The westernmost longitude.
		///
	public:

		///
		/// Construct a geodetic extent in an ellipsoid given by four positions.
		/// @param {double} west The westernmost longitude.
		/// @param {double} south The southernmost latitude.
		/// @param {double} east The easternmost longitude.
		/// @param {double} north The northenmost latitude.
		///
		GeodeticExtent(
			double west  = 0.0, 
			double south = 0.0,
			double east  = 0.0, 
			double north = 0.0);

		///
		/// Construct a geodetic extent in an ellipsoid given by two extremes geodetic positions.
		/// @param {double} bottom_left Contains the westernmost longitude and southernmost latitude.
		/// @param {double} top_right Contains the easternmost longitude and northernmost latitude.
		///
		GeodeticExtent(
			const Geodetic2D& bottom_left,
			const Geodetic2D& top_right);

		///
		/// Destructor
		///
		~GeodeticExtent();

		///
		/// Get the height in radians of this extent.
		/// @return {double} The height in radians.
		///
		double get_height() const;

		///
		/// Get the width in radians of this extent.
		/// @return {double} The width in radians.
		///
		double get_width() const;

		///
		/// Get the westernmost point in radians of this extent.
		/// @return {double} The westernmost point in radians.
		///
		double get_west() const;

		///
		/// Get the southernmost point in radians of this extent.
		/// @return {double} The southernmost point in radians.
		///
		double get_south() const;

		///
		/// Get the easternmost point in radians of this extent.
		/// @return {double} The easternmost point in radians.
		///
		double get_east() const;

		///
		/// Get the northernmost point in radians of this extent.
		/// @return {double} The northernmost point in radians.
		///
		double get_north() const;

		///
		/// Get the center geodetic position in radians of this extent.
		/// @return {double} The center geodetic position in radians.
		///
		Geodetic2D get_center() const;


		///
		/// Get the center geodetic position in radians of this extent.
		/// @return {double} The center geodetic position in radians.
		///
		Geodetic2D get_south_west() const;

        ///
        /// Get the center geodetic position in radians of this extent.
        /// @return {double} The center geodetic position in radians.
        ///
        Geodetic2D get_south_east() const;


        ///
        /// Get the center geodetic position in radians of this extent.
        /// @return {double} The center geodetic position in radians.
        ///
        Geodetic2D get_north_west() const;


        ///
        /// Get the center geodetic position in radians of this extent.
        /// @return {double} The center geodetic position in radians.
        ///
        Geodetic2D get_north_east() const;


        std::vector<vec3d> subsample(
                const Ellipsoid& ellipsoid   = Ellipsoid::wgs84(),
                const double& surface_height = 0.0) const;

		///
		/// Check's if the extent is equals to an other extent.
		/// @param {GeodeticExtent} other The extent to compare with.
		/// @result {bool} If the extent is equals to the other.
		///
		bool equals(const GeodeticExtent& other) const;

        bool contains(const Geodetic2D& geodetic) const;

		static GeodeticExtent from_degrees(
			double west,
			double south,
			double east,
			double north);

	private:
		double m_west;
		double m_south;
		double m_east;
		double m_north;
	};

	inline bool operator==(
		const GeodeticExtent& left, 
		const GeodeticExtent& right)
	{
		return left.equals(right);
	}

	inline bool operator!=(
		const GeodeticExtent& left, 
		const GeodeticExtent& right)
	{
		return !left.equals(right);
	}
}
