#pragma once

#include <cmath>

#include "Geodetic2D.h"
#include "Geodetic3D.h"
#include "Vector.h"

namespace argeo
{
	///
	/// A class representing a geocentric position.
	/// The position is given by a x and a y, and a z coordinates with the origin in the earth center.
	///
	class Geocentric2D
	{

	public:

		///
		/// Construct a geocentric position from a x and y coordinates.
		/// @constructor
		/// @param {double} [x = 0.0] x The x coordinate.
		/// @param {double} [y = 0.0] y The y coordinate.
		///
		Geocentric2D(
			const double& x = 0.0,
			const double& y = 0.0);

		///
		/// Construct a geocentric position from vector containing a x and y coordinates.
		/// @constructor
		/// @param {vec2d} XY The XY coordinates.
		///
		Geocentric2D(const vec2d& xy);

		///
		/// @destructor
		///
		~Geocentric2D();

		///
		/// Get the x coordinate.
		/// @result {double} The x coordinate.
		///
		double get_x() const;

		///
		/// Get the y coordinate.
		/// @result {double} The y coordinate.
		///
		double get_y() const;

		///
		/// Check's if the position is equals to an other position.
		/// @param {Geocentric2D} other The position to compare with.
		/// @result {bool} If the position is equals to the other.
		///
		bool equals(const Geocentric2D& other) const;

		Geocentric2D normalize() const;

		double dot(const Geocentric2D& other) const;

		double length() const;

		vec2d to_vec2d() const;

	private:
		double m_x;
		double m_y;
	};

	inline bool operator ==(
		const Geocentric2D& left,
		const Geocentric2D& right)
	{
		return left.equals(right);
	}

	inline bool operator !=(
		const Geocentric2D& left,
		const Geocentric2D& right)
	{
		return !left.equals(right);
	}

	inline Geocentric2D operator*(
		const Geocentric2D& a,
		const Geocentric2D& b)
	{
		Geocentric2D res(
			a.get_x() * b.get_x(),
			a.get_y() * b.get_y());
		return res;
	}

	inline Geocentric2D operator*(
		const Geocentric2D& a, 
		const double& b)
	{
		Geocentric2D res(
			a.get_x() * b,
			a.get_y() * b);
		return res;
	}

	inline Geocentric2D operator+(
		const Geocentric2D& a,
		const Geocentric2D& b)
	{
		Geocentric2D res(
			a.get_x() + b.get_x(),
			a.get_y() + b.get_y());
		return res;
	}

	inline Geocentric2D operator+(
		const Geocentric2D& a,
		const double& b)
	{
		Geocentric2D res(
			a.get_x() + b,
			a.get_y() + b);
		return res;
	}

	inline Geocentric2D operator/(
		const Geocentric2D& a,
		const double& b)
	{
		Geocentric2D res(
			a.get_x() / b,
			a.get_y() / b);
		return res;
	}

	inline Geocentric2D operator/(
		const Geocentric2D& a,
		const Geocentric2D& b)
	{
		Geocentric2D res(
			a.get_x() / b.get_x(),
			a.get_y() / b.get_y());
		return res;
	}

	inline Geocentric2D operator-(
		const Geocentric2D& a,
		const Geocentric2D& b)
	{
		Geocentric2D res(
			a.get_x() - b.get_x(),
			a.get_y() - b.get_y());
		return res;
	}

}