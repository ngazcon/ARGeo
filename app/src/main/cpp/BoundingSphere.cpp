#include "BoundingSphere.h"

#include <cmath>

namespace argeo
{
	BoundingSphere::BoundingSphere(
		const vec3d& center,
		const double& radius)
		: m_center(center)
		, m_radius(radius)
	{ }

	BoundingSphere::~BoundingSphere()
	{ }

	std::unique_ptr<BoundingSphere> BoundingSphere::from_corner_points(
		const vec3d& corner, 
		const vec3d& opposite_corner) 
	{
		vec3d  center = (corner + opposite_corner) * 0.5;
        double radius = vec_distance(center, opposite_corner);

        return std::unique_ptr<BoundingSphere>(new BoundingSphere(
			center, 
			radius));
    };

	std::unique_ptr<BoundingSphere> BoundingSphere::expand(const vec3d& point)
	{
		double radius = vec_length(point - m_center);

		return std::unique_ptr<BoundingSphere>(new BoundingSphere(
			m_center,
			radius > m_radius ? radius : m_radius));
	}

	std::unique_ptr<BoundingSphere> BoundingSphere::from_points(const std::vector<vec3d>& points)
	{
		if (points.empty()) 
		{
			return std::unique_ptr<BoundingSphere>(new BoundingSphere(
				vec3d::zero(),
				0.0));
		}

		vec3d currentPos = points[0];

		vec3d xMin = currentPos;
		vec3d yMin = currentPos;
		vec3d zMin = currentPos;				  
		vec3d xMax = currentPos;
		vec3d yMax = currentPos;
		vec3d zMax = currentPos;
		
		unsigned int numPositions = points.size();

		for (unsigned int i = 1; i < numPositions; i++) {

			currentPos = points[i];

			double x = currentPos.x;
			double y = currentPos.y;
			double z = currentPos.z;

			// Store points containing the the smallest and largest components
			if (x < xMin.x) 
			{
				xMin = currentPos;
			}

			if (x > xMax.x) 
			{
				xMax = currentPos;
			}

			if (y < yMin.y) 
			{
				currentPos = yMin;
			}

			if (y > yMax.y) 
			{
				currentPos = yMax;
			}

			if (z < zMin.z) 
			{
				zMin = currentPos;
			}

			if (z > zMax.z) 
			{
				zMax = currentPos;
			}
		}

		// Compute x-, y-, and z-spans (Squared distances b/n each component's min. and max.).
		double xSpan = vec_length_squared(xMax - xMin);
		double ySpan = vec_length_squared(yMax - yMin);
		double zSpan = vec_length_squared(zMax - zMin);


		// Set the diameter endpoints to the largest span.
		vec3d diameter1 = xMin;
		vec3d diameter2 = xMax;
		double maxSpan  = xSpan;

		if (ySpan > maxSpan) 
		{
			maxSpan = ySpan;
			diameter1 = yMin;
			diameter2 = yMax;
		}

		if (zSpan > maxSpan) 
		{
			maxSpan   = zSpan;
			diameter1 = zMin;
			diameter2 = zMax;
		}

		// Calculate the center of the initial sphere found by Ritter's algorithm
		vec3d ritterCenter(
			(diameter1.x + diameter2.x) * 0.5,
			(diameter1.y + diameter2.y) * 0.5,
			(diameter1.z + diameter2.z) * 0.5);


		// Calculate the radius of the initial sphere found by Ritter's algorithm
		double radiusSquared = vec_length_squared(diameter2 - ritterCenter);
		double ritterRadius  = std::sqrt(radiusSquared);


		// Find the center of the sphere found using the Naive method.
		vec3d minBoxPt(
			xMin.x,
			yMin.y,
			zMin.z);


		vec3d maxBoxPt(
			xMax.x,
			yMax.y,
			zMax.z);

		vec3d naiveCenter = (minBoxPt + maxBoxPt) * 0.5;

		// Begin 2nd pass to find naive radius and modify the ritter sphere.
		double naiveRadius = 0;
		for (unsigned int i = 0; i < numPositions; i++) 
		{
			currentPos = points[i];

			// Find the furthest point from the naive center to calculate the naive radius.
			double r = vec_length(currentPos - naiveCenter);

			if (r > naiveRadius)
			{
				naiveRadius = r;
			}

			// Make adjustments to the Ritter Sphere to include all points.
			double oldCenterToPointSquared = vec_length_squared(currentPos - ritterCenter);
			if (oldCenterToPointSquared > radiusSquared) 
			{
				double oldCenterToPoint = std::sqrt(oldCenterToPointSquared);
				
				// Calculate new radius to include the point that lies outside
				ritterRadius  = (ritterRadius + oldCenterToPoint) * 0.5;
				radiusSquared = ritterRadius * ritterRadius;

				// Calculate center of new Ritter sphere
				double oldToNew   = oldCenterToPoint - ritterRadius;
				ritterCenter.x = (ritterRadius * ritterCenter.x + oldToNew * currentPos.x) / oldCenterToPoint;
				ritterCenter.y = (ritterRadius * ritterCenter.y + oldToNew * currentPos.y) / oldCenterToPoint;
				ritterCenter.z = (ritterRadius * ritterCenter.z + oldToNew * currentPos.z) / oldCenterToPoint;
			}
		}

		if (ritterRadius < naiveRadius) 
		{
			return std::unique_ptr<BoundingSphere>(new BoundingSphere(
				ritterCenter,
				ritterRadius));
		}
		else 
		{
			return std::unique_ptr<BoundingSphere>(new BoundingSphere(
				naiveCenter,
				naiveRadius));
		}
	}

