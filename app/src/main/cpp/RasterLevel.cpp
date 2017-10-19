#include "RasterLevel.h"
#include "RasterTile.h"
#include "RasterTileIdentifier.h"
#include "RasterSource.h"

namespace argeo
{
	RasterLevel::RasterLevel(
		RasterSource* source, 
		const unsigned int& level, 
		const GeodeticExtent& extent, 
		const double& longitude_posts,
		const double& latitude_posts,
		const unsigned int& longitude_posts_per_tile,
		const unsigned int& latitude_posts_per_tile) :
		m_source(source),
		m_level(level),
		m_extent(extent),
		m_longitude_posts(longitude_posts),
		m_latitude_posts(latitude_posts),
		m_latitude_post_per_tile(latitude_posts_per_tile),
		m_longitude_posts_per_tile(longitude_posts_per_tile),
		m_post_delta_longitude((extent.get_east() - extent.get_west())  / (longitude_posts - 1)),
		m_post_delta_latitude((extent.get_north() - extent.get_south()) / (latitude_posts  - 1))
	{ }


	RasterLevel::~RasterLevel()
	{ }

	RasterSource* RasterLevel::get_source() const
	{
		return m_source;
	}

	GeodeticExtent RasterLevel::get_extent() const
	{
		return m_extent;
	}

	unsigned int RasterLevel::get_level() const
	{
		return m_level;
	}

	unsigned int RasterLevel::get_longitude_posts() const
	{
		return m_longitude_posts;
	}

	unsigned int RasterLevel::get_latitude_posts() const
	{
		return m_latitude_posts;
	}

	unsigned int RasterLevel::get_longitude_posts_per_tile() const
	{
		return m_longitude_posts_per_tile;
	}

	unsigned int RasterLevel::get_latitude_posts_per_tile() const
	{
		return m_latitude_post_per_tile;
	}

	double RasterLevel::get_post_delta_longitude() const
	{
		return m_post_delta_longitude;
	}

	double RasterLevel::get_post_delta_latitude() const
	{
		return m_post_delta_latitude;
	}

	double RasterLevel::longitude_to_index(const double& longitude) const
	{
		return (longitude - m_extent.get_west()) / m_post_delta_longitude;
	}

	double RasterLevel::latitude_to_index(const double& latitude) const
	{
		return (latitude - m_extent.get_south()) / m_post_delta_latitude;
	}

	double RasterLevel::index_to_longitude(const double& longitude_index) const
	{
		return m_extent.get_west() + (longitude_index * m_post_delta_longitude);
	}

	double RasterLevel::index_to_latitude(const double& latitude_index) const
	{
		return m_extent.get_south() + (latitude_index * m_post_delta_latitude);
	}

	std::vector<RasterTileRegion> RasterLevel::get_tiles_in_extent(
		const int& west, 
		const int& south, 
		const int& east, 
		const int& north) const
	{
		int tile_x_start = west   / static_cast<int>(m_longitude_posts_per_tile);
		int tile_x_stop  = east   / static_cast<int>(m_longitude_posts_per_tile);
		int tile_y_start = south  / static_cast<int>(m_latitude_post_per_tile);
		int tile_y_stop  = north  / static_cast<int>(m_latitude_post_per_tile);

		if (west < 0)
		{
			--tile_x_start;
		}
		if (east < 0)
		{
			--tile_x_stop;
		}
		if (south < 0)
		{
			--tile_y_start;
		}
		if (north < 0)
		{
			--tile_y_stop;
		}

		int tile_width  = tile_x_stop - tile_x_start + 1;
		int tile_height = tile_y_stop - tile_y_start + 1;

		std::vector<RasterTileRegion> result;
		result.reserve(tile_width * tile_height);

		// Generate the tiles regions that are contained in the level region.
		// Each tile region contains a tile and the extent needed.
		for (int tile_y = tile_y_start; tile_y <= tile_y_stop; ++tile_y)
		{
			int tile_y_origin = tile_y * m_latitude_post_per_tile;

			int region_south = south - tile_y_origin;
			if (region_south < 0)
				region_south = 0;

			int region_north = north - tile_y_origin;
			if (region_north >= m_latitude_post_per_tile)
				region_north = m_latitude_post_per_tile - 1;

			for (int tile_x = tile_x_start; tile_x <= tile_x_stop; ++tile_x)
			{
				int tile_x_origin = tile_x * m_longitude_posts_per_tile;

				int region_west = west - tile_x_origin;
				if (region_west < 0)
					region_west = 0;

				int region_east = east - tile_x_origin;
				if (region_east >= m_longitude_posts_per_tile)
					region_east = m_longitude_posts_per_tile - 1;

				// Get the raster tile from the source.
				RasterTile* tile =
					m_source->get_tile(RasterTileIdentifier(m_level, tile_x, tile_y));

				// Add the tile region.
				result.push_back(RasterTileRegion(
					tile,
					region_west,
					region_south,
					region_east,
					region_north));
			}
		}

		return result;
	}
}