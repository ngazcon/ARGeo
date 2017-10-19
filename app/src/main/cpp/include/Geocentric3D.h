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
	class Geocentric3D
	{

	public:

		///
		/// Construct a geocentric position from a x and y and a z coordinates.
		/// @constructor
		/// @param {double} [x = 0.0] x The x coordinate.
		/// @param {double} [y = 0.0] y The y coordinate.
		/// @param {double} [z = 0.0] z The z coordinate.
		///
		Geocentric3D(
			const double& x = 0.0,
			const double& y = 0.0,
			const double& z = 0.0);

		///
		/// Construct a geocentric position from vector containing a x and y and a z coordinates.
		/// @constructor
		/// @param {vec3d} xyz The xyz coordinates.
		///
		Geocentric3D(const vec3d& xyz);

		///
		/// @destructor
		///
		~Geocentric3D();

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
		/// Get the z coordinate.
		/// @result {double} The z coordinate.
		///
		double get_z() const;

		///
		/// Check's if the position is equals to an other position.
		/// @param {Geocentric3D} other The position to compare with.
		/// @result {bool} If the position is equals to the other.
		///
		bool equals(const Geocentric3D& other) const;

		Geocentric3D normalize() const;

		double dot(const Geocentric3D& other) const;

		double length() const;

		vec3d to_vec3d() const;


		static Geocentric3D undefined();

		static Geocentric3D from_degrees(
				const double& longitude,
				const double& latitude,
				const double& height,
				const Ellipsoid& ellipsoid);

		static Geocentric3D from_radians(
				const double& longitude,
				const double& latitude,
                const double& height,
                const Ellipsoid& ellipsoid);

	private:
		double m_x;
		double m_y;
		double m_z;
	};

	inline bool undefined(const Geocentric3D& o)
	{
		return
				std::isnan(o.get_x()) ||
				std::isnan(o.get_y()) ||
				std::isnan(o.get_z());
	}

	inline bool operator ==(
		const Geocentric3D& left,
		const Geocentric3D& right)
	{
		return left.equals(right);
	}

	inline bool operator !=(
		const Geocentric3D& left,
		const Geocentric3D& right)
	{
		return !left.equals(right);
	}

	inline Geocentric3D operator*(
		const Geocentric3D& a,
		const Geocentric3D& b)
	{
		Geocentric3D res(
			a.get_x() * b.get_x(),
			a.get_y() * b.get_y(),
			a.get_z() * b.get_z());
		return res;
	}

	inline Geocentric3D operator*(
		const Geocentric3D& a, 
		const double& b)
	{
		Geocentric3D res(
			a.get_x() * b,
			a.get_y() * b,
			a.get_z() * b);
		return res;
	}

	inline Geocentric3D operator+(
		const Geocentric3D& a,
		const Geocentric3D& b)
	{
		Geocentric3D res(
			a.get_x() + b.get_x(),
			a.get_y() + b.get_y(),
			a.get_z() + b.get_z());
		return res;
	}

	inline Geocentric3D operator+(
		const Geocentric3D& a,
		const double& b)
	{
		Geocentric3D res(
			a.get_x() + b,
			a.get_y() + b,
			a.get_z() + b);
		return res;
	}

	inline Geocentric3D operator/(
		const Geocentric3D& a,
		const double& b)
	{
		Geocentric3D res(
			a.get_x() / b,
			a.get_y() / b,
			a.get_z() / b);
		return res;
	}

	inline Geocentric3D operator/(
		const Geocentric3D& a,
		const Geocentric3D& b)
	{
		Geocentric3D res(
			a.get_x() / b.get_x(),
			a.get_y() / b.get_y(),
			a.get_z() / b.get_z());
		return res;
	}

	inline Geocentric3D operator-(
		const Geocentric3D& a,
		const Geocentric3D& b)
	{
		Geocentric3D res(
			a.get_x() - b.get_x(),
			a.get_y() - b.get_y(),
			a.get_z() - b.get_z());
		return res;
	}

	inline Geocentric3D operator-=(
		const Geocentric3D& a,
		const Geocentric3D& b)
	{
		Geocentric3D res(
			a.get_x() - b.get_x(),
			a.get_y() - b.get_y(),
			a.get_z() - b.get_z());
		return res;
	}
}