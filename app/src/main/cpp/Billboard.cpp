#include "Billboard.h"
#include "BillboardCollection.h"

#include "guid.h"
#include "android/include/JNIHelper.h"

#include <algorithm>
#include <SceneTransforms.h>

namespace argeo
{
	unsigned int Billboard::IMAGE_WIDTH			    = -1;
	unsigned int Billboard::IMAGE_HEIGHT		    = -1;

	unsigned int Billboard::SHOW_INDEX			    =  0;
	unsigned int Billboard::WIDTH_INDEX			    =  1;
	unsigned int Billboard::HEIGHT_INDEX		    =  2;
	unsigned int Billboard::IMAGE_INDEX			    =  3;
	unsigned int Billboard::POSITION_INDEX		    =  4;
	unsigned int Billboard::COLOR_INDEX			    =  5;
	unsigned int Billboard::PIXEL_OFFSET_INDEX      =  6;
	unsigned int Billboard::HORIZONTAL_ORIGIN_INDEX =  7;
	unsigned int Billboard::VERTICAL_ORIGIN_INDEX   =  8;
    unsigned int Billboard::SCALE_INDEX             =  9;
	unsigned int Billboard::NUMBER_OF_PROPERTIES    =  10;

	Billboard::Billboard(
		BillboardCollection* collection,
		const std::string& image_path,
		const std::string& image_id,
		const vec3d& position,
		const vec2i& pixel_offset,
		const unsigned int& width,
		const unsigned int& height,
        const HeightClamping& height_clamping)
		: m_color(ColorHelper::white_color())
		, m_vertex_buffer_offset(0)
		, m_dirty(false)
		, m_collection(collection)
		, m_image_index(INDEX_NOT_DEFINED)
		, m_origins(0)
		, m_pixel_offset(pixel_offset)
		, m_position(position)
        , m_actual_position(position)
        , m_actual_clamped_position(vec3d::undefined())
		, m_image_path(image_path)
		, m_image_id(image_id)
		, m_width(width)
		, m_height(height)
		, m_image(nullptr)
		, m_pick_id(nullptr)
	    , m_mode(SceneMode::Scene3D)
        , m_size_in_meters(false)
        , m_scale(1.0)
        , m_height_clamping(height_clamping)
        , m_remove_update_height_callback(nullptr)
	{
		if (image_id.empty())
		{
			m_image_id = image_path;
		}

		if (collection->get_texture_atlas() != nullptr)
		{
			load_image();
		}

        update_clamping();
	}

	Billboard::Billboard(
		BillboardCollection* collection,
		Bitmap* image,
		const std::string& image_id,
		const vec3d& position,
		const vec2i& pixel_offset,
		const unsigned int& width,
		const unsigned int& height,
        const HeightClamping& height_clamping)
		: m_color(ColorHelper::white_color())
		, m_vertex_buffer_offset(0)
		, m_dirty(false)
		, m_collection(collection)
		, m_image_index(INDEX_NOT_DEFINED)
		, m_origins(0)
		, m_pixel_offset(pixel_offset)
		, m_position(position)
		, m_image(image)
		, m_image_id(image_id)
		, m_width(width)
		, m_height(height)
		, m_pick_id(nullptr)
		, m_image_path(std::string())
        , m_remove_update_height_callback(nullptr)
        , m_height_clamping(height_clamping)
	{
		if (image_id.empty())
		{
			std::ostringstream stream;

#ifdef __ANDROID__
			JNIEnv* jni;
			jint status = JNIHelper::get_instance().attach_current_thread(&jni);
			GuidGenerator generator(jni);
			stream << generator.newGuid();
			JNIHelper::get_instance().detach_current_thread(status);
#else
			GuidGenerator generator;
			stream << generator.newGuid();
#endif

			m_image_id = stream.str();
		}

		if (collection->get_texture_atlas() != nullptr)
		{
			load_image();
		}

        update_clamping();
	}


	Billboard::~Billboard()
	{ }

