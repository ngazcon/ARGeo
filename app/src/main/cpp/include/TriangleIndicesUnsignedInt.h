#pragma once

namespace argeo
{
	class TriangleIndicesUnsignedInt
	{
	public:
		TriangleIndicesUnsignedInt(
			unsigned int ui0,
			unsigned int ui1,
			unsigned int ui2);

		TriangleIndicesUnsignedInt(
			int ui0,
			int ui1,
			int ui2);

		~TriangleIndicesUnsignedInt();

		unsigned int ui0;
		unsigned int ui1;
		unsigned int ui2;
	};
}
