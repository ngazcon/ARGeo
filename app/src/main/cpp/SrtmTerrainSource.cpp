#include "SrtmTerrainSource.h"

#include "Device.h"
#include "EmbeddedResources.h"
#include "StringHelper.h"

#include <iomanip>
#include <fstream>
#include <algorithm>
#include <thread>
#include <fcntl.h>


namespace argeo
{
	// Token for unsensed data.
	const float VOID = -32768;

    //const int NUMBER_OF_LEVELS = 1;
	const int NUMBER_OF_LEVELS = 4;

    const int MIN_HEIGHT = -44;


	// This sizes defines the resolution of the tile.
	const int SOURCE_TILE_WIDTH  = 1201;
	const int SOURCE_TILE_HEIGHT = 1201;

	// This deltas defines the degrees covered by one tile of this source.
	const int SOURCE_LEVEL_ZERO_DELTA_LONGITUDE_DEGREES = 8;
	const int SOURCE_LEVEL_ZERO_DELTA_LATITUDE_DEGREES  = 8;
    //const int SOURCE_LEVEL_ZERO_DELTA_LONGITUDE_DEGREES = 1;
    //const int SOURCE_LEVEL_ZERO_DELTA_LATITUDE_DEGREES  = 1;

	SrtmTerrainSource::SrtmTerrainSource(ITerrain* terrain)
		: RasterSource::RasterSource(terrain)
        , m_extent(-180.0, -90.0, 180.0, 90.0)
        , m_max_height(-std::numeric_limits<float>::max())
        , m_min_height(std::numeric_limits<float>::max())
	{
		m_levels_collection.resize(NUMBER_OF_LEVELS);

		double delta_longitude = SOURCE_LEVEL_ZERO_DELTA_LONGITUDE_DEGREES;
		double delta_latitude  = SOURCE_LEVEL_ZERO_DELTA_LATITUDE_DEGREES;

		double longitude_extent = m_extent.get_east()  - m_extent.get_west();
		double latitude_extent  = m_extent.get_north() - m_extent.get_south();
	
		for (int i = 0; i < m_levels_collection.size(); ++i)
		{
			double longitude_posts = (longitude_extent / delta_longitude) * SOURCE_TILE_WIDTH   + 1;
			double latitude_posts  = (latitude_extent  / delta_latitude)  * SOURCE_TILE_HEIGHT  + 1;

			m_levels_collection[i] = new RasterLevel(
				this,
				i,
				m_extent,
				longitude_posts,
				latitude_posts,
				SOURCE_TILE_WIDTH,
				SOURCE_TILE_HEIGHT);

			delta_longitude /= 2.0;
			delta_latitude  /= 2.0;
		}

		m_write_pixel_buffer = Device::create_write_pixel_buffer(
			PixelBufferHint::Stream,
			SOURCE_TILE_HEIGHT *
			SOURCE_TILE_WIDTH *
			sizeof(float));
	}


	SrtmTerrainSource::~SrtmTerrainSource()
	{
		for (int i = 0; i < m_levels_collection.size(); ++i)
		{
			delete m_levels_collection[i];
		}
	}

