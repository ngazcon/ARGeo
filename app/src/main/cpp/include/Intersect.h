#pragma once

namespace argeo
{
	///
	/// Describes the intersection test result.
	///
	enum class Intersect : unsigned int
	{
		Outside   = 0,
		Inside    = 1,
		Intersect = 2,
	};
}