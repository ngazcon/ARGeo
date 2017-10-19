#pragma once

#include "RasterLevel.h"
#include "Texture2D.h"
#include "Vector.h"
#include "BoundingVolume.h"
#include "SceneMode.h"
#include "Ray.h"
#include "MapProjection.h"

#include <memory>

namespace argeo
{
	struct Extent
	{
	public:

		Extent(
            const unsigned int&  west  = 0,
            const unsigned int&  south = 0,
            const unsigned int&  east  = 0,
            const unsigned int&  north = 0)
			: west(west)
			, south(south)
			, east(east)
			, north(north)
		{ }

        unsigned int west;
        unsigned int south;
        unsigned int east;
        unsigned int north;

        inline bool operator!=(const Extent& right)
        {
            return east != right.east ||
                   west != right.west ||
                   north != right.north ||
                   south != right.south;
        }
	};

	class ClipmapLevel
	{
	public:
		ClipmapLevel();
		~ClipmapLevel();

		vec2i get_origin_in_textures() const;
		vec2i get_origin_in_imagery()  const;

		Extent get_current_extent() const;
		Extent get_current_imagery_extent() const;

		Extent get_next_extent() const;
		Extent get_next_imagery_extent() const;

		RasterLevel* get_terrain() const;
		RasterLevel* get_imagery() const;
		
		ClipmapLevel* get_finer_level() const;
		ClipmapLevel* get_coarser_level() const;

		Texture2D* get_height_texture() const;
        Texture2D* get_normal_texture() const;
		Texture2D* get_imagery_texture() const;

		bool get_offset_strip_on_north() const;
		bool get_offset_strip_on_east() const;

		unsigned int get_imagery_width() const;
        unsigned int get_imagery_height() const;

		void set_terrain(RasterLevel* value);
		void set_imagery(RasterLevel* value);

		void set_origin_in_imagery(vec2i value);
		void set_origin_in_textures(vec2i value);

		void set_height_texture(std::unique_ptr<Texture2D> value);

		void set_normal_texture(std::unique_ptr<Texture2D> value);

		void set_imagery_texture(std::unique_ptr<Texture2D> value);

		void set_finer_level(ClipmapLevel* value);
		void set_coarser_level(ClipmapLevel* value);

		void set_imagery_height(const unsigned int& value);
		void set_imagery_width(const unsigned int&  value);

		void set_current_extent(const Extent& value);
		void set_current_imagery_extent(const Extent& value);

		void set_next_extent(const Extent& value);
		void set_next_imagery_extent(const Extent& value);

		void set_offset_strip_on_east(const bool& value);
		void set_offset_strip_on_north(const bool& value);

		void set_minimum_height(const float& value);
		void set_maximum_height(const float& value);

		float get_minimum_height() const;
		float get_maximum_height() const;

        GeodeticExtent get_geodetic_extent() const;
        vec3d  get_center();
		double get_width();

        BoundingVolume* get_bounding_volume();

        vec3d pick(
                const Ray& ray,
                const SceneMode& mode,
                MapProjection* projection,
                const bool& cull_back_faces);

    private:

        vec3d get_position(
                const SceneMode& mode,
                MapProjection* projection,
                const unsigned int& row,
                const unsigned int& col,
                vec4ub* heights);

        void clean();

	private:
		RasterLevel* m_terrain;
		RasterLevel* m_imagery;
		
		ClipmapLevel* m_finer_level;
		ClipmapLevel* m_coarser_level;
		
		std::unique_ptr<Texture2D> m_height_texture;
        std::unique_ptr<Texture2D> m_normal_texture;
		std::unique_ptr<Texture2D> m_imagery_texture;

		vec2i m_origin_in_textures;
		vec2i m_origin_in_imagery;

		bool m_offset_strip_on_north;
		bool m_offset_strip_on_east;

		Extent m_current_extent;
		Extent m_next_extent;

		Extent m_current_imagery_extent;
		Extent m_next_imagery_extent;

		int m_imagery_width;
		int m_imagery_height;

		float m_maximum_height;
		float m_minimum_height;

        float m_width;
        vec3d m_center;

        bool m_dirty;
		std::unique_ptr<BoundingVolume> m_bounding_volume;
	};
}
