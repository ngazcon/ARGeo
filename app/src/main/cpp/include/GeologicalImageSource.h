#pragma once

#include "RasterSource.h"
#include "RasterLevel.h"

#include <string>
#include <vector>

namespace argeo
{
	class GeologicalImageSource : public RasterSource
	{
	public:
		GeologicalImageSource(ITerrain* terrain);
		~GeologicalImageSource();

		RasterLevelCollection get_levels() const;
		GeodeticExtent get_extent() const;
		double get_maximum_height() const;
		double get_minimum_height() const;

		RasterSourceTileData load_tile_data(RasterTileIdentifier identifier) const;

		int get_tile_longitude_posts() const;
		int get_tile_latitude_posts() const;

	private:

		// The raster levels.
		RasterLevelCollection m_levels_collection;

		// The extent of the coarsest level.
		GeodeticExtent m_extent;

		// The amount of loaded tiles.
		int m_tiles_loaded;

		int m_level_zero_delta_longitude_degrees;
		int m_level_zero_delta_latitude_degrees;

		int m_tile_width;
		int m_tile_height;

		//
		// Constansts for this terrain source.
		//

		// Token for unsensed data.
		const float VOID = -32768;

		const int NUMBER_OF_LEVELS = 1;

		// This sizes defines the resolution of the tile.
		const int SOURCE_TILE_WIDTH  = 1201;
		const int SOURCE_TILE_HEIGHT = 1201;

		const int MIN_HEIGHT = -44;
		const int MAX_HEIGHT = 1240;

		// This deltas defines the degrees covered by one tile of this source.
		const int SOURCE_LEVEL_ZERO_DELTA_LONGITUDE_DEGREES = 1.0;
		const int SOURCE_LEVEL_ZERO_DELTA_LATITUDE_DEGREES = 1.0;
	};
}