	RasterSourceTileData SrtmTerrainSource::load_tile_data(
		RasterTileIdentifier identifier)
	{
		unsigned int divisor = pow(2.0, identifier.level);
		unsigned int files_to_read = pow(2.0, NUMBER_OF_LEVELS - identifier.level - 1);

		// Each of the 27 tiles that covers 50 degrees of latitude and 40 degrees of longitude has 6,000 rows and 4,800 columns. 
		// Each of the 6 Antarctica tiles that cover 30 degrees of latitude and 60 degrees of longitude has 3,600 rows and 7,200 columns. 
		unsigned int heights_to_read = SOURCE_TILE_HEIGHT * SOURCE_TILE_WIDTH;

		float* heightmap = m_write_pixel_buffer->map_range<float>(
			0,
			m_write_pixel_buffer->get_size_in_bytes(),
			BufferAccess::UnsynchronizedBit | BufferAccess::WriteBit);

		std::fill(heightmap, heightmap + heights_to_read, 0.0f);

		double longitude_resolution = SOURCE_LEVEL_ZERO_DELTA_LONGITUDE_DEGREES / divisor;
		double latitude_resolution  = SOURCE_LEVEL_ZERO_DELTA_LATITUDE_DEGREES  / divisor;

		double west  = -180.0 + longitude_resolution * identifier.x;
		double south = -90.0  + latitude_resolution  * identifier.y;
		
		float maximum = -std::numeric_limits<float>::max();
		float minimum =  std::numeric_limits<float>::max();

		std::atomic<float> atomic_maximum(maximum);
		std::atomic<float> atomic_minimum(minimum);
		std::vector<std::thread> thread_pool;

		for (unsigned int i = 0; i < files_to_read; i++)
		{
			for (unsigned int j = 0; j < files_to_read; j++)
			{
				const int tile_west  = west + j;
				const int tile_south = south + i;

				std::ostringstream pbuffer;
				pbuffer << "raster/srtm/";
				pbuffer << (tile_south < 0 ? "S" : "N") << std::setw(2) << std::setfill('0') << static_cast<int>(std::abs(tile_south));
				pbuffer << (tile_west  < 0 ? "W" : "E") << std::setw(3) << std::setfill('0') << static_cast<int>(std::abs(tile_west));
				pbuffer << ".hgt";

				if (EmbeddedResources::resource_exists(pbuffer.str()))
				{

                    read(
                            pbuffer.str(),
                            identifier.level,
                            files_to_read - i - 1,
                            j,
                            heightmap,
                            atomic_maximum,
					        atomic_minimum);

					/*
                    thread_pool.push_back(std::thread(
                            &SrtmTerrainSource::read,
                            this,
                            pbuffer.str(),
                            identifier.level,
                            files_to_read - i - 1,
                            j,
                            heightmap,
                            std::ref(atomic_maximum),
                            std::ref(atomic_minimum)));
                            */
				}
			}

			for (std::thread &t : thread_pool)
			{
				t.join();
			}

			maximum = atomic_maximum;
			minimum = atomic_minimum;
		}


		RasterSourceTileData data;
		data.maximum = maximum;
		data.minimum = minimum;

		m_write_pixel_buffer->unmap();

		data.texture = Device::create_texture2D(TextureDescription(
			SOURCE_TILE_WIDTH,
			SOURCE_TILE_HEIGHT,
			TextureFormat::Red32f,
			false));

		data.texture->copy_from_buffer(
			m_write_pixel_buffer.get(),
			ImageFormat::Red,
			ImageDataType::Float);
		
		return data;
	}

	void SrtmTerrainSource::write_to_file(
		const std::string& filepath,
		float* data) const
	{
		if (!EmbeddedResources::resource_directory_exists(StringHelper::dirname(filepath)))
		{
			EmbeddedResources::create_resource_path(filepath);
		}

		FILE* file = EmbeddedResources::resource_open(filepath, "wb");

		for (unsigned int row = 0; row < SOURCE_TILE_HEIGHT; row++)
		{
			for (unsigned int col = 0; col < SOURCE_TILE_WIDTH; col++)
			{
				unsigned int data_row = SOURCE_TILE_HEIGHT - row - 1;
				unsigned int data_col = col;

				// Get the height stored for the position.
				// The data is stored in big endian so we need to parse it!
				int  height = static_cast<int>(data[data_row * SOURCE_TILE_WIDTH + data_col]);

				// The writed int is signed so we need to mask it and convert.
				height = (height & ~(1 << 15)) + (height & (1 << 15));

				char high_byte = (height >> 8) & 0xFF;
				char low_byte  = (height & 0xFF);

				char height_to_write[2] = { high_byte, low_byte };

				unsigned int index = row * SOURCE_TILE_WIDTH + col;

				::fwrite(height_to_write, sizeof(char), 2, file);
			}
		}

		::fclose(file);
		delete data;
	}

	void SrtmTerrainSource::read_from_file(
		const std::string& filepath,
		float* data,
		float& maximum,
		float& minimum) const
	{
		unsigned int buf_length = SOURCE_TILE_HEIGHT * SOURCE_TILE_WIDTH * 2;

		// Buffer used to store file data
		std::unique_ptr<unsigned char> buf(new unsigned char[buf_length]);

		// Open the file.
		FILE* asset_file = EmbeddedResources::resource_open(filepath, "r");

		/* Advise the kernel of our access pattern.  */
		posix_fadvise64(
			fileno(asset_file),
			0,
			0,
			POSIX_FADV_SEQUENTIAL);

		if (::fread(
			reinterpret_cast<unsigned char*>(buf.get()),
			sizeof(unsigned char),
			buf_length,
			asset_file))
		{
			for (unsigned int row = 0; row < SOURCE_TILE_HEIGHT; row++)
			{
				for (unsigned int col = 0; col < SOURCE_TILE_WIDTH; col++)
				{
					unsigned int index = (row * SOURCE_TILE_WIDTH + col);

					// Get the height stored for the position.
					// Parse from signed char to unsigned.
					// The data is stored in big endian so we need to parse it!
					//int height = (buf.get()[index * 2 + 0]) << 8 | (buf.get()[index * 2 + 1]);
					// The readed int is signed so we need to mask it and convert.
					//height = (height & ~(1 << 15)) - (height & (1 << 15));

					int height = row;

					unsigned int data_row = SOURCE_TILE_HEIGHT - row - 1;
					unsigned int data_col = col;

					data[data_row * SOURCE_TILE_WIDTH + data_col] =
						static_cast<float>(height);

					const float max = maximum;
					const float min = minimum;

					maximum = std::max(max, static_cast<float>(height));
					minimum = std::min(min, static_cast<float>(height));
				}
			}
		}

		// Close the file descriptor.
		::fclose(asset_file);
	}

