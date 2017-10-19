#include "Geocentric2D.h"

#include <stdexcept>

namespace argeo
{
	Geocentric2D::Geocentric2D(
		const double& x,
		const double& y)
		: m_x(x)
		, m_y(y)
	{ }

	Geocentric2D::Geocentric2D(const vec2d& xy)
		: m_x(xy.x)
		, m_y(xy.y)
	{ }

	Geocentric2D::~Geocentric2D()
	{ }

	double Geocentric2D::get_x() const
	{
		return m_x;
	}

	double Geocentric2D::get_y() const
	{
		return m_y;
	}

	bool Geocentric2D::equals(const Geocentric2D& other) const
	{
		return m_x == other.m_x
			&& m_y == other.m_y;
	}

	Geocentric2D Geocentric2D::normalize() const
	{
		return *this / Geocentric2D(length());
	}

	double Geocentric2D::dot(const Geocentric2D& other) const
	{
		return
			m_x * other.get_x() +
			m_y * other.get_y();
	}

	double Geocentric2D::length() const
	{
		return std::sqrt(dot(*this));
	}

	vec2d Geocentric2D::to_vec2d() const
	{
		return vec2d(m_x, m_y);
	}
}