#pragma once

#include "VertexAttribute.h"
#include "Vector.h"

#include <string>

namespace argeo
{
	class VertexAttributeFloatVector3 : public VertexAttribute<vec3>
	{
	public:

		VertexAttributeFloatVector3(std::string name);
		VertexAttributeFloatVector3(std::string name, int capacity);

		~VertexAttributeFloatVector3();
	};
}
