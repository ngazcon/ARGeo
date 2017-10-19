#include "CullingVolume.h"


namespace argeo
{
	const std::vector<vec3d> CullingVolume::g_faces =
	{
		vec3d::unit_x(),
		vec3d::unit_y(),
		vec3d::unit_z()
	};

	CullingVolume::CullingVolume(std::vector<vec4d> planes)
		: m_planes(planes)
	{ }


	CullingVolume::~CullingVolume()
	{ }

	std::unique_ptr<CullingVolume> CullingVolume::from_bounding_sphere(BoundingSphere* bounding_sphere)
	{
		unsigned int length = g_faces.size();

		std::vector<vec4d> planes(length * 2);

		vec3d center  = bounding_sphere->get_center();
		double radius = bounding_sphere->get_radius();

		unsigned int plane_index = 0;
		vec3d plane_center;

		for (unsigned int i = 0; i < length; ++i) 
		{
			vec3d face_normal = g_faces[i];

			vec4d& plane0 = planes[plane_index];
			vec4d& plane1 = planes[plane_index + 1];

			plane_center = face_normal * (-radius) + center;

			plane0.x = face_normal.x;
			plane0.y = face_normal.y;
			plane0.z = face_normal.z;
			plane0.w = vec_dot(face_normal, plane_center);

			plane_center = face_normal * radius + center;
			
			plane1.x = -face_normal.x;
			plane1.y = -face_normal.y;
			plane1.z = -face_normal.z;
			plane1.w = -vec_dot(-face_normal, plane_center);

			plane_index += 2;
		}

		return std::unique_ptr<CullingVolume>(new CullingVolume(planes));
	}

	Intersect CullingVolume::visibility(BoundingVolume* bounding_volume)
	{
		bool intersecting = false;
		for (vec4d plane : m_planes) 
		{
			Intersect result = bounding_volume->intersect(Plane(to_vec3d(plane), plane.w));
			if (result == Intersect::Outside)
			{
				return Intersect::Outside;
			}
			else if (result == Intersect::Intersect) 
			{
				intersecting = true;
			}
		}
		return intersecting ? Intersect::Intersect : Intersect::Inside;
	}
}