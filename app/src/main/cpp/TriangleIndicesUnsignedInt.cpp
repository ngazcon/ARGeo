#include "TriangleIndicesUnsignedInt.h"

namespace argeo
{
	TriangleIndicesUnsignedInt::TriangleIndicesUnsignedInt(
		unsigned int ui0, 
		unsigned int ui1, 
		unsigned int ui2)
		: ui0(ui0),
		  ui1(ui1),
		  ui2(ui2)
	{ }

	TriangleIndicesUnsignedInt::TriangleIndicesUnsignedInt(
		int ui0, 
		int ui1, 
		int ui2)
		: ui0(static_cast<unsigned int>(ui0)),
		  ui1(static_cast<unsigned int>(ui1)),
		  ui2(static_cast<unsigned int>(ui2))
	{ }

	TriangleIndicesUnsignedInt::~TriangleIndicesUnsignedInt()
	{ }
}