#pragma once

#include <functional>
#include <sstream>
#include <string>

namespace argeo
{

	struct RasterTileIdentifier
	{
	public:

		RasterTileIdentifier(int level, int x, int y)
			: level(level),
			  x(x),
			  y(y)
		{ }

		bool equals(RasterTileIdentifier other) const
		{
			return level == other.level &&
				   x     == other.x &&
				   y     == other.y;
		}

		
		const int x;
		const int y;
		const int level;
	};

	inline bool operator==(const RasterTileIdentifier& left, const RasterTileIdentifier& right)
	{
		return left.equals(right);
	}

	inline bool operator<(const RasterTileIdentifier& left, const RasterTileIdentifier& right)
	{
		return left.level < right.level;
	}

	struct RasterTileIdentifierHasher
	{
		std::size_t operator()(const RasterTileIdentifier& t) const
		{
			std::stringstream sbuf;

			sbuf <<
				 "l" << t.level <<
				 "x" << t.x <<
				 "y" << t.y;


			//calculate hash here.
			return std::hash<std::string>()(sbuf.str());
		}
	};
}