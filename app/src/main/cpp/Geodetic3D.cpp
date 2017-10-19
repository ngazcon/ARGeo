#include "Geodetic3D.h"

#include <stdexcept>

namespace argeo
{
	Geodetic3D::Geodetic3D(
		Geodetic2D geodetic2D,
		double height)
		: Geodetic3D(
			geodetic2D.get_longitude(),
			geodetic2D.get_latitude(),
			height)
	{ }

	Geodetic3D::Geodetic3D(
		double longitude,
		double latitude,
		double height)
		: m_longitude(longitude)
		, m_latitude(latitude)
		, m_height(height)
	{
		if ((longitude < -180.0) || (longitude > 180.0))
		{
			throw std::out_of_range("longitude");
		}

		if ((latitude < -90.0) || (latitude > 90.0))
		{
			throw std::out_of_range("latitude");
		}

	}
	Geodetic3D::~Geodetic3D()
	{ }

	double Geodetic3D::get_longitude() const
	{
		return m_longitude;
	}

	double Geodetic3D::get_latitude() const
	{
		return m_latitude;
	}

	double Geodetic3D::get_height() const
	{
		return m_height;
	}

	bool Geodetic3D::equals(Geodetic3D other) const
	{
		return m_longitude == other.m_longitude 
			&& m_latitude  == other.m_latitude
			&& m_height    == other.m_height;
	}

	Geodetic3D Geodetic3D::undefined()
	{
		return Geodetic3D(
				std::numeric_limits<double>::quiet_NaN(),
				std::numeric_limits<double>::quiet_NaN(),
				std::numeric_limits<double>::quiet_NaN());
	}
}