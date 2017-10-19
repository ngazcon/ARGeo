#pragma once

namespace argeo
{
	class PrimitiveRestart
	{
	public:

		PrimitiveRestart(
			bool enabled = false,
			int  index   = 0)
			: enabled(enabled)
			, index(index)
		{ }

		bool enabled;
		int index;
	};
}
