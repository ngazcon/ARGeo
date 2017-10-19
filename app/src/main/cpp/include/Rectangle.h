#pragma once

#include <stdexcept>

namespace argeo
{
	struct Rectangle
	{
	public:

		Rectangle(
			unsigned int x	    = 0,
			unsigned int y	    = 0,
			unsigned int width  = 0,
			unsigned int height = 0)
			: x(x)
			, y(y)
			, width(width)
			, height(height)
		{  }

		unsigned int get_width() const
		{
			return width;
		}

		void set_width(unsigned int value)
		{
			width = value;
		}

		unsigned int get_height() const
		{
			return height;
		}

		void set_height(unsigned int value)
		{
			height = value;
		}


		void set_left(unsigned int value)
		{
			x = value;
		}

		unsigned int get_left() const
		{
			return x;
		}

		void set_bottom(unsigned int value)
		{
			y = value;
		}

		unsigned int get_bottom() const
		{
			return y;
		}

		unsigned int get_right() const
		{
			return x + width;
		}

		unsigned int get_top() const
		{
			return y + height;
		}

		bool equals(Rectangle other) const
		{
			return
				(width == other.width) &&
				(height == other.height) &&
				(y == other.y) &&
				(x == other.x);
		}

	private:

		unsigned int x;
		unsigned int y;
		unsigned int width;
		unsigned int height;

	};

	inline bool operator==(const Rectangle& left, const Rectangle& right)
	{
		return left.equals(right);
	}

	inline bool operator!=(const Rectangle& left, const Rectangle& right)
	{
		return !left.equals(right);
	}
}