	void Billboard::load_image()
	{
		TextureAtlas* atlas = m_collection->get_texture_atlas();
		Defer image_index_future;
		
		if (m_image != nullptr)
		{
			image_index_future = atlas->add_image(m_image_id, m_image);
		}
		else if (!m_image_path.empty())
		{
			image_index_future = atlas->add_image(m_image_id, m_image_path);
		}

		image_index_future.then([&](const unsigned int& index)
		{
			m_image_index = index;
			make_dirty(Billboard::IMAGE_INDEX);
		});
	}

	PickId* Billboard::get_pick_id(Context* context)
	{
		if (m_pick_id == nullptr)
		{
			m_pick_id = context->create_pick_id(this);
		}
		return m_pick_id.get();
	}

	void Billboard::make_dirty(const unsigned int& index)
	{
		if (m_collection != nullptr)
		{
			m_collection->notify_dirty(
				this,
				index);
			m_dirty = true;
		}
	}

	unsigned int Billboard::get_image_index() const
	{
		return m_image_index;
	}

	void Billboard::set_vertex_buffer_offset(const unsigned int& value)
	{
		m_vertex_buffer_offset = value;
	}

	unsigned int Billboard::get_vertex_buffer_offset() const
	{
		return m_vertex_buffer_offset;
	}

	void Billboard::set_width(const unsigned int& value)
	{
		if (m_width != value)
		{
			m_width = value;
			make_dirty(Billboard::WIDTH_INDEX);
		}
	}

	unsigned int Billboard::get_width() const
	{
		return m_width;
	}

	void Billboard::set_height(const unsigned int& value)
	{
		if (m_height != value)
		{
			m_height = value;
			make_dirty(Billboard::HEIGHT_INDEX);
		}
	}

	unsigned int Billboard::get_height() const
	{
		return m_height;
	}

        vec3d Billboard::compute_actual_position(
            const vec3d &position,
            FrameState *frame_state,
            const mat4d &model_matrix)
    {
        if (!vec_undefined(get_clamped_position()))
        {
            if (frame_state->get_mode() != m_mode)
            {
                update_clamping();
            }
            return get_clamped_position();
        }
        else if (frame_state->get_mode() == SceneMode::Scene3D)
        {
            return position;
        }

        vec3d pos = mat_multiply_by_point(model_matrix, position);
        return SceneTransforms::compute_actual_wgs84_position(pos, frame_state);
    }

    void Billboard::set_clamped_position(const vec3d& value)
    {
        m_actual_clamped_position = value;
        make_dirty(POSITION_INDEX);
    }

    vec3d Billboard::get_clamped_position() const
    {
        return m_actual_clamped_position;
    }

    vec3d Billboard::get_actual_position() const
    {
        return vec_undefined(get_clamped_position()) ?
               m_actual_position :
               get_clamped_position();
    }

    void Billboard::set_actual_position(const vec3d& value)
    {
        if (vec_undefined(get_clamped_position()))
        {
            m_actual_position = value;
        }

        make_dirty(POSITION_INDEX);
    }

	vec3d Billboard::get_position() const
	{
		return m_position;
	}
	
	void Billboard::set_position(const vec3d& value)
	{
		if (m_position != value)
		{
            m_actual_position = value;
			m_position = value;

            update_clamping();
			make_dirty(Billboard::POSITION_INDEX);
		}
	}

	Color Billboard::get_color() const
	{
		return m_color; 
	}

	void Billboard::set_color(Color value)
	{
		if (m_color != value)
		{
			m_color = value;
			make_dirty(Billboard::COLOR_INDEX);
		}
	}

	HorizontalOrigin Billboard::get_horizontal_origin() const
	{
		return (HorizontalOrigin)((m_origins & m_horizontal_origin_mask) >> m_vertical_origin_shift);
	}


	void Billboard::set_horizontal_origin(const HorizontalOrigin& value)
	{
		if (get_horizontal_origin() != value)
		{
			m_origins &= ~m_horizontal_origin_mask;
			m_origins |= (((uint8_t)value << m_vertical_origin_shift) & m_horizontal_origin_mask);
			make_dirty(Billboard::HORIZONTAL_ORIGIN_INDEX);
		}
	}

	VerticalOrigin Billboard::get_vertical_origin() const
	{
		return (VerticalOrigin)((m_origins & m_vertical_origin_mask) >> m_vertical_origin_shift); 
	}


