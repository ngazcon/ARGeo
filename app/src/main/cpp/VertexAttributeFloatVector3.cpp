#include "VertexAttributeFloatVector3.h"

namespace argeo
{
	VertexAttributeFloatVector3::VertexAttributeFloatVector3(std::string name)
		: VertexAttribute(name, VertexAttributeType::FloatVector3)
	{ }

	VertexAttributeFloatVector3::VertexAttributeFloatVector3(std::string name, int capacity)
		: VertexAttribute(name, VertexAttributeType::FloatVector3, capacity)
	{ }

	VertexAttributeFloatVector3::~VertexAttributeFloatVector3()
	{ }
}