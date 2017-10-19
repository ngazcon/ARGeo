#pragma once

#include "StencilTestFace.h"

namespace argeo
{
	class StencilTest
	{
	public:

		StencilTest(
			bool enabled               = false,
			StencilTestFace front_face = StencilTestFace(),
			StencilTestFace back_face  = StencilTestFace())
			: enabled(enabled)
			, front_face(front_face)
			, back_face(back_face)
		{ }

		bool enabled;
		StencilTestFace front_face;
		StencilTestFace back_face;
	};
}
