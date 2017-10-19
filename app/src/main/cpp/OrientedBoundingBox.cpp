#include "OrientedBoundingBox.h"

#include "EllipsoidTangentPlane.h"

#include <cmath>

namespace argeo
{
	OrientedBoundingBox::OrientedBoundingBox(
		const vec3d& center,
		const mat3d& half_axes) 
		: m_center(center)
		, m_half_axes(half_axes)
	{ }


	OrientedBoundingBox::~OrientedBoundingBox()
	{ }

	Intersect OrientedBoundingBox::intersect(const Plane& plane) const
	{
		vec3d normal    = plane.get_normal();

		double normal_x = normal.x;
		double normal_y = normal.y;
		double normal_z = normal.z;

		// plane is used as if it is its normal; the first three components are assumed to be normalized
		double rad_effective = 
			fabs(normal_x * m_half_axes.data[0] + normal_y * m_half_axes.data[1] + normal_z * m_half_axes.data[2]) +
			fabs(normal_x * m_half_axes.data[3] + normal_y * m_half_axes.data[4] + normal_z * m_half_axes.data[5]) +
			fabs(normal_x * m_half_axes.data[6] + normal_y * m_half_axes.data[7] + normal_z * m_half_axes.data[8]);

		double distance_to_plane = vec_dot(normal, m_center) + plane.get_distance();

		if (distance_to_plane <= -rad_effective)
		{
			// The entire box is on the negative side of the plane normal
			return Intersect::Outside;
		}
		else if (distance_to_plane >= rad_effective)
		{
			// The entire box is on the positive side of the plane normal
			return Intersect::Inside;
		}

		return Intersect::Intersect;
	}

	std::unique_ptr<OrientedBoundingBox> OrientedBoundingBox::from_rectangle(
		const GeodeticExtent& extent,
		const double& min_height,
		const double& max_height,
		const Ellipsoid& ellipsoid)
	{
		if (extent.get_width() < 0.0 || extent.get_height() > PI)
		{
			throw std::invalid_argument(
				"Rectangle width must be between 0 and pi");
		}

		if (extent.get_height() < 0.0 || extent.get_height() > PI)
		{
			throw std::invalid_argument(
				"Rectangle height must be between 0 and pi");
		}

		if (!ArgeoMath::equals_epsilon(ellipsoid.get_radii().x, ellipsoid.get_radii().y, EPSILON15))
		{
			throw std::invalid_argument(
				"Ellipsoid must be an ellipsoid of revolution (radii.x == radii.y)");
		}

		// The bounding box will be aligned with the tangent plane at the center of the rectangle.
		Geodetic2D tangent_point_cartographic = extent.get_center();
		Geocentric3D tangent_point            = ellipsoid.to_geocentric3D(tangent_point_cartographic);

		EllipsoidTangentPlane tangent_plane(tangent_point, ellipsoid);

		Plane plane = tangent_plane.get_plane();

		// Corner arrangement:
		//          N/+y
		//      [0] [1] [2]
		// W/-x [7]     [3] E/+x
		//      [6] [5] [4]
		//          S/-y
		// "C" refers to the central lat/long, which by default aligns with the tangent point (above).
		// If the rectangle spans the equator, CW and CE are instead aligned with the equator.

		double lon_center = tangent_point_cartographic.get_longitude();
		double lat_center = (extent.get_south() < 0.0 && extent.get_north() > 0.0) ? 0.0 : tangent_point_cartographic.get_latitude();

		// Compute XY extents using the rectangle at maximum height
		std::vector<Geocentric3D> geocentric_points = ellipsoid.to_geocentric3D({
			// perimeterNW
			Geodetic3D(
				extent.get_west(),
				extent.get_north(),
				max_height),

			// perimeterNC
			Geodetic3D(
				lon_center,
				extent.get_north(),
				max_height),

			// perimeterNE
			Geodetic3D(
				extent.get_east(),
				extent.get_north(),
				max_height),

			// perimeterCE
			Geodetic3D(
				extent.get_east(),
				lat_center,
				max_height),

			// perimeterSE
			Geodetic3D(
				extent.get_east(),
				extent.get_south(),
				max_height),

			// perimeterSC
			Geodetic3D(
				lon_center,
				extent.get_south(),
				max_height),

			// perimeterSW
			Geodetic3D(
				extent.get_west(),
				extent.get_south(),
				max_height),

			// perimeterCW
			Geodetic3D(
				extent.get_west(),
				lat_center,
				max_height),

		});

		std::vector<Geocentric2D> projected_points  = tangent_plane.project_points_to_nearest_onto_plane(geocentric_points);

		double min_x = fmin(fmin(projected_points[6].get_x(), projected_points[7].get_x()), projected_points[0].get_x());
		double max_x = fmax(fmax(projected_points[2].get_x(), projected_points[3].get_x()), projected_points[4].get_x());
		double min_y = fmin(fmin(projected_points[4].get_y(), projected_points[5].get_y()), projected_points[6].get_y());
		double max_y = fmax(fmax(projected_points[0].get_y(), projected_points[1].get_y()), projected_points[2].get_y());

		// Compute minimum Z using the rectangle at minimum height
		geocentric_points = ellipsoid.to_geocentric3D({
			// perimeterNW
			Geodetic3D(
				extent.get_west(),
				extent.get_north(),
				min_height),

			// perimeterNE
			Geodetic3D(
				extent.get_east(),
				extent.get_north(),
				min_height),

			// perimeterSE
			Geodetic3D(
				extent.get_east(),
				extent.get_south(),
				min_height),

			// perimeterSW
			Geodetic3D(
				extent.get_west(),
				extent.get_south(),
				min_height),
		});

		double min_z = fmin(plane.get_point_distance(geocentric_points[0].to_vec3d()), plane.get_point_distance(geocentric_points[1].to_vec3d()));
		min_z		 = fmin(min_z, plane.get_point_distance(geocentric_points[2].to_vec3d()));
		min_z		 = fmin(min_z, plane.get_point_distance(geocentric_points[3].to_vec3d()));

		// Since the tangent plane touches the surface at height = 0, this is okay
		double max_z = max_height;

		return from_tangent_plane_extents(
			tangent_plane,
			min_x,
			max_x, 
			min_y,
			max_y, 
			min_z,
			max_z);
	}

	
	std::unique_ptr<OrientedBoundingBox> OrientedBoundingBox::from_tangent_plane_extents(
		EllipsoidTangentPlane tangent_plane,
		const double& minimum_x,
		const double& maximum_x,
		const double& minimum_y,
		const double& maximum_y,
		const double& minimum_z,
		const double& maximum_z)
	{
		vec3d x_axis = tangent_plane.get_x_axis().to_vec3d();
		vec3d y_axis = tangent_plane.get_y_axis().to_vec3d();
		vec3d z_axis = tangent_plane.get_normal().to_vec3d();

		mat3d half_axes(
			x_axis.x, x_axis.y, x_axis.z,
			y_axis.x, y_axis.y, y_axis.z,
			z_axis.x, z_axis.y, z_axis.z);

		vec3d center_offset(
			(minimum_x + maximum_x) / 2.0,
			(minimum_y + maximum_y) / 2.0,
			(minimum_z + maximum_z) / 2.0);

		vec3d scale(
			(maximum_x - minimum_x) / 2.0,
			(maximum_y - minimum_y) / 2.0,
			(maximum_z - minimum_z) / 2.0);

		vec3d center = half_axes * center_offset + tangent_plane.get_origin().to_vec3d();
		half_axes    = mat_multiply_by_scale(half_axes, scale);

		return std::unique_ptr<OrientedBoundingBox>(new OrientedBoundingBox(
			center,
			half_axes));
	}

