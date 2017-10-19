#pragma once

#include "IndicesType.h"

namespace argeo
{
	class IIndices
	{
	protected:
		IIndices(IndicesType type);

	public:
		IndicesType get_type() const;

	private:
		IndicesType m_type;

	};
}
