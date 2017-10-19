#pragma once

#include "VertexAttribute.h"
#include "Vector.h"
#include "Color.h"

#include <string>

namespace argeo
{
	class VertexAttributeByte : public VertexAttribute < unsigned char >
	{
	public:

		VertexAttributeByte(std::string name);
		VertexAttributeByte(std::string name, int capacity);

		~VertexAttributeByte();
	};
}
