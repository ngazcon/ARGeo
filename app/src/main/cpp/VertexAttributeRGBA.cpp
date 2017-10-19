#include "VertexAttributeRGBA.h"

#include <limits>
#include <stdexcept>

namespace argeo
{
	VertexAttributeRGBA::VertexAttributeRGBA(std::string name)
		: VertexAttribute(name, VertexAttributeType::UnsignedByte)
	{ }

	VertexAttributeRGBA::VertexAttributeRGBA(std::string name, int capacity)
		: VertexAttribute(name, VertexAttributeType::UnsignedByte, capacity * 4)
	{
		if (capacity > std::numeric_limits<int>::max() / 4)
		{
			throw std::invalid_argument("capacity causes int overflow.");
		}
	}


	VertexAttributeRGBA::~VertexAttributeRGBA()
	{ }

	void VertexAttributeRGBA::add_color(Color color)
	{
		m_values.push_back(color.red);
		m_values.push_back(color.green);
		m_values.push_back(color.blue);
		m_values.push_back(color.alpha);
	}

}