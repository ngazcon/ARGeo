#include "TriangleIndicesUnsignedShort.h"

namespace argeo
{
	TriangleIndicesUnsignedShort::TriangleIndicesUnsignedShort(
		unsigned short ui0, 
		unsigned short ui1, 
		unsigned short ui2)
		: ui0(ui0),
		  ui1(ui1),
		  ui2(ui2)
	{ }

	TriangleIndicesUnsignedShort::TriangleIndicesUnsignedShort(
		short ui0,
		short ui1, 
		short ui2)
		: ui0(static_cast<unsigned short>(ui0)),
		  ui1(static_cast<unsigned short>(ui1)),
		  ui2(static_cast<unsigned short>(ui2))
	{ }

	TriangleIndicesUnsignedShort::~TriangleIndicesUnsignedShort()
	{ }
}