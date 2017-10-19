#include "ClipmapUpdate.h"

#include <cmath>

namespace argeo
{
	ClipmapUpdate::ClipmapUpdate(
		ClipmapLevel* level, 
		int west,
		int south, 
		int east, 
		int north)
		: m_level(level)
        , m_west(west)
        , m_south(south)
        , m_east(east)
        , m_north(north)
	{ }


	ClipmapUpdate::~ClipmapUpdate()
	{ }

	ClipmapLevel* ClipmapUpdate::get_level() const
	{
		return m_level;
	}

	int ClipmapUpdate::get_west() const
	{
		return m_west;
	}

	int ClipmapUpdate::get_south() const
	{
		return m_south;
	}

	int ClipmapUpdate::get_east() const
	{
		return m_east;
	}

	int ClipmapUpdate::get_north() const
	{
		return m_north;
	}

	int ClipmapUpdate::get_width() const
	{
		return get_east() - get_west() + 1;
	}

	int ClipmapUpdate::get_height() const
	{
		return get_north() - get_south() + 1;
	}

	/// <summary>
	/// Creates a new region which is equivalent to this one but with a one post buffer added
	/// around the perimeter.  The buffer will not cause the update region to exceed the bounds
	/// of the level's <see cref="ClipmapLevel.NextExtent"/>.
	/// </summary>
	/// <returns>The new region.</returns>
	ClipmapUpdate ClipmapUpdate::add_buffer_within_level_next_extent() const
	{
		return ClipmapUpdate(
			m_level,
			fmax(m_west - 1,  m_level->get_next_extent().west),
			fmax(m_south - 1, m_level->get_next_extent().south),
			fmin(m_east + 1,  m_level->get_next_extent().east),
			fmin(m_north + 1, m_level->get_next_extent().north));
	}
}	