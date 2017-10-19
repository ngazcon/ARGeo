#include "Plane.h"

namespace argeo
{
	Plane::Plane(
		const vec4d& normal_and_distance)
		: m_normal(to_vec3d(normal_and_distance))
		, m_distance(normal_and_distance.w)
	{ }

	Plane::Plane(
		const vec3d& normal,
		const double& distance)
		: m_normal(normal)
		, m_distance(distance)
	{ }


	double Plane::get_point_distance(const vec3d& point) const
	{
		return vec_dot(m_normal, point) + m_distance;
	}

	double Plane::get_distance() const
	{
		return m_distance;
	}

	vec3d Plane::get_normal() const
	{
		return m_normal;
	}

	Plane Plane::from_point_normal(
		const vec3d& point,
		const vec3d& normal)
	{
		return Plane(
			normal,
		    -vec_dot(normal, point));
	}
}
