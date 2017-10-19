#include "Geocentric3D.h"
#include "Ellipsoid.h"

namespace argeo
{
	Geocentric3D::Geocentric3D(
		const double& x,
		const double& y,
		const double& z)
		: m_x(x)
		, m_y(y)
		, m_z(z)
	{ }

	Geocentric3D::Geocentric3D(const vec3d& xyz)
		: m_x(xyz.x)
		, m_y(xyz.y)
		, m_z(xyz.z)
	{ }

	Geocentric3D::~Geocentric3D()
	{ }

	double Geocentric3D::get_x() const
	{
		return m_x;
	}

	double Geocentric3D::get_y() const
	{
		return m_y;
	}

	double Geocentric3D::get_z() const
	{
		return m_z;
	}

	bool Geocentric3D::equals(const Geocentric3D& other) const
	{
		return m_x == other.m_x
			&& m_y == other.m_y
			&& m_z == other.m_z;
	}

	Geocentric3D Geocentric3D::normalize() const
	{
		return *this / length();
	}

	double Geocentric3D::dot(const Geocentric3D& other) const
	{
		return
			m_x * other.get_x() +
			m_y * other.get_y() +
			m_z * other.get_z();
	}

	double Geocentric3D::length() const
	{
		return std::sqrt(dot(*this));
	}

	vec3d Geocentric3D::to_vec3d() const
	{
		return vec3d(m_x, m_y, m_z);
	}

	Geocentric3D Geocentric3D::from_degrees(
			const double& longitude,
			const double& latitude,
			const double& height,
			const Ellipsoid& ellipsoid)
	{
        return from_radians(
                ArgeoMath::to_radians(longitude),
                ArgeoMath::to_radians(latitude),
                height,
                ellipsoid);
	}

	Geocentric3D Geocentric3D::from_radians(
			const double& longitude,
			const double& latitude,
			const double& height,
			const Ellipsoid& ellipsoid)
	{
        const vec3d radii_squared = ellipsoid.get_radii_squared();
        const double cos_latitude = ::cos(latitude);

        vec3d n(
                cos_latitude * ::cos(longitude),
                cos_latitude * ::sin(longitude),
                ::sin(latitude));

        n = vec_normalize(n);
        vec3d k = n * radii_squared;

        const double gamma = ::sqrt(vec_dot(n, k));

        k /= gamma;
        n  = n * height;

        return k + n;
	}

	Geocentric3D Geocentric3D::undefined()
	{
		return Geocentric3D(
				std::numeric_limits<double>::quiet_NaN(),
				std::numeric_limits<double>::quiet_NaN(),
				std::numeric_limits<double>::quiet_NaN());
	}
}