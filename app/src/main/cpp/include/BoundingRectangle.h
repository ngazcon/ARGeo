#pragma once

#include "Vector.h"

#include <vector>

namespace argeo
{
	struct BoundingRectangle
	{
		BoundingRectangle(
			double x      = 0,
			double y      = 0,
			double width  = 0, 
			double height = 0)
			: x(x)
			, y(y)
			, width(width)
			, height(height)
		{ }

		bool intersect(
			BoundingRectangle left,
			BoundingRectangle right)
		{
			double left_x  = left.x;
			double left_y  = left.y;
			double right_x = right.x;
			double right_y = right.y;

			if (!(left_x > right_x + right.width ||
				  left_x + left.width  < right_x ||
				  left_y + left.height < right_y ||
				  left_y > right_y + right.height))
			{
				return true;
			}

			return false;
		};

		static BoundingRectangle from_points(std::vector<vec2d> positions) 
		{
			BoundingRectangle result;

			if (positions.size() == 0) 
			{
				result.x      = 0;
				result.y      = 0;
				result.width  = 0;
				result.height = 0;
				return result;
			}

			double length   = positions.size();

			double minimumX = positions[0].x;
			double minimumY = positions[0].y;

			double maximumX = positions[0].x;
			double maximumY = positions[0].y;

			for (int i = 1; i < length; i++)
			{
				vec2d p  = positions[i];
				double x = p.x;
				double y = p.y;

				minimumX = fmin(x, minimumX);
				maximumX = fmax(x, maximumX);
				minimumY = fmin(y, minimumY);
				maximumY = fmax(y, maximumY);
			}

			result.x	  = minimumX;
			result.y	  = minimumY;
			result.width  = maximumX - minimumX;
			result.height = maximumY - minimumY;

			return result;
		};

		double x;
		double y;
		double width;
		double height;
	};
}
