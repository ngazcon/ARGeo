#pragma once

#include "Vector.h"

namespace argeo
{
	struct Color
	{
		///
		/// Represents a RGBA color.
		///

		///
		/// Construct a color with the given RGBA values.
		/// @constructor
		/// @param {unsigned char} [r = 0] The red component.
		/// @param {unsigned char} [g = 0] The green component.
		/// @param {unsigned char} [b = 0] The blue component.
		/// @param {unsigned char} [a = 0] The alpha component.
		///
		Color(
			const unsigned char& r = 0, 
			const unsigned char& g = 0,
			const unsigned char& b = 0, 
			const unsigned char& a = 0)
			: red(r)
			, green(g)
			, blue(b)
			, alpha(a)
		{ }

		///
		/// Construct a color with the given hexa RGBA value.
		/// @constructor
		/// @param {unsigned int} hexa The hexa RGBA value.
		///
		Color(const unsigned int& hexa)
			: red((hexa >> 24) & 0xFF)
			, green((hexa >> 16) & 0xFF)
			, blue((hexa >> 8) & 0xFF)
			, alpha((hexa) & 0xFF)
		{ }

		union
		{
			struct
			{
				unsigned char red;
				unsigned char green;
				unsigned char blue;
				unsigned char alpha;
			};
			unsigned char data[4];
		};
		
	};

	inline bool operator==(
		const Color& left,
		const Color& right)
	{
		return
			(left.red == right.red) &&
			(left.green == right.green) &&
			(left.blue == right.blue) &&
			(left.alpha == right.alpha);
	}

	inline bool operator!=(
		const Color& left,
		const Color& right)
	{
		return
			(left.red != right.red) ||
			(left.green != right.green) ||
			(left.blue != right.blue) ||
			(left.alpha != right.alpha);
	}
}
