#pragma once

#include "VertexAttribute.h"
#include "Vector.h"
#include "Color.h"

#include <string>

namespace argeo
{
	class VertexAttributeRGBA : public VertexAttribute < unsigned char >
	{
	public:

		VertexAttributeRGBA(std::string name);
		VertexAttributeRGBA(std::string name, int capacity);

		~VertexAttributeRGBA();

		void add_color(Color color);
	};
}