	void Billboard::set_vertical_origin(const VerticalOrigin& value)
	{
		if (get_vertical_origin() != value)
		{
			m_origins &= ~m_vertical_origin_mask;
			m_origins |= (((unsigned int)value << m_vertical_origin_shift) & m_vertical_origin_mask);
			make_dirty(Billboard::VERTICAL_ORIGIN_INDEX);
		}
	}

	vec2i Billboard::get_pixel_offset() const
	{
		return m_pixel_offset; 
	}


	void Billboard::set_pixel_offset(const vec2i& value)
	{
		if (m_pixel_offset != value)
		{
			m_pixel_offset = value;
			make_dirty(Billboard::PIXEL_OFFSET_INDEX);
		}
	}

    bool Billboard::get_size_in_meters() const
    {
        return m_size_in_meters;
    }

    void Billboard::set_size_in_meters(const bool &value)
    {
        if (m_size_in_meters != value)
        {
            m_size_in_meters = value;
            make_dirty(COLOR_INDEX);
        }
    }

    double Billboard::get_scale() const
    {
        return m_scale;
    }

    void Billboard::set_scale(const double &value)
    {
        if (m_scale != value)
        {
            m_scale = value;
            make_dirty(SCALE_INDEX);
        }
    }

    void Billboard::set_height_clamping(const HeightClamping &value)
    {
        if (m_height_clamping != value)
        {
            m_height_clamping = value;
            update_clamping();
            make_dirty(POSITION_INDEX);
        }
    }

    HeightClamping Billboard::get_height_clamping() const
    {
        return m_height_clamping;
    }


	void Billboard::set_group(BillboardCollection* value)
	{
		m_collection = value;
	}

	BillboardCollection* Billboard::get_group()
	{
		return m_collection;
	}

    void Billboard::update_clamping()
    {
        Scene* scene = m_collection->get_scene();
        ITerrain* terrain = scene->get_terrain();
        Ellipsoid ellipsoid = terrain->get_ellipsoid();

        SceneMode mode = scene->get_mode();
        bool mode_changed = mode != m_mode;
        m_mode = mode;

        if ((m_height_clamping == HeightClamping::None || mode_changed) &&
            (m_remove_update_height_callback != nullptr))
        {
            m_remove_update_height_callback();
            m_remove_update_height_callback = nullptr;
            set_clamped_position(vec3d::undefined());
        }

        if (m_height_clamping == HeightClamping::None || vec_undefined(m_position))
        {
            return;
        }

        Geodetic3D position = ellipsoid.to_geodetic3D(m_position);
        if (undefined(position))
        {
            m_actual_clamped_position = vec3d::undefined();
            return;
        }

        if (m_remove_update_height_callback != nullptr)
        {
            m_remove_update_height_callback();
        }

        m_update_height_function = [&, position](vec3d& clamped_position)
        {
            if (m_height_clamping == HeightClamping::Ground)
            {
                if (m_mode == SceneMode::Scene3D)
                {
                    Geodetic3D clamped_geodetic = ellipsoid.to_geodetic3D(clamped_position);
                    clamped_geodetic = Geodetic3D(
                            clamped_geodetic.get_longitude(),
                            clamped_geodetic.get_latitude(),
                            clamped_geodetic.get_height() + position.get_height());
                    clamped_position = ellipsoid.to_geocentric3D(clamped_geodetic).to_vec3d();
                }
                else
                {
                    clamped_position.x += position.get_height();
                }
            }
            set_clamped_position(clamped_position);
        };

        m_remove_update_height_callback = terrain->get_raster_source()->add_listener(
                m_update_height_function,
                position);

        Geodetic3D geodetic = position;
        const double height = terrain->get_point_height(Geodetic2D(
                position.get_longitude(),
                position.get_latitude()));

        if (!std::isnan(height))
        {
            geodetic = Geodetic3D(
                    geodetic.get_longitude(),
                    geodetic.get_latitude(),
                    height);
        }

        vec3d clamped_position = ellipsoid.to_geocentric3D(geodetic).to_vec3d();
        m_update_height_function(clamped_position);
    }

	void Billboard::clean()
	{
		m_dirty = false;
	}
}