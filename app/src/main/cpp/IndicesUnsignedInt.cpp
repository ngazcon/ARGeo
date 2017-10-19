#include "IndicesUnsignedInt.h"

namespace argeo
{
	IndicesUnsignedInt::IndicesUnsignedInt()
		: IIndices(IndicesType::UnsignedInt)
	{ }

	IndicesUnsignedInt::IndicesUnsignedInt(int capacity)
		: IIndices(IndicesType::UnsignedInt)
	{
		m_values.reserve(capacity);
	}

	IndicesUnsignedInt::~IndicesUnsignedInt()
	{ }

	std::vector<unsigned int> IndicesUnsignedInt::get_values() const
	{
		return m_values;
	}

	void IndicesUnsignedInt::add(unsigned int index)
	{
		m_values.push_back(index);
	}

	void IndicesUnsignedInt::add_triangle(TriangleIndicesUnsignedInt triangle)
	{
		m_values.push_back(triangle.ui0);
		m_values.push_back(triangle.ui1);
		m_values.push_back(triangle.ui2);
	}
	
}
