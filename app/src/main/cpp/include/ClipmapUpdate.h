#pragma once

#include "ClipmapLevel.h"
#include <memory>

namespace argeo
{
	class ClipmapUpdate
	{
	public:
		ClipmapUpdate(
			ClipmapLevel* level, 
			int west, 
			int south,
			int east,
			int north);

		~ClipmapUpdate();

		ClipmapLevel* get_level() const;

		int get_west()  const;
		int get_south() const;
		int get_east()  const;
		int get_north() const;

		int get_width()  const;
		int get_height() const;

        /// <summary>
        /// Creates a new region which is equivalent to this one but with a one post buffer added
        /// around the perimeter.  The buffer will not cause the update region to exceed the bounds
        /// of the level's <see cref="ClipmapLevel.NextExtent"/>.
        /// </summary>
        /// <returns>The new region.</returns>
		ClipmapUpdate add_buffer_within_level_next_extent() const;

	private:
		ClipmapLevel* m_level;

		int m_west;
		int m_south;
		int m_east;
		int m_north;
	};
}
