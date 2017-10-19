#pragma once

#include "IIndices.h"
#include "IndicesUnsignedInt.h"
#include "TriangleIndicesUnsignedInt.h"

#include <vector>

namespace argeo
{
	class IndicesUnsignedInt : public IIndices
	{
	public:

		IndicesUnsignedInt();

		IndicesUnsignedInt(int capacity);

		~IndicesUnsignedInt();

		std::vector<unsigned int> get_values() const;
		
		void add(unsigned int index);
		void add_triangle(TriangleIndicesUnsignedInt triangle);

	private:

		std::vector<unsigned int> m_values;
	};
}
