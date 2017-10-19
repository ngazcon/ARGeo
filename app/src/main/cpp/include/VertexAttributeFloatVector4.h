#pragma once

#include "VertexAttribute.h"
#include "Vector.h"

namespace argeo
{
	class VertexAttributeFloatVector4 : public VertexAttribute < vec4 >
	{
	public:

		VertexAttributeFloatVector4(std::string name);
		VertexAttributeFloatVector4(std::string name, int capacity);

		~VertexAttributeFloatVector4();
	};
}