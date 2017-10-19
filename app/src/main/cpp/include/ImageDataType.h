#pragma once

namespace argeo
{
	enum class ImageDataType
	{
		Byte,
		UnsignedByte,
		Short,
		UnsignedShort,
		Int,
		UnsignedInt,
		Float,
		HalfFloat,
		UnsignedByte332,
		UnsignedShort4444,
		UnsignedShort5551,
		UnsignedInt8888,
		UnsignedInt1010102,
		UnsignedByte233Reversed,
		UnsignedShort565,
		UnsignedShort565Reversed,
		UnsignedShort4444Reversed,
		UnsignedShort1555Reversed,
		UnsignedInt8888Reversed,
		UnsignedInt2101010Reversed,
		UnsignedInt248,
		UnsignedInt10F11F11FReversed,
		UnsignedInt5999Reversed,
		Float32UnsignedInt248Reversed
	};
}
