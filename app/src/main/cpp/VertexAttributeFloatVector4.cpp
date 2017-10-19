#include "VertexAttributeFloatVector4.h"

namespace argeo
{
	VertexAttributeFloatVector4::VertexAttributeFloatVector4(std::string name)
		: VertexAttribute(name, VertexAttributeType::FloatVector4)
	{ }

	VertexAttributeFloatVector4::VertexAttributeFloatVector4(std::string name, int capacity)
		: VertexAttribute(name, VertexAttributeType::FloatVector4, capacity)
	{ }

	VertexAttributeFloatVector4::~VertexAttributeFloatVector4()
	{ }
}