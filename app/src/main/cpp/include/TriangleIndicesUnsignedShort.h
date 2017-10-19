#pragma once

namespace argeo
{
	class TriangleIndicesUnsignedShort
	{
	public:
		TriangleIndicesUnsignedShort(unsigned short ui0, unsigned short ui1, unsigned short ui2);
		TriangleIndicesUnsignedShort(short ui0, short ui1, short ui2);

		~TriangleIndicesUnsignedShort();

		unsigned short ui0;
		unsigned short ui1;
		unsigned short ui2;
	};
}