	std::unique_ptr<BoundingVolume> OrientedBoundingBox::transform(const mat4d& affine_matrix)
	{
		return nullptr;
	}

	Interval OrientedBoundingBox::compute_plane_distances(
		const vec3d& position,
		const vec3d& direction)
	{
		double min_dist =  std::numeric_limits<double>::max();
		double max_dist = -std::numeric_limits<double>::max();

		vec3d u = mat_get_column(m_half_axes, 0);
		vec3d v = mat_get_column(m_half_axes, 1);
		vec3d w = mat_get_column(m_half_axes, 2);

		// project first corner
		vec3d corner = m_center + u + v + w;
		vec3d to_center = corner - position;
		
		double mag = vec_dot(direction, to_center);

		min_dist = ::fmin(mag, min_dist);
		max_dist = ::fmax(mag, max_dist);

		// project second corner
		corner = m_center + u + v - w;
		to_center = corner - position;

		mag = vec_dot(direction, to_center);

		min_dist = ::fmin(mag, min_dist);
		max_dist = ::fmax(mag, max_dist);

		// project third corner
		corner = m_center + u - v + w;
		to_center = corner - position;
		mag = vec_dot(direction, to_center);

		min_dist = ::fmin(mag, min_dist);
		max_dist = ::fmax(mag, max_dist);

		// project fourth corner
		corner = m_center + u - v - w;
		to_center = corner - position;
		mag = vec_dot(direction, to_center);

		min_dist = ::fmin(mag, min_dist);
		max_dist = ::fmax(mag, max_dist);

		// project fifth corner
		corner = m_center - u + v + w;
		to_center = corner - position;
		mag = vec_dot(direction, to_center);

		min_dist = ::fmin(mag, min_dist);
		max_dist = ::fmax(mag, max_dist);

		// project sixth corner
		corner = m_center - u + v - w;
		to_center = corner - position;
		mag = vec_dot(direction, to_center);

		min_dist = ::fmin(mag, min_dist);
		max_dist = ::fmax(mag, max_dist);

		// project seventh corner
		corner = m_center - u - v + w;
		to_center = corner - position;
		mag = vec_dot(direction, to_center);

		min_dist = ::fmin(mag, min_dist);
		max_dist = ::fmax(mag, max_dist);

		// project eighth corner
		corner = m_center - u - v - w;
		to_center = corner - position;
		mag = vec_dot(direction, to_center);

		min_dist = ::fmin(mag, min_dist);
		max_dist = ::fmax(mag, max_dist);

		return Interval(
			min_dist,
			max_dist);

	}

    void OrientedBoundingBox::set_center(const vec3d& value)
    {
        m_center = value;
    }

    void OrientedBoundingBox::set_half_axes(const mat3d& value)
    {
        m_half_axes = value;
    }

	BoundingVolume* OrientedBoundingBox::clone(BoundingVolume* result)
	{
        OrientedBoundingBox* casted_result = dynamic_cast<OrientedBoundingBox*>(result);

		if (casted_result == nullptr)
		{
			casted_result = new OrientedBoundingBox();
		}

		casted_result->set_center(m_center);
		casted_result->set_half_axes(m_half_axes);

		return casted_result;
	}

}