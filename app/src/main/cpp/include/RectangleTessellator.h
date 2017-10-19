#pragma once

#include "Mesh.h"
#include "RectangleD.h"

#include <memory>

namespace argeo
{
	class RectangleTessellator
	{
	public:

		static std::unique_ptr<Mesh> compute(
			RectangleD rectangle,
			int number_of_partitions_x, 
			int number_of_partitions_y);

	};
}

