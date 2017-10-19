#pragma once

#include "ArgeoMath.h"

namespace argeo
{
	class Ellipsoid;

	class Geodetic2D
	{
		///
		/// A class representing a geodetic position in an ellipsoid.
		/// The position is given by a latitude and a longitude over the ellipsoid's surface.
		///
	public:		

		///
		/// Construct a geodetic position from a latitude and longitude.
		/// @constructor
		/// @param {double} [longitude = 0.0] longitude The longitude in radians.
		/// @param {double} [latitude = 0.0] latitude The latitude in radians.
		/// @exception {std::out_of_range} longitude must be between [-180.0, 180.0]
		/// @exception {std::out_of_range} latitude must be between [-90.0,  90.0]
		///
		Geodetic2D(
			double longitude = 0.0,
			double latitude  = 0.0);

		///
		/// @destructor
		///
		~Geodetic2D();

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
		/// Check's if the position is near to an other position with a epsilon tolerance.
		/// @param {Geodetic2D} other The position to compare.
		/// @param {double} epsilon The tolerance.
		/// @result {bool} If the position is near to the other.
		///
		bool equals_epsilon(
			const Geodetic2D& other, 
			const double& epsilon) const;

		///
		/// Check's if the position is equals to an other position.
		/// @param {Geodetic2D} other The position to compare with.
		/// @result {bool} If the position is equals to the other.
		///
		bool equals(const Geodetic2D& other) const;

	private:
		double m_longitude;
		double m_latitude;
	};

	inline bool operator==(
		const Geodetic2D& left, 
		const Geodetic2D& right)
	{
		return left.equals(right);
	}

	inline bool operator!=(
		const Geodetic2D& left, 
		const Geodetic2D& right)
	{
		return !left.equals(right);
	}

	inline Geodetic2D operator +(
		const Geodetic2D& left,
		const Geodetic2D& right)
	{
		return Geodetic2D(
			left.get_longitude() + right.get_longitude(),
			left.get_latitude() + right.get_latitude());
	}

	///
	/// Converts from a position expressed in radians to a position expressed in degrees.
	/// @param {Geodetic2D} geodetic The position in radians.
	/// @return {Geodetic2D} geodetic The position in degrees.
	///
	inline Geodetic2D to_degrees(const Geodetic2D& geodetic)
	{
		return Geodetic2D(
			ArgeoMath::to_degrees(geodetic.get_longitude()),
			ArgeoMath::to_degrees(geodetic.get_latitude()));
	}

	///
	/// Converts from a position expressed in degrees to a position expressed in radians.
	/// @param {Geodetic2D} geodetic The position in degrees.
	/// @return {Geodetic2D} geodetic The position in radians.
	///
	inline Geodetic2D to_radians(const Geodetic2D& geodetic)
	{
		return Geodetic2D(
			ArgeoMath::to_radians(geodetic.get_longitude()),
			ArgeoMath::to_radians(geodetic.get_latitude()));
	}
	
}