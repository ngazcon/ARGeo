#pragma once

#include "IIndices.h"
#include "IndicesType.h"
#include "TriangleIndicesUnsignedShort.h"

#include <vector>

namespace argeo
{
	class IndicesUnsignedShort : public IIndices
	{
	public:

		IndicesUnsignedShort();

		IndicesUnsignedShort(int capacity);

		~IndicesUnsignedShort();

		void add_index(unsigned short index);

		std::vector<unsigned short> get_values() const;
		void add_triangle(TriangleIndicesUnsignedShort triangle);

	private:

		std::vector<unsigned short> m_values;
	};
}