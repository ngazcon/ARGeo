#include "VertexAttributeDoubleVector3.h"

namespace argeo
{
	VertexAttributeDoubleVector3::VertexAttributeDoubleVector3(std::string name)
		: VertexAttribute(name, VertexAttributeType::EmulatedDoubleVector3)
	{ }

	VertexAttributeDoubleVector3::VertexAttributeDoubleVector3(std::string name, int capacity)
		: VertexAttribute(name, VertexAttributeType::EmulatedDoubleVector3, capacity)
	{ }

	VertexAttributeDoubleVector3::~VertexAttributeDoubleVector3()
	{ }
}