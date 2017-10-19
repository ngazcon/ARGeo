#pragma once

#include "VertexAttribute.h"
#include "Vector.h"

#include <string>

namespace argeo
{
	class VertexAttributeFloatVector2 : public VertexAttribute < vec2 >
	{
	public:

		VertexAttributeFloatVector2(std::string name);
		VertexAttributeFloatVector2(std::string name, int capacity);

		~VertexAttributeFloatVector2();
	};
}
