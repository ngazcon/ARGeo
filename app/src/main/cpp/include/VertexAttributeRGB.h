#pragma once

#include "VertexAttribute.h"
#include "Vector.h"
#include "Color.h"

#include <string>

namespace argeo
{
	class VertexAttributeRGB : public VertexAttribute < unsigned char >
	{
	public:

		VertexAttributeRGB(std::string name);
		VertexAttributeRGB(std::string name, int capacity);

		~VertexAttributeRGB();

		void add_color(Color color);
	};
}
