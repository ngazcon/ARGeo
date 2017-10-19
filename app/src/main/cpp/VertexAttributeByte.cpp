#include "VertexAttributeByte.h"

#include <limits>
#include <stdexcept>

namespace argeo
{
	VertexAttributeByte::VertexAttributeByte(std::string name)
		: VertexAttribute(name, VertexAttributeType::UnsignedByte)
	{ }

	VertexAttributeByte::VertexAttributeByte(std::string name, int capacity)
		: VertexAttribute(name, VertexAttributeType::UnsignedByte, capacity)
	{
	}

	VertexAttributeByte::~VertexAttributeByte()
	{ }
}