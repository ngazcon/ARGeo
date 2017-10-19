#pragma once
namespace argeo
{
	class DepthRange
	{
	public:

		DepthRange(
			double near = 0.0,
			double far  = 1.0)
			: near_value(near)
			, far_value(far)
		{ }

		const double near_value;
		const double far_value;
	};
}
