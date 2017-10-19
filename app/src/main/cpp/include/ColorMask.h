#pragma once

namespace argeo
{
	class ColorMask
	{
	public:

		ColorMask(
			bool r  = true,
			bool g  = true,
			bool b  = true, 
			bool a  = true)
			: red(r)
			, green(g)
			, blue(b)
			, alpha(a)
		{ }


		bool equals(const ColorMask& other) const
		{
			return
				red == other.red &&
				green == other.green &&
				blue == other.blue &&
				alpha == other.alpha;
		}

		bool red;
		bool green;
		bool blue;
		bool alpha;
	};

	inline bool operator==(const ColorMask& left, const ColorMask& right)
	{
		return left.equals(right);
	}

	inline bool operator !=(const ColorMask& left, const ColorMask& right)
	{
		return !left.equals(right);
	}
}
