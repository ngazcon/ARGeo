#include "GeographicProjection.h"

namespace argeo
{
	GeographicProjection::GeographicProjection(const Ellipsoid& ellipsoid)
		: m_ellipsoid(ellipsoid)
	{
		m_semi_major_axis = m_ellipsoid.get_maximum_radius();
		m_one_over_semi_major_axis = 1.0 / m_semi_major_axis;
	}

	GeographicProjection::~GeographicProjection()
	{ }

	
	Geocentric3D GeographicProjection::project(const Geodetic3D& geodetic) const
	{
		// Actually this is the special case of equidistant cylindrical called the plate carree
		return Geocentric3D(
			geodetic.get_longitude() * m_semi_major_axis,
			geodetic.get_latitude() * m_semi_major_axis,
			geodetic.get_height());
	}

	Geodetic3D GeographicProjection::unproject(const Geocentric3D& geocentric) const
	{
		return Geodetic3D(
			geocentric.get_x() * m_one_over_semi_major_axis,
			geocentric.get_y() * m_one_over_semi_major_axis,
			geocentric.get_z());
	}

	Ellipsoid GeographicProjection::get_ellipsoid() const
	{
		return m_ellipsoid;
	}
}