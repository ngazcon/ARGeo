#include "IndicesUnsignedShort.h"


namespace argeo
{
	IndicesUnsignedShort::IndicesUnsignedShort()
		: IIndices(IndicesType::UnsignedShort)
	{ }

	IndicesUnsignedShort::IndicesUnsignedShort(int capacity)
		: IIndices(IndicesType::UnsignedShort)
	{
		m_values.reserve(capacity);
	}

	IndicesUnsignedShort::~IndicesUnsignedShort()
	{ }

	void IndicesUnsignedShort::add_index(unsigned short index)
	{
		m_values.push_back(index);
	}

	std::vector<unsigned short> IndicesUnsignedShort::get_values() const
	{
		return m_values;
	}

	void IndicesUnsignedShort::add_triangle(TriangleIndicesUnsignedShort triangle)
	{
		m_values.push_back(triangle.ui0);
		m_values.push_back(triangle.ui1);
		m_values.push_back(triangle.ui2);
	}
}