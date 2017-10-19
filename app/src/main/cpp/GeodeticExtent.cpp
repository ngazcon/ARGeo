#include "GeodeticExtent.h"

namespace argeo
{

	GeodeticExtent::GeodeticExtent(
		double west,
		double south,
		double east,
		double north)
		: m_west(west)
		, m_south(south)
		, m_east(east)
		, m_north(north)
	{ }

	GeodeticExtent::GeodeticExtent(
		const Geodetic2D& bottom_left,
		const Geodetic2D& top_right)
		: m_west(bottom_left.get_longitude())
		, m_south(bottom_left.get_latitude())
		, m_east(top_right.get_longitude())
		, m_north(top_right.get_latitude())
	{ }

	GeodeticExtent::~GeodeticExtent()
	{ }

	GeodeticExtent GeodeticExtent::from_degrees(
		double west,
		double south,
		double east,
		double north)
	{
		return GeodeticExtent(
			ArgeoMath::to_radians(west),
			ArgeoMath::to_radians(south), 
			ArgeoMath::to_radians(east), 
			ArgeoMath::to_radians(north));
	}

	double GeodeticExtent::get_height() const
	{
		return m_north - m_south;
	}

	double GeodeticExtent::get_width() const
	{
		double east = m_east;
		double west = m_west;

		if (east < west) 
		{
			east += TWO_PI;
		}

		return east - west;
	}

	double GeodeticExtent::get_west() const
	{
		return m_west;
	}

	double GeodeticExtent::get_south() const
	{
		return m_south;
	}

	double GeodeticExtent::get_east() const
	{
		return m_east;
	}

	double GeodeticExtent::get_north() const
	{
		return m_north;
	}

	Geodetic2D GeodeticExtent::get_center() const
	{
		double east = m_east;
		double west = m_west;

		if (east < west)
		{
			east += TWO_PI;
		}

		double longitude = ArgeoMath::negative_pi_to_pi((west + east) * 0.5);
		double latitude  = (m_south + m_north) * 0.5;

		return Geodetic2D(
			longitude,
			latitude);
	}

	Geodetic2D GeodeticExtent::get_north_east() const
	{
		return Geodetic2D(
				m_east,
				m_north);
	}

    Geodetic2D GeodeticExtent::get_north_west() const
    {
        return Geodetic2D(
                m_west,
                m_north);
    }

    Geodetic2D GeodeticExtent::get_south_west() const
    {
        return Geodetic2D(
                m_west,
				m_south);
    }

    Geodetic2D GeodeticExtent::get_south_east() const
    {
        return Geodetic2D(
                m_east,
				m_south);
    }

    std::vector<vec3d> GeodeticExtent::subsample(
            const Ellipsoid& ellipsoid,
            const double& surface_height) const
    {
        std::vector<vec3d> result;
        result.push_back(ellipsoid.to_geocentric3D(Geodetic3D(
                m_west,
                m_north,
                surface_height)).to_vec3d());

        result.push_back(ellipsoid.to_geocentric3D(Geodetic3D(
                m_east,
                m_north,
                surface_height)).to_vec3d());

        result.push_back(ellipsoid.to_geocentric3D(Geodetic3D(
                m_east,
                m_south,
                surface_height)).to_vec3d());


        result.push_back(ellipsoid.to_geocentric3D(Geodetic3D(
                m_west,
                m_south,
                surface_height)).to_vec3d());

        double lat;
        double lon = 0.0;

        if (m_north < 0.0)
        {
            lat = m_north;
        }
        else if (m_south > 0.0)
        {
            lat = m_south;
        }
        else
        {
            lat = 0.0;
        }

        for (unsigned int i = 1; i < 8; ++i)
        {
            lon = -PI + i * PI_OVER_TWO;

            if (contains(Geodetic2D(lat, lon)))
            {
                result.push_back(ellipsoid.to_geocentric3D(Geodetic3D(
                        lon,
                        lat,
                        surface_height)).to_vec3d());
            }
        }

        if (lat == 0.0)
        {
            result.push_back(ellipsoid.to_geocentric3D(Geodetic3D(
                    lon,
                    lat,
                    surface_height)).to_vec3d());


            result.push_back(ellipsoid.to_geocentric3D(Geodetic3D(
                    m_east,
                    lat,
                    surface_height)).to_vec3d());
        }

        return result;
    }

    bool GeodeticExtent::contains(const Geodetic2D& geodetic) const
    {
        double east  = m_east;
        double west  = m_west;

        double longitude = geodetic.get_longitude();
        double latitude  = geodetic.get_latitude();

        if (east < west)
        {
            east += TWO_PI;

            if (longitude < 0.0)
            {
                longitude += TWO_PI;
            }
        }

        return (longitude > west || ArgeoMath::equals_epsilon(longitude, west, EPSILON14)) &&
               (longitude < east || ArgeoMath::equals_epsilon(longitude, east, EPSILON14)) &&
               latitude >= m_south &&
               latitude <= m_north;
    }

	bool GeodeticExtent::equals(const GeodeticExtent& other) const
	{
		return m_west  == other.m_west 
			&& m_south == other.m_south
			&& m_east  == other.m_east
			&& m_north == other.m_north;
	}
}