	void SrtmTerrainSource::read(
		const std::string& filepath,
		const unsigned int& level,
		const unsigned int& rows_offset,
		const unsigned int& columns_offset,
		float* data,
		std::atomic<float>& maximum,
		std::atomic<float>& minimum)
	{
		unsigned int step = pow(2.0, NUMBER_OF_LEVELS - level - 1);
		unsigned int buf_length = SOURCE_TILE_HEIGHT * SOURCE_TILE_WIDTH * 2;

		unsigned int rows_per_file = (SOURCE_TILE_HEIGHT / step);
		unsigned int cols_per_file = (SOURCE_TILE_WIDTH / step);

		unsigned int row_offset = rows_per_file * rows_offset;
		unsigned int col_offset = cols_per_file * columns_offset;

		// Buffer used to store file data
		std::unique_ptr<unsigned char> buf(new unsigned char[buf_length]);

		// Open the file.
		FILE* asset_file = EmbeddedResources::resource_open(filepath, "r");

		/* Advise the kernel of our access pattern.  */
		posix_fadvise64(
			fileno(asset_file),
			0,
			0,
			POSIX_FADV_SEQUENTIAL);

		if (::fread(
			reinterpret_cast<unsigned char*>(buf.get()),
			sizeof(unsigned char),
			buf_length,
			asset_file))
		{
			for (unsigned int row = 0; row < rows_per_file; row++)
			{
				for (unsigned int col = 0; col < cols_per_file; col++)
				{
					unsigned int index = (row * SOURCE_TILE_WIDTH + col) * step;

					// Get the height stored for the position.
					// Parse from signed char to unsigned.
					// The data is stored in big endian so we need to parse it!
					int height = (buf.get()[index * 2 + 0]) << 8 | (buf.get()[index * 2 + 1]);

					// The readed int is signed so we need to mask it and convert.
					height = (height & ~(1 << 15)) - (height & (1 << 15));
                    if (height == VOID)
                    {
                        height = MIN_HEIGHT;
                    }

                    /*
                    double delta_row = (double)rows_per_file / 2.0 - row;
                    double delta_col = (double)cols_per_file / 2.0 - col;

                    if(5 >= ::fabs(delta_row) &&
                       5 >= ::fabs(delta_col))
                    {
                        height = ::fabs(delta_row) * 1000;
                    }
                    else {
                        height = 100;
                    }
*/

					unsigned int data_row = SOURCE_TILE_HEIGHT - row_offset - row - 1;
					unsigned int data_col = col_offset + col;

					data[data_row * SOURCE_TILE_WIDTH + data_col] =
						static_cast<float>(height);

					const float max = maximum;
					const float min = minimum;

					maximum = std::max(max, static_cast<float>(height));
					minimum = std::min(min, static_cast<float>(height));
				}
			}
		}

        const float max = maximum;
        const float min = minimum;
        m_max_height  = std::max(m_max_height, max);
        m_min_height  = std::min(m_min_height, min);

		// Close the file descriptor.
		::fclose(asset_file);
	}

	int SrtmTerrainSource::get_tile_longitude_posts() const
	{
		return SOURCE_TILE_WIDTH;
	}

	int SrtmTerrainSource::get_tile_latitude_posts() const
	{
		return SOURCE_TILE_HEIGHT;
	}

	RasterLevelCollection SrtmTerrainSource::get_levels() const
	{
		return m_levels_collection;
	}

	GeodeticExtent SrtmTerrainSource::get_extent() const
	{
		return m_extent;
	}

	double SrtmTerrainSource::get_maximum_height() const
	{
		return m_max_height;
	}

	double SrtmTerrainSource::get_minimum_height() const
	{
		return m_min_height;
	}
}

