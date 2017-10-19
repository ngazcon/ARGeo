#pragma once

namespace argeo
{
	struct Interval
	{
		Interval(
			const double& start = 0.0,
			const double& stop  = 0.0)
			: start(start)
			, stop(stop)
		{ }

		double start;
		double stop;
	};
}