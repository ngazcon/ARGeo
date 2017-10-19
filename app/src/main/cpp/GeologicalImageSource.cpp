#include "GeologicalImageSource.h"

#include "Device.h"
#include "WritePixelBuffer.h"
#include "TextureHelper.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>

namespace argeo
{
	GeologicalImageSource::GeologicalImageSource(ITerrain* terrain)
		: RasterSource::RasterSource(terrain)
		, m_extent(-180.0, -90.0, 180.0, 90.0)
	{
		m_levels_collection.resize(NUMBER_OF_LEVELS);

		m_level_zero_delta_latitude_degrees = SOURCE_LEVEL_ZERO_DELTA_LATITUDE_DEGREES;
		m_level_zero_delta_longitude_degrees = SOURCE_LEVEL_ZERO_DELTA_LONGITUDE_DEGREES;

		m_tile_height = SOURCE_TILE_HEIGHT;
		m_tile_width = SOURCE_TILE_WIDTH;

		double delta_longitude = m_level_zero_delta_longitude_degrees;
		double delta_latitude = m_level_zero_delta_latitude_degrees;

		double longitude_extent = m_extent.get_east() - m_extent.get_west();
		double latitude_extent = m_extent.get_north() - m_extent.get_south();

		for (int i = 0; i < m_levels_collection.size(); ++i)
		{
			int longitude_posts = (int)round(longitude_extent / delta_longitude) * get_tile_longitude_posts() + 1;
			int latitude_posts = (int)round(latitude_extent / delta_latitude)  * get_tile_latitude_posts() + 1;

			m_levels_collection[i] = new RasterLevel(
				this,
				i,
				m_extent,
				longitude_posts,
				latitude_posts,
				get_tile_longitude_posts(),
				get_tile_latitude_posts());

			delta_longitude /= 2.0;
			delta_latitude  /= 2.0;
		}
	}


	GeologicalImageSource::~GeologicalImageSource()
	{
		for (int i = 0; i < m_levels_collection.size(); ++i)
		{
			delete m_levels_collection[i];
		}
	}

	RasterSourceTileData GeologicalImageSource::load_tile_data(RasterTileIdentifier identifier) const
	{
		double divisor = pow(2.0, identifier.level);

		// Each of the 27 tiles that covers 50 degrees of latitude and 40 degrees of longitude has 6,000 rows and 4,800 columns. 
		// Each of the 6 Antarctica tiles that cover 30 degrees of latitude and 60 degrees of longitude has 3,600 rows and 7,200 columns. 

		int heights_to_read = get_tile_longitude_posts() * get_tile_latitude_posts();

		std::vector<float> heightmap;
		heightmap.resize(heights_to_read);

		double longitude_resolution = m_level_zero_delta_longitude_degrees / divisor;
		double latitude_resolution = m_level_zero_delta_latitude_degrees / divisor;

		double west = -180.0 + longitude_resolution * (identifier.x + 0);
		double east = -180.0 + longitude_resolution * (identifier.x + 1);
		double north = -90.0 + latitude_resolution  * (identifier.y + 0);
		double south = -90.0 + latitude_resolution  * (identifier.y + 1);

		std::string file_name;
		std::stringstream sbuffer;

		//file_name.append(south < 0 ? "S" : "N");
		//sbuffer << std::setw(2) << std::setfill('0') << static_cast<int>(abs(south));
		//file_name.append(sbuffer.str());
		//
		//sbuffer.str(std::string());
		//
		//file_name.append(west < 0 ? "W" : "E");
		//sbuffer << std::setw(3) << std::setfill('0') << static_cast<int>(abs(west));
		//file_name.append(sbuffer.str());


		file_name.append("pergamino.jpg");

		std::string file_path;
		file_path.append("imagery/");

		std::unique_ptr<Bitmap> bitmap = Bitmap::create_bitmap_from_asset(file_path + file_name);

		return RasterSourceTileData(
			0,
			0,
			Device::create_texture2D(
				bitmap.get(),
				TextureHelper::imaging_pixel_format_to_texture_format(bitmap->get_pixel_format())));
	}

	int GeologicalImageSource::get_tile_longitude_posts() const
	{
		return m_tile_width;
	}

	int GeologicalImageSource::get_tile_latitude_posts() const
	{
		return m_tile_height;
	}

	RasterLevelCollection GeologicalImageSource::get_levels() const
	{
		return m_levels_collection;
	}

	GeodeticExtent GeologicalImageSource::get_extent() const
	{
		return m_extent;
	}

	double GeologicalImageSource::get_maximum_height() const
	{
		return MAX_HEIGHT;
	}

	double GeologicalImageSource::get_minimum_height() const
	{
		return MIN_HEIGHT;
	}
}

