#pragma once

#include "IndexDataType.h"
#include "ComponentDataType.h"
#include <stdexcept>

namespace argeo
{
	class VertexArraySizes
	{
	public:

		static int size_of(IndexDataType type)
		{
			switch (type)
			{
			case IndexDataType::UnsignedShort:
				return sizeof(unsigned short);
			case IndexDataType::UnsignedInteger:
				return sizeof(unsigned int);
			}

			throw std::invalid_argument("type");
		}

		static int size_of(ComponentDataType type)
		{
			switch (type)
			{
			case ComponentDataType::Byte:
			case ComponentDataType::UnsignedByte:
				return sizeof(unsigned char);
			case ComponentDataType::Short:
				return sizeof(short);
			case ComponentDataType::UnsignedShort:
				return sizeof(unsigned short);
			case ComponentDataType::Int:
				return sizeof(int);
			case ComponentDataType::UnsignedInt:
				return sizeof(unsigned int);
			case ComponentDataType::Float:
				return sizeof(float);
			}

			throw std::invalid_argument("type");
		}
	};
}
