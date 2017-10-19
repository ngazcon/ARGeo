#include "EllipsoidTangentPlane.h"
#include "IntersectionAlgorithms.h"
#include "Transforms.h"

namespace argeo
{
	EllipsoidTangentPlane::EllipsoidTangentPlane(
		const Geodetic3D& origin,
		const Ellipsoid&  ellipsoid)
		: EllipsoidTangentPlane(
			ellipsoid.to_geocentric3D(origin),
			ellipsoid)
	{ }

	EllipsoidTangentPlane::EllipsoidTangentPlane(
		const Geocentric3D& origin,
		const Ellipsoid& ellipsoid)
		: m_ellipsoid(ellipsoid)
	{
		if (origin.get_x() == 0 && origin.get_y() == 0 && origin.get_z() == 0)
		{
			throw std::invalid_argument("origin must not be at the center of the ellipsoid.");
		}

		m_origin = origin;// ellipsoid.scale_to_geodetic_surface(origin);

		mat4d enu = Transforms::enu_to_fixed_frame(
			m_origin.to_vec3d(),
			ellipsoid);

		m_x_axis = Geocentric3D(
			enu.data[0],
			enu.data[1],
			enu.data[2]
		);

		m_y_axis = Geocentric3D(
			enu.data[4],
			enu.data[5],
			enu.data[6]
		);

		Geocentric3D normal(
			enu.data[8],
			enu.data[9],
			enu.data[10]
		);

		m_plane = Plane::from_point_normal(
			m_origin.to_vec3d(), 
			normal.to_vec3d());
	}

	EllipsoidTangentPlane::~EllipsoidTangentPlane()
	{ }

	Geocentric2D EllipsoidTangentPlane::project_point_onto_plane(const Geocentric3D& point)
	{
		Ray ray;
		ray.origin    = point.to_vec3d();
		ray.direction = point.normalize().to_vec3d();

		vec3d intersection_point = IntersectionAlgorithms::ray_plane(ray, m_plane);

		if (std::isnan(intersection_point.x) ||
			std::isnan(intersection_point.y) ||
			std::isnan(intersection_point.z))
		{
			ray.direction = -ray.direction;
			intersection_point = IntersectionAlgorithms::ray_plane(ray, m_plane);
		}

		if (!std::isnan(intersection_point.x) &&
			!std::isnan(intersection_point.y) &&
			!std::isnan(intersection_point.z))
		{
			Geocentric3D  v = intersection_point - m_origin;
			double x = m_x_axis.dot(v);
			double y = m_y_axis.dot(v);

			return Geocentric2D(x, y);
		}

		return Geocentric2D(std::numeric_limits<double>::quiet_NaN());

	}

	std::vector<Geocentric2D> EllipsoidTangentPlane::project_points_onto_plane(std::vector<Geocentric3D> points)
	{
		std::vector<Geocentric2D> result;
		
		for (int i = 0; i < points.size(); i++)
		{
			Geocentric2D intersection_point = project_point_onto_plane(points[i]);

			if (!std::isnan(intersection_point.get_x()) &&
				!std::isnan(intersection_point.get_y()))
			{
				result.push_back(intersection_point);
			}
		}

		return result;
	}

	Geocentric2D EllipsoidTangentPlane::project_point_to_nearest_onto_plane(const Geocentric3D& point)
	{
		Ray ray;
		ray.origin    = point.to_vec3d();
		ray.direction = m_plane.get_normal();

		vec3d intersection_point = IntersectionAlgorithms::ray_plane(ray, m_plane);

		if (std::isnan(intersection_point.x) ||
			std::isnan(intersection_point.y) ||
			std::isnan(intersection_point.z))
		{
			ray.direction      = -ray.direction;
			intersection_point = IntersectionAlgorithms::ray_plane(ray, m_plane);
		}

		Geocentric3D  v = intersection_point - m_origin;
		double x = m_x_axis.dot(v);
		double y = m_y_axis.dot(v);

		return Geocentric2D(x, y);
	}

	std::vector<Geocentric2D> EllipsoidTangentPlane::project_points_to_nearest_onto_plane(std::vector<Geocentric3D> points)
	{
		std::vector<Geocentric2D> result;

		for (int i = 0; i < points.size(); i++)
		{
			result.push_back(project_point_to_nearest_onto_plane(points[i]));
		}

		return result;
	}

	Plane EllipsoidTangentPlane::get_plane() const
	{
		return m_plane;
	}

	Geocentric3D EllipsoidTangentPlane::get_x_axis() const
	{
		return m_x_axis;
	}

	Geocentric3D EllipsoidTangentPlane::get_y_axis() const
	{
		return m_y_axis;
	}

	Geocentric3D EllipsoidTangentPlane::get_normal() const
	{
		return m_plane.get_normal();
	}

	Geocentric3D EllipsoidTangentPlane::get_origin() const
	{
		return m_origin;
	}

}	
