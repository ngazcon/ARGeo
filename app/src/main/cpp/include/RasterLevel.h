#pragma once

#include "GeodeticExtent.h"
#include "RasterTileRegion.h"

#include <vector>

namespace argeo
{
	class RasterSource;

	class RasterLevel
	{
	public:

		RasterLevel(
			RasterSource* source,
			const unsigned int& level,
			const GeodeticExtent& extent,
			const double& longitude_posts,
			const double& latitude_posts,
			const unsigned int& longitude_posts_per_tile,
			const unsigned int& latitude_posts_per_tile);

		~RasterLevel();

		// Gets the tiles region contained in the given extent.
		// Each tile region is separated by a unitary displacment.
		std::vector<RasterTileRegion> get_tiles_in_extent(
			const int& west,
			const int& south,
			const int& east,
			const int& north) const;

		RasterSource*  get_source() const;
		GeodeticExtent get_extent() const;

		unsigned int get_level() const;
		unsigned int get_longitude_posts() const;
		unsigned int get_latitude_posts() const;
		unsigned int get_longitude_posts_per_tile() const;
		unsigned int get_latitude_posts_per_tile() const;
		double get_post_delta_longitude() const;
		double get_post_delta_latitude() const;

		double longitude_to_index(const double& longitude) const;
		double latitude_to_index(const double& latitude) const;
		double index_to_longitude(const double& longitude_index) const;
		double index_to_latitude(const double& latitude_index) const;

	private:
		RasterSource* m_source;
		GeodeticExtent m_extent;

		unsigned int m_level;
		unsigned int m_longitude_posts;
		unsigned int m_latitude_posts;
		unsigned int m_longitude_posts_per_tile;
		unsigned int m_latitude_post_per_tile;
		double m_post_delta_longitude;
		double m_post_delta_latitude;
	};
}
