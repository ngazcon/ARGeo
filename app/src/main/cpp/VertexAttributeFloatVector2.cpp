#include "VertexAttributeFloatVector2.h"

namespace argeo
{
	VertexAttributeFloatVector2::VertexAttributeFloatVector2(std::string name)
		: VertexAttribute(name, VertexAttributeType::FloatVector2)
	{ }

	VertexAttributeFloatVector2::VertexAttributeFloatVector2(std::string name, int capacity)
		: VertexAttribute(name, VertexAttributeType::FloatVector2, capacity)
	{ }

	VertexAttributeFloatVector2::~VertexAttributeFloatVector2()
	{ }
}