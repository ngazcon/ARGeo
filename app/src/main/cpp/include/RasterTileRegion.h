#pragma once

namespace argeo
{
	class RasterTile;

	struct RasterTileRegion
	{
	public:

		RasterTileRegion(
			RasterTile* tile, 
			int west, 
			int south,
			int east,
			int north)
			: tile(tile)
			, west(west)
			, south(south)
			, east(east)
			, north(north)
		{ }

		RasterTile* tile;
		int west;
		int south;
		int east;
		int north;
	};
}

