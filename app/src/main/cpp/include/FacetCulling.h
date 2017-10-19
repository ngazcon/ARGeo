#pragma once

#include "WindingOrder.h"
#include "CullFace.h"

namespace argeo
{
	class FacetCulling
	{
	public:
		FacetCulling(
			bool enabled	 = false,
			CullFace face    = CullFace::Back,
			WindingOrder front_face_winding_order = WindingOrder::CounterClockwise)
			: enabled(enabled)
			, face(face)
			, front_face_winding_order(front_face_winding_order)
		{ }

		bool enabled;
		CullFace face;
		WindingOrder front_face_winding_order;
	};
}
