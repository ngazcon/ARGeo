#pragma once

#include "RasterTileIdentifier.h"
#include "RasterLevel.h"
#include "Texture2D.h"
#include "RasterSourceTileData.h"

#include <memory>

namespace argeo
{
	class RasterSource;

	class RasterTile
	{
	public:

		RasterTile(
			RasterSource* source,
			RasterTileIdentifier identifier);

		~RasterTile();

		RasterTileIdentifier get_identifier() const;

		RasterLevel*  get_level()  const;
		RasterSource* get_source() const;

		double get_maximum_height() const;
		void   set_maximum_height(const double& value);

		double get_minimum_height() const;
		void   set_minimum_height(const double& value);

		int get_west()  const;
		int get_south() const;
		int get_east()  const;
		int get_north() const;

		int get_height() const;
		int get_width() const;

        GeodeticExtent get_extent() const;

		RasterTile* get_southwest_child() const;
		RasterTile* get_southeast_child() const;
		RasterTile* get_northwest_child() const;
		RasterTile* get_northeast_child() const;

		RasterSourceTileData load_data() const;

		void on_changed() const;

	private:
		RasterTileIdentifier m_identifier;
		RasterLevel* m_level;

		double m_maximum_height;
		double m_minimum_height;

		int m_west;
		int m_south;
		int m_east;
		int m_north;
	};
}
