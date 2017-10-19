#pragma once

#include <cmath>

#include "Geodetic2D.h"

namespace argeo
{
	///
	/// A class representing a geodetic position in an ellipsoid.
	/// The position is given by a latitude and a longitude over the ellipsoid's surface, plus a height offset.
	///
	class Geodetic3D
	{

	public:

		///
		/// Construct a geodetic position from a latitude and longitude and a height over the ellipsoid's surface.
		/// @constructor
		/// @param {double} [longitude = 0.0] longitude The longitude in radians.
		/// @param {double} [latitude = 0.0] latitude The latitude in radians.
		/// @param {double} [height = 0.0] height The height in meters.
		/// @exception {std::out_of_range} longitude must be between [-180.0, 180.0]
		/// @exception {std::out_of_range} latitude must be between [-90.0,  90.0]
		///
		Geodetic3D(
			double longitude = 0.0,
			double latitude  = 0.0,
			double height    = 0.0);

		///
		/// Construct a geodetic position from an Geodetic2D position containing latitude and longitude 
		/// and a height over the ellipsoid's surface.
		/// @constructor
		/// @param {geodetic2D} geodetic The geodetic with latitude and longitude.
		/// @param {double} [height = 0.0] height The height in meters.
		/// @exception {std::out_of_range} geodetic2D longitude must be between [-180.0, 180.0]
		/// @exception {std::out_of_range} geodetic2D latitude must be between [-90.0,  90.0]
		///
		Geodetic3D(
			Geodetic2D geodetic2D,
			double height = 0.0);

		///
		/// @destructor
		///
		~Geodetic3D();

		///
		/// Get the position's latitude in radians.
		/// @result {double} The latitude in radians.
		///
		double get_longitude() const;

		///
		/// Get the position's longitude in radians.
		/// @result {double} The longitude in radians.
		///
		double get_latitude() const;

		///
		/// Get the position's height in meters.
		/// @result {double} The height in meters.
		///
		double get_height() const;

		///
		/// Check's if the position is equals to an other position.
		/// @param {Geodetic3D} other The position to compare with.
		/// @result {bool} If the position is equals to the other.
		///
		bool equals(Geodetic3D other) const;

		static Geodetic3D undefined();

	private:
		double m_longitude;
		double m_latitude;
		double m_height;
	};

	inline bool undefined(const Geodetic3D& o)
	{
		return
			std::isnan(o.get_latitude()) ||
			std::isnan(o.get_longitude()) ||
			std::isnan(o.get_height());
	}

	inline bool operator ==(
		const Geodetic3D& left, 
		const Geodetic3D& right)
	{
		return left.equals(right);
	}

	inline bool operator !=(
		const Geodetic3D& left,
		const Geodetic3D& right)
	{
		return !left.equals(right);
	}

	inline Geodetic3D operator +(
		const Geodetic3D& left,
		const Geodetic3D& right)
	{
		return Geodetic3D(
			left.get_longitude() + right.get_longitude(),
			left.get_latitude()  + right.get_latitude(),
			left.get_height()    + right.get_height());
	}

	///
	/// Converts from a position expressed in radians to a position expressed in degrees.
	/// @param {Geodetic3D} geodetic The position in radians.
	/// @return {Geodetic3D} geodetic The position in degrees.
	///
	inline Geodetic3D to_degrees(const Geodetic3D& geodetic)
	{
		return Geodetic3D(
			ArgeoMath::to_degrees(geodetic.get_longitude()),
			ArgeoMath::to_degrees(geodetic.get_latitude()),
			geodetic.get_height());
	}

	///
	/// Converts from a position expressed in degrees to a position expressed in radians.
	/// @param {Geodetic2D} geodetic The position in degrees.
	/// @return {Geodetic2D} geodetic The position in radians.
	///
	inline Geodetic3D to_radians(const Geodetic3D& geodetic)
	{
		return Geodetic3D(
			ArgeoMath::to_radians(geodetic.get_longitude()),
			ArgeoMath::to_radians(geodetic.get_latitude()),
			geodetic.get_height());
	}

}