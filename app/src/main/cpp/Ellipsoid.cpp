#include "Ellipsoid.h"

#include <stdexcept>
#include <cmath>

namespace argeo
{
	const vec3d Ellipsoid::WGS84_RADII = vec3d(6378137.0, 6378137.0, 6356752.314245);
	
	Ellipsoid::Ellipsoid(
		double x, 
		double y, 
		double z)
		: Ellipsoid(vec3d(x, y, z))
	{ }

	Ellipsoid::Ellipsoid(const vec3d& radii)
		: m_radii(radii)
		, m_radii_squared(
			radii.x * radii.x,
			radii.y * radii.y,
			radii.z * radii.z)
		, m_radii_to_the_fourth(
			m_radii_squared.x * m_radii_squared.x,
			m_radii_squared.y * m_radii_squared.y,
			m_radii_squared.z * m_radii_squared.z)
        , m_one_over_radii(
            1.0 / (radii.x),
            1.0 / (radii.y),
            1.0 / (radii.z))
		, m_one_over_radii_squared(
			1.0 / (radii.x * radii.x),
			1.0 / (radii.y * radii.y),
			1.0 / (radii.z * radii.z))
        , m_center_tolerance_squared(EPSILON1)
        , m_squared_x_over_squared_z(m_radii_squared.x / m_radii_squared.z)
	{
		if ((radii.x <= 0.0) || (radii.y <= 0.0) || (radii.z <= 0.0))
		{
			throw std::out_of_range("radii");
		}
	}


	Ellipsoid::~Ellipsoid()
	{ }

	const vec3d Ellipsoid::get_radii() const
	{
		return m_radii;
	}

	const vec3d Ellipsoid::get_radii_squared() const
	{
		return m_radii_squared;
	}

	const vec3d Ellipsoid::get_one_over_radii_squared() const
	{
		return m_one_over_radii_squared;
	}

	double Ellipsoid::get_minimum_radius() const
	{
		return fmin(m_radii.x, fmin(m_radii.y, m_radii.z));
	}

	double Ellipsoid::get_maximum_radius() const
	{
		return fmax(m_radii.x, fmax(m_radii.y, m_radii.z));
	}

	Ellipsoid Ellipsoid::wgs84()
	{
		return Ellipsoid(WGS84_RADII);
	}

	Ellipsoid Ellipsoid::scaled_wgs84()
	{
		return Ellipsoid(1.0, 1.0, WGS84_RADII.x / WGS84_RADII.z);
	}

	Ellipsoid Ellipsoid::unit_sphere()
	{
		return Ellipsoid(1.0, 1.0, 1.0);
	}

	Geocentric3D Ellipsoid::geodetic_surface_normal(const Geocentric3D& geocentric) const
	{
		return (geocentric * m_one_over_radii_squared).normalize();
	}

	Geocentric3D Ellipsoid::geodetic_surface_normal(const Geodetic3D& geodetic) const
	{
		double cos_latitude = std::cos(geodetic.get_latitude());

		return Geocentric3D(
			cos_latitude * std::cos(geodetic.get_longitude()),
			cos_latitude * std::sin(geodetic.get_longitude()),
			std::sin(geodetic.get_latitude())).normalize();
	}

	std::vector<Geocentric3D> Ellipsoid::to_geocentric3D(std::vector<Geodetic3D> geodetics) const
	{
		std::vector<Geocentric3D> positions_on_ellipsoid;

		for (Geodetic3D geodetic : geodetics)
		{
			positions_on_ellipsoid.push_back(to_geocentric3D(geodetic));
		}

		return positions_on_ellipsoid;
	}

	Geocentric3D Ellipsoid::to_geocentric3D(const Geodetic2D& geodetic) const
	{
		return to_geocentric3D(Geodetic3D(
				geodetic.get_longitude(),
				geodetic.get_latitude(),
				0.0));
	}

	Geocentric3D Ellipsoid::to_geocentric3D(const Geodetic3D& geodetic) const
	{
		Geocentric3D n = geodetic_surface_normal(geodetic);
        Geocentric3D k = m_radii_squared * n;
		double gamma           = std::sqrt(n.dot(k));
		Geocentric3D r_surface = k / gamma;
		return r_surface + (n * geodetic.get_height());
	}

	Geodetic2D Ellipsoid::to_geodetic2D(const Geocentric3D& geocentric) const
	{
		Geocentric3D p = scale_to_geodetic_surface(geocentric);
		Geocentric3D n = geodetic_surface_normal(p);

		return Geodetic2D(
			std::atan2(n.get_y(), n.get_x()),
			std::asin(n.get_z()));
	}

	Geodetic3D Ellipsoid::to_geodetic3D(const Geocentric3D& geocentric) const
	{
		Geocentric3D p = scale_to_geodetic_surface(geocentric);

        if (undefined(p))
        {
            return Geodetic3D::undefined();
        }

		Geocentric3D n = geodetic_surface_normal(p);
		Geocentric3D h = geocentric - p;

		double height = ArgeoMath::sign(h.dot(geocentric)) * h.length();

		return Geodetic3D(
			std::atan2(n.get_y(), n.get_x()),
			std::asin(n.get_z()),
			height);
	}

