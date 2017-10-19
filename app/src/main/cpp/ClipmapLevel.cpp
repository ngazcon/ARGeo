#include <EllipsoidTransformations.h>
#include <Device.h>
#include <TextureHelper.h>
#include "IntersectionAlgorithms.h"
#include "OrientedBoundingBox.h"
#include "BoundingSphere.h"
#include "ClipmapLevel.h"
#include "RasterSource.h"

namespace argeo
{
	ClipmapLevel::ClipmapLevel()
		: m_current_extent(1, 1, 0, 0)
		, m_current_imagery_extent(1, 1, 0, 0)
		, m_terrain(nullptr)
		, m_imagery(nullptr)
        , m_normal_texture(nullptr)
		, m_height_texture(nullptr)
		, m_imagery_texture(nullptr)
		, m_coarser_level(nullptr)
		, m_finer_level(nullptr)
		, m_maximum_height(-std::numeric_limits<float>::max())
	    , m_minimum_height(std::numeric_limits<float>::max())
        , m_width(0)
		, m_dirty(true)
	{ }


	ClipmapLevel::~ClipmapLevel()
	{ }

    double ClipmapLevel::get_width()
    {
        clean();
        return m_width;
    }

	vec2i ClipmapLevel::get_origin_in_textures() const
	{
		return m_origin_in_textures;
	}

	vec2i ClipmapLevel::get_origin_in_imagery() const
	{
		return m_origin_in_imagery;
	}

	Extent ClipmapLevel::get_current_extent() const
	{
		return m_current_extent;
	}

	Extent ClipmapLevel::get_current_imagery_extent() const
	{
		return m_current_imagery_extent;
	}

	Extent ClipmapLevel::get_next_extent() const
	{
		return m_next_extent;
	}

	Extent ClipmapLevel::get_next_imagery_extent() const
	{
		return m_next_imagery_extent;
	}

	RasterLevel* ClipmapLevel::get_terrain() const
	{
		return m_terrain;
	}

	RasterLevel* ClipmapLevel::get_imagery() const
	{
		return m_imagery;
	}

	ClipmapLevel* ClipmapLevel::get_finer_level() const
	{
		return m_finer_level;
	}

	ClipmapLevel* ClipmapLevel::get_coarser_level() const
	{
		return m_coarser_level;
	}

	Texture2D* ClipmapLevel::get_height_texture() const
	{
		return m_height_texture.get();
	}

	Texture2D* ClipmapLevel::get_normal_texture() const
	{
		return m_normal_texture.get();
	}

	Texture2D* ClipmapLevel::get_imagery_texture() const
	{
		return m_imagery_texture.get();
	}

	bool ClipmapLevel::get_offset_strip_on_north() const
	{
		return m_offset_strip_on_north;
	}

	bool ClipmapLevel::get_offset_strip_on_east() const
	{
		return m_offset_strip_on_east;
	}

	unsigned int ClipmapLevel::get_imagery_width() const
	{
		return m_imagery_width;
	}

    unsigned int ClipmapLevel::get_imagery_height() const
	{
		return m_imagery_height;
	}

	void ClipmapLevel::set_terrain(RasterLevel* value)
	{
		m_terrain = value;
	}

	void ClipmapLevel::set_imagery(RasterLevel* value)
	{
		m_imagery = value;
	}

	void ClipmapLevel::set_origin_in_imagery(vec2i value)
	{
		m_origin_in_imagery = value;
	}

	void ClipmapLevel::set_origin_in_textures(vec2i value)
	{
		m_origin_in_textures = value;
	}

	void ClipmapLevel::set_height_texture(std::unique_ptr<Texture2D> value)
	{
		m_height_texture = std::move(value);
	}

	void ClipmapLevel::set_normal_texture(std::unique_ptr<Texture2D> value)
	{
		m_normal_texture = std::move(value);
	}

	void ClipmapLevel::set_imagery_texture(std::unique_ptr<Texture2D> value)
	{
		m_imagery_texture = std::move(value);
	}

	void ClipmapLevel::set_finer_level(ClipmapLevel* value)
	{
		m_finer_level = value;
	}

	void ClipmapLevel::set_coarser_level(ClipmapLevel* value)
	{
		m_coarser_level = value;
	}

	void ClipmapLevel::set_imagery_height(const unsigned int& value)
	{
		m_imagery_height = value;
	}

	void ClipmapLevel::set_imagery_width(const unsigned int& value)
	{
		m_imagery_width = value;
	}

	void ClipmapLevel::set_current_extent(const Extent& value)
	{
        if (m_current_extent != value)
        {
            m_current_extent = value;
            m_dirty = true;
        }
	}

	void ClipmapLevel::set_next_extent(const Extent& value)
	{
		m_next_extent = value;
	}

	void ClipmapLevel::set_next_imagery_extent(const Extent& value)
	{
		m_next_imagery_extent = value;
	}

	void ClipmapLevel::set_current_imagery_extent(const Extent& value)
	{
		m_current_imagery_extent = value;
	}

	void ClipmapLevel::set_offset_strip_on_east(const bool& value)
	{
		m_offset_strip_on_east = value;
	}

	void ClipmapLevel::set_offset_strip_on_north(const bool& value)
	{
		m_offset_strip_on_north = value;
	}

