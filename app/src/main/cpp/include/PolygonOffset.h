#pragma once

namespace argeo
{
	class PolygonOffset
	{
	public:

		PolygonOffset(
			const bool enabled = false,
			const double& factor = 0.0,
			const double& units  = 0.0)
			: enabled(enabled)
			, factor(factor)
			, units(units)
		{ }

		bool enabled;
		double factor;
		double units;
	};
}

