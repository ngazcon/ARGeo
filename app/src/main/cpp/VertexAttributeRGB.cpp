#include "VertexAttributeRGB.h"

#include <limits>
#include <stdexcept>

namespace argeo
{
	VertexAttributeRGB::VertexAttributeRGB(std::string name)
		: VertexAttribute(name, VertexAttributeType::UnsignedByte)
	{ }

	VertexAttributeRGB::VertexAttributeRGB(std::string name, int capacity)
		: VertexAttribute(name, VertexAttributeType::UnsignedByte, capacity * 3)
	{
		if (capacity > std::numeric_limits<int>::max() / 3)
		{
			throw std::invalid_argument("capacity causes int overflow.");
		}
	}


	VertexAttributeRGB::~VertexAttributeRGB()
	{ }

	void VertexAttributeRGB::add_color(Color color)
	{
		m_values.push_back(color.red);
		m_values.push_back(color.green);
		m_values.push_back(color.blue);
	}

}