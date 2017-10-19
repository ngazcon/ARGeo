#pragma once

namespace argeo
{
	struct Point
	{
		///
		/// Represents a point in the screen.
		///

		///
		/// Construct a point with the given screen coordinates.
		/// @constructor
		/// @param {double} x The x coordinate.
		/// @param {double} y The y coordinate.
		///
		Point(
			const double& x, 
			const double& y)
			: x(x),
			  y(y)
		{ }

		double x;
		double y;
	};
	
}
