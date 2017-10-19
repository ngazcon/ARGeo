#pragma once

#include "VertexAttribute.h"
#include "Vector.h"

#include <string>

namespace argeo
{
	class VertexAttributeDoubleVector3 : public VertexAttribute<vec3d>
	{
	public:

		VertexAttributeDoubleVector3(std::string name);
		VertexAttributeDoubleVector3(std::string name, int capacity);

		~VertexAttributeDoubleVector3();
	};
}