	void ClipmapLevel::set_minimum_height(const float& value)
	{
        if (m_minimum_height != value)
        {
            m_minimum_height = value;
            m_dirty = true;
        }
	}

	void ClipmapLevel::set_maximum_height(const float& value)
	{
        if (m_maximum_height != value)
        {
            m_maximum_height = value;
            m_dirty = true;
        }
	}

	float ClipmapLevel::get_minimum_height() const
	{
		return m_minimum_height;
	}

	float ClipmapLevel::get_maximum_height() const
	{
		return m_maximum_height;
	}

    BoundingVolume* ClipmapLevel::get_bounding_volume()
    {
        clean();
        return m_bounding_volume.get();
    }

    vec3d ClipmapLevel::pick(
            const Ray &ray,
            const SceneMode &mode,
            MapProjection *projection,
            const bool &cull_back_faces)
    {
        const unsigned int cols = get_height_texture()->get_description().get_width();
        const unsigned int rows = get_height_texture()->get_description().get_height();

        std::unique_ptr<ReadPixelBuffer> read_buffer =
                m_height_texture->copy_to_buffer(ImageFormat::RedGreenBlueAlpha, ImageDataType::UnsignedByte);

        vec4ub* heights = read_buffer->map_range<vec4ub>(
                0,
                read_buffer->get_size_in_bytes(),
                BufferAccess::ReadBit | BufferAccess::CoherentBit);

        for (unsigned int row = 0; row < rows - 1; row++)
        {
            for (unsigned int col = 0; col < cols - 1; col++)
            {
                vec3d v0 = get_position(mode, projection, row + 0, col + 0, heights);
                vec3d v1 = get_position(mode, projection, row + 0, col + 1, heights);
                vec3d v2 = get_position(mode, projection, row + 1, col + 1, heights);

                vec3d intersection = IntersectionAlgorithms::ray_triangle(
                        ray,
                        v0,
                        v1,
                        v2,
                        cull_back_faces);

                if (!vec_undefined(intersection))
                {
                    return intersection;
                }

                v0 = get_position(mode, projection, row + 0, col + 0, heights);
                v1 = get_position(mode, projection, row + 1, col + 1, heights);
                v2 = get_position(mode, projection, row + 1, col + 0, heights);

                intersection = IntersectionAlgorithms::ray_triangle(
                        ray,
                        v0,
                        v1,
                        v2,
                        cull_back_faces);

                if (!vec_undefined(intersection))
                {
                    return intersection;
                }
            }
        }

        read_buffer->unmap();
        return vec3d::undefined();
    }

	vec3d ClipmapLevel::get_center()
	{
        clean();
        return m_center;
	}

    GeodeticExtent ClipmapLevel::get_geodetic_extent() const
    {
        return GeodeticExtent(
                ArgeoMath::to_radians(m_terrain->index_to_longitude(m_current_extent.west)),
                ArgeoMath::to_radians(m_terrain->index_to_latitude(m_current_extent.south)),
                ArgeoMath::to_radians(m_terrain->index_to_longitude(m_current_extent.east)),
                ArgeoMath::to_radians(m_terrain->index_to_latitude(m_current_extent.north)));
    }

    vec3d ClipmapLevel::get_position(
            const SceneMode &mode,
            MapProjection *projection,
            const unsigned int& row,
            const unsigned int& col,
            vec4ub* heights)
    {
        const double longitude = ArgeoMath::to_radians(m_terrain->index_to_longitude(col + m_current_extent.west));
        const double latitude  = ArgeoMath::to_radians(m_terrain->index_to_latitude(row  + m_current_extent.south));

        const int corrected_column = static_cast<int>(col + m_origin_in_textures.x) % m_height_texture->get_description().get_width();
        const int corrected_row    = static_cast<int>(row + m_origin_in_textures.y) % m_height_texture->get_description().get_height();

        const unsigned int index =
                corrected_row * m_height_texture->get_description().get_width() + corrected_column;

        const double height = ArgeoMath::denormalize_float(
                ArgeoMath::float_from_vec4(heights[index]),
                m_terrain->get_source()->get_maximum_height(),
                m_terrain->get_source()->get_minimum_height());

        Ellipsoid ellipsoid = projection->get_ellipsoid();
        Geodetic3D geodetic = Geodetic3D(
                longitude,
                latitude,
                height);

        vec3d result;
		// TODO: TO ENU
        if (mode != SceneMode::Scene3D)
        {
            result = projection->project(geodetic).to_vec3d();
            result = vec3d(
                    result.z,
                    result.x,
                    result.y);
        }
        else
        {
            result = ellipsoid.to_geocentric3D(geodetic).to_vec3d();
        }

        return result;
    }

    void ClipmapLevel::clean()
    {
        if (m_dirty)
        {
            m_bounding_volume = BoundingSphere::from_rectangle_3D(
                   get_geodetic_extent(),
                   Ellipsoid::wgs84(),
                   m_maximum_height);

            m_width  = static_cast<BoundingSphere*>(m_bounding_volume.get())->get_radius() * 2;
            m_center = static_cast<BoundingSphere*>(m_bounding_volume.get())->get_center();
            m_dirty  = false;
        }
    }
}