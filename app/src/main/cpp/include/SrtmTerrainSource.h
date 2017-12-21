#pragma once

#include "RasterSource.h"
#include "RasterLevel.h"
#include "WritePixelBuffer.h"

#include <string>
#include <vector>
#include <atomic>

namespace argeo
{
	class SrtmTerrainSource : public RasterSource
	{
	public:
		SrtmTerrainSource(ITerrain* terrain);
		~SrtmTerrainSource();

		RasterLevelCollection get_levels() const;
		GeodeticExtent get_extent() const;
		double get_maximum_height() const;
		double get_minimum_height() const;

		RasterSourceTileData load_tile_data(RasterTileIdentifier identifier);

		int get_tile_longitude_posts() const;
		int get_tile_latitude_posts() const;


	private:

		std::unique_ptr<WritePixelBuffer> m_write_pixel_buffer;

		void write_to_file(
			const std::string& filepath,
			float* data) const;

		void read_from_file(
			const std::string& filepath,
			float* data,
			float& maximum,
			float& minimum)const;

		void read(
			const std::string& filepath,
			const unsigned int& level,
			const unsigned int& rows_offset,
			const unsigned int& columns_offset,
			float* data,
			std::atomic<float>& maximum,
			std::atomic<float>& minimum);

		// The raster levels.
		RasterLevelCollection m_levels_collection;

		// The extent of the coarsest level.
		GeodeticExtent m_extent;

		// The amount of loaded tiles.
		int m_tiles_loaded;

		float m_min_height;
		float m_max_height;
	};
}
