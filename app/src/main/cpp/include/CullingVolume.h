#pragma once

#include "BoundingVolume.h"
#include "BoundingSphere.h"
#include "Plane.h"

#include <vector>

namespace argeo
{
	class CullingVolume
	{
		///
		/// Represent a volume used to apply frustum culling.
		///

	public:

		///
		/// The culling volume defined by planes.
		/// @constructor
		/// @param {std::vector<Plane>} planes A list of clipping planes.
		/// 
		CullingVolume(std::vector<vec4d> planes);

		///
		/// @destructor
		///
		~CullingVolume();


		static std::unique_ptr<CullingVolume> from_bounding_sphere(BoundingSphere* bounding_sphere);

		///
		/// Determines whether a bounding volume intersects the culling volume.
		/// @param {BoundingVolume*} boundingVolume The bounding volume whose intersection with the culling volume is to be tested.
		/// @returns {Intersect}  Intersect::Outside, Intersect.Intersecting, or Intersect.Inside.
		///
		Intersect visibility(BoundingVolume* bounding_volume);

		
	private:
		static const std::vector<vec3d> g_faces;

		std::vector<vec4d> m_planes;
	};
}