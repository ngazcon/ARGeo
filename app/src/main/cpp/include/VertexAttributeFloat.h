#pragma once

#include "VertexAttributeType.h"
#include "VertexAttribute.h"

namespace argeo
{
	class VertexAttributeFloat : public VertexAttribute < float >
	{
	public:

		VertexAttributeFloat(std::string name)
			: VertexAttribute<float>(name, VertexAttributeType::Float)
		{
		}

		VertexAttributeFloat(std::string name, int capacity)
			: VertexAttribute<float>(name, VertexAttributeType::Float, capacity)
		{
		}

		~VertexAttributeFloat()
		{
		}
	};
}