	std::unique_ptr<BoundingVolume> BoundingSphere::transform_without_scale(const mat4d& affine_matrix)
	{
		return std::unique_ptr<BoundingVolume>(new BoundingSphere(
			affine_matrix * m_center,
			m_radius));	
	}

	std::unique_ptr<BoundingVolume> BoundingSphere::transform(const mat4d& affine_matrix)
	{
		return std::unique_ptr<BoundingVolume>(new BoundingSphere(
			affine_matrix * m_center,
			mat_get_maximum_scale(affine_matrix) * m_radius
		));
	}

	Interval BoundingSphere::compute_plane_distances(
		const vec3d& position,
		const vec3d& direction)
	{
		vec3d to_center = m_center - position;
		double mag = vec_dot(direction, to_center);

		return Interval(
			mag - m_radius,
			mag + m_radius);
	}

	vec3d BoundingSphere::get_center() const
	{
		return m_center;
	}

	double BoundingSphere::get_radius() const
	{
		return m_radius;
	}

	void BoundingSphere::set_center(const vec3d& value)
	{
		m_center = value;
	}

	void BoundingSphere::set_radius(const double& value)
	{
		m_radius = value;
	}

	Intersect BoundingSphere::intersect(const Plane& plane) const
	{
		double distance_to_plane = vec_dot(plane.get_normal(), m_center) + plane.get_distance();

		if (distance_to_plane < -m_radius)
		{
			// The center point is negative side of the plane normal
			return Intersect::Outside;
		}
		else if (distance_to_plane < m_radius)
		{
			// The center point is positive side of the plane, but radius extends beyond it; partial overlap
			return Intersect::Intersect;;
		}

		return Intersect::Inside;
	}

    double BoundingSphere::distance_squared_to(const vec3d& point)
    {
        const  vec3d diff = m_center - point;
        return vec_length_squared(diff) - m_radius * m_radius;
    }

	bool BoundingSphere::equals(const BoundingSphere& other) const
	{
		return m_center == other.get_center() &&
			m_radius == other.get_radius();	
	}

	std::unique_ptr<BoundingSphere> BoundingSphere::from_rectangle_with_heights_2D(
			const GeodeticExtent& extent,
			MapProjection* projection,
			const double& minimum_height,
			const double& maximum_height)
    {
        Geodetic3D south_west = Geodetic3D(
                extent.get_south_west(),
                minimum_height);

        Geodetic3D north_east = Geodetic3D(
                extent.get_north_east(),
                maximum_height);

        vec3d lower_left  = projection->project(south_west).to_vec3d();
        vec3d upper_right = projection->project(north_east).to_vec3d();

        double width     = upper_right.x - lower_left.x;
        double height    = upper_right.y - lower_left.y;
        double elevation = upper_right.z - lower_left.z;

        double radius = ::sqrt(width * width + height * height + elevation * elevation) * 0.5;
        vec3d center(
            lower_left.x + width * 0.5,
            lower_left.y + height * 0.5,
            lower_left.z + elevation * 0.5);

        return std::unique_ptr<BoundingSphere>(new BoundingSphere(
                center,
                radius));
    }

	std::unique_ptr<BoundingSphere> BoundingSphere::from_rectangle_3D(
			const GeodeticExtent& extent,
			const Ellipsoid& ellipsoid,
			const double& surface_height)
    {
        return BoundingSphere::from_points(extent.subsample(ellipsoid, surface_height));
    }

    BoundingVolume* BoundingSphere::clone(BoundingVolume* result)
    {
        BoundingSphere* casted_result = dynamic_cast<BoundingSphere*>(result);

        if (casted_result == nullptr)
        {
            casted_result = new BoundingSphere();
        }

        casted_result->set_center(m_center);
        casted_result->set_radius(m_radius);

        return casted_result;
    }
}