	Geocentric3D Ellipsoid::scale_to_geodetic_surface(const Geodetic3D& geodetic) const
	{
		double longitude = geodetic.get_longitude();
		double latitude  = geodetic.get_latitude();

		double cos_lat   = std::cos(latitude);

		return Geocentric3D(
			cos_lat * std::cos(longitude),
			cos_lat * std::sin(longitude),
			std::sin(latitude)).normalize();
	}

	Geocentric3D Ellipsoid::scale_to_geodetic_surface(const Geocentric3D& position) const
	{
		const double position_x = position.get_x();
		const double position_y = position.get_y();
		const double position_z = position.get_z();

		const double one_over_radii_x = m_one_over_radii.x;
		const double one_over_radii_y = m_one_over_radii.y;
		const double one_over_radii_z = m_one_over_radii.z;

		const double x2 = position_x * position_x * one_over_radii_x * one_over_radii_x;
		const double y2 = position_y * position_y * one_over_radii_y * one_over_radii_y;
		const double z2 = position_z * position_z * one_over_radii_z * one_over_radii_z;

		// Compute the squared ellipsoid norm.
        const double squared_norm = x2 + y2 + z2;
		const double ratio = ::sqrt(1.0 / squared_norm);

		// As an initial approximation, assume that the radial intersection is the projection point.
		Geocentric3D intersection = position * ratio;

		// If the position is near the center, the iteration will not converge.
		if (squared_norm < m_center_tolerance_squared)
        {
			return !std::isinf(ratio) ? Geocentric3D::undefined() : intersection;
		}

		const double one_over_radii_squared_x = m_one_over_radii_squared.x;
		const double one_over_radii_squared_y = m_one_over_radii_squared.y;
		const double one_over_radii_squared_z = m_one_over_radii_squared.z;

		// Use the gradient at the intersection point in place of the true unit normal.
		// The difference in magnitude will be absorbed in the multiplier.
		vec3d gradient(
		    intersection.get_x() * one_over_radii_squared_x * 2.0,
		    intersection.get_y() * one_over_radii_squared_y * 2.0,
		    intersection.get_z() * one_over_radii_squared_z * 2.0);

		// Compute the initial guess at the normal vector multiplier, lambda.
		double lambda = (1.0 - ratio) * position.length() / (0.5 * vec_length(gradient));
		double correction = 0.0;

		double func;
		double denominator;
		double x_multiplier;
		double y_multiplier;
		double z_multiplier;
		double x_multiplier_2;
		double y_multiplier_2;
		double z_multiplier_2;
		double x_multiplier_3;
		double y_multiplier_3;
		double z_multiplier_3;

		do
		{
			lambda -= correction;

			x_multiplier = 1.0 / (1.0 + lambda * one_over_radii_squared_x);
			y_multiplier = 1.0 / (1.0 + lambda * one_over_radii_squared_y);
			z_multiplier = 1.0 / (1.0 + lambda * one_over_radii_squared_z);

			x_multiplier_2 = x_multiplier * x_multiplier;
			y_multiplier_2 = y_multiplier * y_multiplier;
			z_multiplier_2 = z_multiplier * z_multiplier;

			x_multiplier_3 = x_multiplier_2 * x_multiplier;
			y_multiplier_3 = y_multiplier_2 * y_multiplier;
			z_multiplier_3 = z_multiplier_2 * z_multiplier;

			func = x2 * x_multiplier_2 + y2 * y_multiplier_2 + z2 * z_multiplier_2 - 1.0;

			// "denominator" here refers to the use of this expression in the velocity and acceleration
			// computations in the sections to follow.
			denominator =
                    x2 * x_multiplier_3 * one_over_radii_squared_x +
                    y2 * y_multiplier_3 * one_over_radii_squared_y +
                    z2 * z_multiplier_3 * one_over_radii_squared_z;

			const double derivative = -2.0 * denominator;
			correction = func / derivative;

		}
		while (std::abs(func) > EPSILON12);

		return Geocentric3D(
                position_x * x_multiplier,
                position_y * y_multiplier,
                position_z * z_multiplier);
	}

	Geocentric3D Ellipsoid::scale_to_geocentric_surface(const Geocentric3D& position) const
	{
		double beta = 1.0 / sqrt(
			(position.get_x() * position.get_x()) * m_one_over_radii_squared.x +
			(position.get_y() * position.get_y()) * m_one_over_radii_squared.y +
			(position.get_z() * position.get_z()) * m_one_over_radii_squared.z);

		return position * beta;
	}

    Geocentric3D Ellipsoid::get_surface_normal_intersection_with_z_axis(
            const Geocentric3D &position,
            const double &buffer)
    {
        Geocentric3D result(
            0.0,
            0.0,
            position.get_z() * (1 - m_squared_x_over_squared_z));

        if (std::abs(result.get_z()) >= m_radii.z - buffer)
        {
            return Geocentric3D::undefined();
        }

        return result;
    }
}