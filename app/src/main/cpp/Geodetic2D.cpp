#include "Geodetic2D.h"

#include <stdexcept>

namespace argeo
{
	Geodetic2D::Geodetic2D(
		double longitude,
		double latitude)
		: m_longitude(longitude)
		, m_latitude(latitude)
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

	Geodetic2D::~Geodetic2D()
	{ }

	double Geodetic2D::get_longitude() const
	{
		return m_longitude;
	}

	double Geodetic2D::get_latitude() const
	{
		return m_latitude;
	}

	bool Geodetic2D::equals_epsilon(
		const Geodetic2D& other,
		const double& epsilon) const
	{
		return ArgeoMath::equals_epsilon(m_longitude, other.m_longitude, epsilon)
			&& ArgeoMath::equals_epsilon(m_latitude, other.m_latitude, epsilon);
	}

	bool Geodetic2D::equals(const Geodetic2D& other) const
	{
		return m_longitude == other.m_longitude
			&& m_latitude  == other.m_latitude;
	}
}