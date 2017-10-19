#include "RasterTile.h"
#include "RasterLevelCollection.h"
#include "RasterSource.h"

#include <cmath>

namespace argeo
{
	RasterTile::RasterTile(
		RasterSource* source, 
		RasterTileIdentifier identifier)
		: m_identifier(identifier)
		, m_level(source->get_levels().at(identifier.level))
		, m_west(identifier.x * m_level->get_longitude_posts_per_tile())
	    , m_east(fmin(m_west + m_level->get_latitude_posts_per_tile(), m_level->get_longitude_posts()) - 1)
		, m_south(identifier.y * m_level->get_latitude_posts_per_tile())
		, m_north(fmin(m_south + m_level->get_latitude_posts_per_tile(), m_level->get_latitude_posts()) - 1)
		, m_maximum_height(-std::numeric_limits<double>::max())
		, m_minimum_height(std::numeric_limits<double>::max())
	{ }


	RasterTile::~RasterTile()
	{ }

    double RasterTile::get_maximum_height() const
    {
        return m_maximum_height;
    }

    void RasterTile::set_maximum_height(const double &value)
    {
        m_maximum_height = value;
    }

    double RasterTile::get_minimum_height() const
    {
        return m_minimum_height;
    }

    void RasterTile::set_minimum_height(const double &value)
    {
        m_minimum_height = value;
    }

	RasterTile* RasterTile::get_southwest_child() const
	{
		RasterTileIdentifier current = m_identifier;
		RasterTileIdentifier child = RasterTileIdentifier(current.level + 1, current.x * 2, current.y * 2);
		return get_source()->get_tile(child);
	}

	RasterTile* RasterTile::get_southeast_child() const
	{
		RasterTileIdentifier current = m_identifier;
		RasterTileIdentifier child = RasterTileIdentifier(current.level + 1, current.x * 2 + 1, current.y * 2);
		return get_source()->get_tile(child);
	}

	RasterTile* RasterTile::get_northwest_child() const
	{
		RasterTileIdentifier current = m_identifier;
		RasterTileIdentifier child = RasterTileIdentifier(current.level + 1, current.x * 2, current.y * 2 + 1);
		return get_source()->get_tile(child);
	}

	RasterTile* RasterTile::get_northeast_child() const
	{
		RasterTileIdentifier current = m_identifier;
		RasterTileIdentifier child = RasterTileIdentifier(current.level + 1, current.x * 2 + 1, current.y * 2 + 1);
		return get_source()->get_tile(child);
	}

	RasterSourceTileData RasterTile::load_data() const
	{
		return get_source()->load_tile_data(m_identifier);
	}

	void RasterTile::on_changed() const
	{
		get_source()->on_tile_changed(this);
	}

	RasterSource* RasterTile::get_source() const
	{
		return m_level->get_source();
	}

	RasterTileIdentifier RasterTile::get_identifier() const
	{
		return m_identifier;
	}

	RasterLevel* RasterTile::get_level() const
	{
		return m_level;
	}

    GeodeticExtent RasterTile::get_extent() const
    {
        return GeodeticExtent(
                ArgeoMath::to_radians(m_level->index_to_longitude(m_west)),
                ArgeoMath::to_radians(m_level->index_to_latitude(m_south)),
                ArgeoMath::to_radians(m_level->index_to_longitude(m_east)),
                ArgeoMath::to_radians(m_level->index_to_latitude(m_north)));
    }

	int RasterTile::get_west() const
	{
		return m_west;
	}

	int RasterTile::get_south() const
	{
		return m_south;
	}

	int RasterTile::get_east() const
	{
		return m_east;
	}

	int RasterTile::get_north() const
	{
		return m_north;
	}

	int RasterTile::get_width() const
	{
		return m_east - m_west + 1;
	}

	int RasterTile::get_height() const
	{
		return m_north - m_south + 1;
	}
}