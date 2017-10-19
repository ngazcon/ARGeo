#include "IIndices.h"

namespace argeo
{
	IIndices::IIndices(IndicesType type)
		: m_type(type)
	{ }

	IndicesType IIndices::get_type() const
	{
		return m_type;
	}
}
