#include "BillboardCollection.h"

#include "Device.h"
#include "BillboardShaders.h"
#include "UniformSampler.h"

#include <algorithm>

namespace argeo
{
	const ShaderVertexAttributeLocations BillboardCollection::attribute_locations =
	{
		{ "positionHigh",					 0 },
		{ "positionLow",					 1 },
		{ "textureCoordinatesAndImageSize",	 2 },
		{ "color",							 3 },
		{ "pickColor",						 4 },
		{ "origin",							 5 },
		{ "directionAndPixelOffset",		 6 },
	};

	BillboardCollection::BillboardCollection(
            Scene* scene,
		    const unsigned int& capacity)
		: m_show(true)
		, m_bounding_volume_dirty(false)
		, m_base_volume(new BoundingSphere())
        , m_base_volume_WC(new BoundingSphere())
        , m_base_volume_2D(new BoundingSphere())
		, m_draw_command(PrimitiveType::Triangles)
		, m_texture_atlas_guid("")
        , m_model_matrix(mat4d::identity())
        , m_actual_model_matrix(mat4d::identity())
        , m_max_size(0.0)
        , m_max_scale(1.0)
        , m_max_pixel_offset(0.0)
        , m_max_eye_offset(0.0)
        , m_all_sized_in_meters(true)
        , m_all_horizontal_center(true)
        , m_all_vertical_center(true)
        , m_scene(scene)
	{
		m_draw_command.set_pass(CommandPass::OverlayCommand);

		m_properties.resize(Billboard::NUMBER_OF_PROPERTIES, false);
		m_billboards.reserve(capacity);

		m_render_parameters.facet_culling = FacetCulling(false);
		m_render_parameters.blending =
			Blending(
				true,
				SourceBlendingFactor::SourceAlpha,
				SourceBlendingFactor::SourceAlpha,
				DestinationBlendingFactor::OneMinusSourceAlpha,
				DestinationBlendingFactor::OneMinusSourceAlpha);
		
		m_uniform_map["u_atlas"] = [&](IUniform* value)
		{
			static_cast<UniformSampler*>(value)->set_texture(
				m_render_resources.texture_atlas->get_texture());
		};
	}

	BillboardCollection::~BillboardCollection()
	{
		for (Billboard* b : m_billboards)
		{
			release_billboard(b);
		}
		dispose_vertex_array();
	}

	void BillboardCollection::create_vertex_array(Context* context)
	{
		m_render_resources.positions_buffer = Device::create_vertex_buffer(
			BufferHint::StaticDraw,
			m_billboards.size() * sizeof(vec3) * 4 * 2);

		m_render_resources.color_buffer = Device::create_vertex_buffer(
			BufferHint::StaticDraw,
			m_billboards.size() * sizeof(BlittableRGBA) * 4);

		m_render_resources.pick_color_buffer = Device::create_vertex_buffer(
			BufferHint::StaticDraw,
			m_billboards.size() * sizeof(BlittableRGBA) * 4);

		m_render_resources.direction_and_pixel_offset_buffer = Device::create_vertex_buffer(
			BufferHint::StaticDraw,
			m_billboards.size() * sizeof(vec4) * 4);

		m_render_resources.texture_coordinates_and_image_size_buffer = Device::create_vertex_buffer(
			BufferHint::StaticDraw,
			m_billboards.size() * sizeof(vec4) * 4);

		m_render_resources.origin_buffer = Device::create_vertex_buffer(
			BufferHint::StaticDraw,
			m_billboards.size() * 4);

		VertexBufferAttribute* position_high_attribute = new VertexBufferAttribute(
			m_render_resources.positions_buffer.get(),
			ComponentDataType::Float,
			3,
			false,
			0,
			2 * sizeof(vec3));

		VertexBufferAttribute* position_low_attribute = new VertexBufferAttribute(
			m_render_resources.positions_buffer.get(),
			ComponentDataType::Float,
			3,
			false,
			sizeof(vec3),
			2 * sizeof(vec3));

		VertexBufferAttribute* color_attribute = new VertexBufferAttribute(
			m_render_resources.color_buffer.get(),
			ComponentDataType::UnsignedByte,
			4,
			true,
			0,
			0);

		VertexBufferAttribute* pick_color_attribute = new VertexBufferAttribute(
			m_render_resources.pick_color_buffer.get(),
			ComponentDataType::UnsignedByte,
			4,
			true,
			0,
			0);

		VertexBufferAttribute* origin_attribute = new VertexBufferAttribute(
			m_render_resources.origin_buffer.get(),
			ComponentDataType::UnsignedByte,
			1);

		VertexBufferAttribute* direction_and_pixel_offset_attribute = new VertexBufferAttribute(
			m_render_resources.direction_and_pixel_offset_buffer.get(),
			ComponentDataType::Float,
			4);

		VertexBufferAttribute* texture_coordinates_and_image_size_attribute = new VertexBufferAttribute(
			m_render_resources.texture_coordinates_and_image_size_buffer.get(),
			ComponentDataType::Float,
			4);

		std::vector<unsigned int> indices;
		for (unsigned int i = 0; i < m_billboards.size(); i++)
		{
			indices.push_back(i * 4 + 0);
			indices.push_back(i * 4 + 1);
			indices.push_back(i * 4 + 2);
			indices.push_back(i * 4 + 2);
			indices.push_back(i * 4 + 3);
			indices.push_back(i * 4 + 0);
		}

		m_render_resources.index_buffer = Device::create_index_buffer(
			BufferHint::StaticDraw,
			indices.size() * sizeof(unsigned int));

		m_render_resources.index_buffer->copy_from_system<unsigned int>(indices);

		// Create the VAO.
		m_render_resources.vertex_array = context->create_vertex_array(
			VertexBufferAttributes
		{
			{ attribute_locations.at("positionHigh"),					position_high_attribute },
			{ attribute_locations.at("positionLow"),					position_low_attribute },
			{ attribute_locations.at("textureCoordinatesAndImageSize"), texture_coordinates_and_image_size_attribute },
			{ attribute_locations.at("color"),							color_attribute },
			{ attribute_locations.at("pickColor"),						pick_color_attribute },
			{ attribute_locations.at("origin"),							origin_attribute },
			{ attribute_locations.at("directionAndPixelOffset"),        direction_and_pixel_offset_attribute },
		},
		m_render_resources.index_buffer.get());
		m_render_resources.vertex_array->set_dispose_buffers(true);
	}


	void BillboardCollection::dispose_vertex_array()
	{
		m_render_resources.positions_buffer.reset(nullptr);
		m_render_resources.texture_coordinates_and_image_size_buffer.reset(nullptr);
		m_render_resources.color_buffer.reset(nullptr);
		m_render_resources.origin_buffer.reset(nullptr);
		m_render_resources.direction_and_pixel_offset_buffer.reset(nullptr);
		m_render_resources.vertex_array.reset(nullptr);
	}

	void BillboardCollection::update(FrameState* frame_state)
	{
		Context* context = frame_state->get_context();

		if (m_render_resources.texture_atlas == nullptr)
		{
			m_render_resources.texture_atlas =
				std::unique_ptr<TextureAtlas>(new TextureAtlas(context));

			for (Billboard* billboard : m_billboards)
			{
				billboard->load_image();
			}
		}

		if (m_render_resources.texture_atlas->get_texture_coordinates().size() == 0)
		{
			// Can't write billboard vertices until we have texture coordinates
			// provided by a texture atlas.
			return;
		}

        update_mode(frame_state);

		bool create_vertex_array = 
			m_render_resources.create_vertex_array ||
			m_texture_atlas_guid.compare(m_render_resources.texture_atlas->get_guid()) != 0;
		m_texture_atlas_guid = m_render_resources.texture_atlas->get_guid();

		if (create_vertex_array)
		{
			m_render_resources.create_vertex_array = false;
			update_all(context);
		}
		else if (m_dirty_billboards.size() != 0)
		{
			update_dirty(context);
		}

        if (m_bounding_volume_dirty)
        {
            m_bounding_volume_dirty = false;
            m_base_volume_WC.reset(
                    static_cast<BoundingSphere*>(m_base_volume->transform(m_model_matrix).get()));
        }

        BoundingSphere* bounding_volume = nullptr;
        mat4d model_matrix = mat4d::identity();
        if (frame_state->get_mode() == SceneMode::Scene3D)
        {
            model_matrix    = m_model_matrix;
            bounding_volume = static_cast<BoundingSphere*>(m_base_volume_WC->clone());
        }
        else
        {
            bounding_volume = static_cast<BoundingSphere*>(m_base_volume_2D->clone());
        }

        update_bounding_volume(bounding_volume, frame_state);

		m_dirty_billboards.clear();
		std::replace(
			m_properties.begin(),
			m_properties.end(),
			true,
			false);

		Pass pass = frame_state->get_pass();
		if (pass == Pass::Render)
		{
			if (m_render_resources.shader_program == nullptr)
			{
				m_render_resources.shader_program = context->get_shader_cache()->get_shader_program(
					billboard_vs,
					billboard_fs,
					attribute_locations);
			}

			if (m_render_resources.render_state == nullptr)
			{
				m_render_resources.render_state =
					RenderState::from_cache(m_render_parameters);
			}

			if (m_show)
			{
				m_draw_command.set_shader_program(m_render_resources.shader_program);
				m_draw_command.set_vertex_array(m_render_resources.vertex_array.get());
				m_draw_command.set_render_state(m_render_resources.render_state);
				m_draw_command.set_uniform_map(&m_uniform_map);
				m_draw_command.set_cull(true);
				m_draw_command.set_bounding_volume(m_base_volume.get());
                m_draw_command.set_model_matrix(model_matrix);

				if (m_render_resources.vertex_array.get() != nullptr)
				{
					frame_state->get_command_queue().push_back(&m_draw_command);
				}
			}
		}
		else if (pass == Pass::Pick)
		{
			if (m_render_resources.picking_shader_program == nullptr)
			{
				std::string vs = "#define RENDER_FOR_PICK 1";
				vs			  += billboard_vs;

				std::string fs = "#define RENDER_FOR_PICK 1";
				fs			  += billboard_fs;

				m_render_resources.picking_shader_program = context->get_shader_cache()->get_shader_program(
					vs,
					fs,
					attribute_locations);
			}

			if (m_show)
			{
				m_draw_command.set_shader_program(m_render_resources.picking_shader_program);
				m_draw_command.set_vertex_array(m_render_resources.vertex_array.get());
				m_draw_command.set_render_state(m_render_resources.render_state);
				m_draw_command.set_uniform_map(&m_uniform_map);
				m_draw_command.set_cull(true);
				m_draw_command.set_bounding_volume(m_base_volume.get());

				if (m_render_resources.vertex_array.get() != nullptr)
				{
					frame_state->get_command_queue().push_back(&m_draw_command);
				}
			}
		}
	}

	void BillboardCollection::update_all(Context* context)
	{
		// Since billboards were added or removed, all billboards are 
		// rewritten so dirty billboards are automatically cleaned.
		m_dirty_billboards.clear();

		// Create vertex array with appropriately sized vertex buffers
		dispose_vertex_array();

		if (m_billboards.size() > 0)
		{
			create_vertex_array(context);

			std::vector<vec3> positions(m_billboards.size() * 4 * 2);
			std::vector<BlittableRGBA> colors(m_billboards.size() * 4);
			std::vector<BlittableRGBA> pick_colors(m_billboards.size() * 4);
			std::vector<unsigned char> origins(m_billboards.size() * 4);
			std::vector<vec4> texture_coordinates_and_image_sizes(m_billboards.size() * 4);
			std::vector<vec4> directions_and_pixel_offsets(m_billboards.size() * 4);
			
			for (int i = 0; i < m_billboards.size(); ++i)
			{
				Billboard* b = m_billboards[i];

				evec3d position(b->get_actual_position());

				positions[i * 8 + 0]  = positions[i * 8 + 2]  =
				positions[i * 8 + 4]  = positions[i * 8 + 6]  = position.high;
				positions[i * 8 + 1]  = positions[i * 8 + 3]  =
				positions[i * 8 + 5]  = positions[i * 8 + 7]  = position.low;
				
				directions_and_pixel_offsets[i * 4 + 0] = vec4(-1, -1, b->get_pixel_offset());
				directions_and_pixel_offsets[i * 4 + 1] = vec4( 1, -1, b->get_pixel_offset());
				directions_and_pixel_offsets[i * 4 + 2] = vec4( 1,  1, b->get_pixel_offset());
				directions_and_pixel_offsets[i * 4 + 3] = vec4(-1,  1, b->get_pixel_offset());

				unsigned int image_index = b->get_image_index();
				const RectangleD billboard_texture_coordinates = 
					m_render_resources.texture_atlas->get_texture_coordinates(image_index);

				int texture_height = m_render_resources.texture_atlas->get_texture()->get_description().get_height();
				double height = b->get_height() != Billboard::IMAGE_HEIGHT ?
					b->get_height() :
					billboard_texture_coordinates.get_height() * texture_height;
				double image_height = std::ceil(height * 0.5);

				int texture_width = m_render_resources.texture_atlas->get_texture()->get_description().get_width();
				double width = b->get_width() != Billboard::IMAGE_WIDTH ?
					b->get_width() :
					billboard_texture_coordinates.get_width() * texture_width;
				double image_width = std::ceil(width * 0.5);

				texture_coordinates_and_image_sizes[i * 4 + 0] = vec4(
					billboard_texture_coordinates.get_lower_left().x,
					billboard_texture_coordinates.get_lower_left().y,
					image_width,
					image_height);

				texture_coordinates_and_image_sizes[i * 4 + 1] = vec4(
					billboard_texture_coordinates.get_upper_right().x,
					billboard_texture_coordinates.get_lower_left().y,
					image_width,
					image_height);

				texture_coordinates_and_image_sizes[i * 4 + 2] = vec4(
					billboard_texture_coordinates.get_upper_right().x,
					billboard_texture_coordinates.get_upper_right().y,
					image_width,
					image_height);

				texture_coordinates_and_image_sizes[i * 4 + 3] = vec4(
					billboard_texture_coordinates.get_lower_left().x,
					billboard_texture_coordinates.get_upper_right().y,
					image_width,
					image_height);

				colors[i * 4 + 0] = colors[i * 4 + 1] =
				colors[i * 4 + 2] = colors[i * 4 + 3] =  b->get_color();

				pick_colors[i * 4 + 0] = pick_colors[i * 4 + 1] =
				pick_colors[i * 4 + 2] = pick_colors[i * 4 + 3]  = b->get_pick_id(context)->color;

				origins[i * 4 + 0] = origins[i * 4 + 1] =
				origins[i * 4 + 2] = origins[i * 4 + 3] = billboard_origin(b);

				b->set_vertex_buffer_offset(i * 4);

                // Expand the base bounding volume for 3D.
                if (m_mode == SceneMode::Scene3D)
                {
                    m_base_volume = m_base_volume->expand(b->get_actual_position());
                    m_bounding_volume_dirty = true;
                }

                m_all_sized_in_meters   &= b->get_size_in_meters();
                m_max_size               = ::fmax(m_max_size, image_height);
                m_max_scale              = ::fmax(m_max_scale, b->get_scale());
                m_all_horizontal_center &= b->get_horizontal_origin() == HorizontalOrigin::Center;
                m_all_vertical_center   &= b->get_vertical_origin() == VerticalOrigin::Center;
                m_max_pixel_offset       = ::fmax(m_max_pixel_offset, b->get_pixel_offset().x);
                m_max_pixel_offset       = ::fmax(m_max_pixel_offset, b->get_pixel_offset().y);

                b->clean();
			}

			copy_billboards_from_system_memory(
				positions, 
				colors, 
				pick_colors,
				origins, 
				texture_coordinates_and_image_sizes,
				directions_and_pixel_offsets,
				0,
				m_billboards.size() * 4);
		}
	}

	void BillboardCollection::update_dirty(Context* context)
	{
		std::vector<vec3> positions(m_dirty_billboards.size() * 4 * 2);
		std::vector<BlittableRGBA> colors(m_dirty_billboards.size() * 4);
		std::vector<BlittableRGBA> pick_colors(m_dirty_billboards.size() * 4);
		std::vector<unsigned char> origins(m_dirty_billboards.size() * 4);
		
		std::vector<vec4> texture_coordinates_and_image_sizes(m_dirty_billboards.size() * 4);
		std::vector<vec4> directions_and_pixel_offsets(m_dirty_billboards.size() * 4);

		int buffer_offset = m_dirty_billboards[0]->get_vertex_buffer_offset();
		int previous_buffer_offset = buffer_offset - 1;
		int length = 0;

		for (int i = 0; i < m_dirty_billboards.size(); ++i)
		{
			Billboard* b = m_dirty_billboards[i];

			if (previous_buffer_offset != (b->get_vertex_buffer_offset() - 1))
			{
				copy_billboards_from_system_memory(
					positions,
					colors,
					pick_colors,
					origins,
					texture_coordinates_and_image_sizes,
					directions_and_pixel_offsets,
					buffer_offset, 
					length * 4);

				buffer_offset = b->get_vertex_buffer_offset();
				length = 0;
			}

			evec3d position(b->get_actual_position());

			positions[length * 8 + 0]  = positions[length * 8 + 2] =
			positions[length * 8 + 4]  = positions[length * 8 + 6] = position.high;
			positions[length * 8 + 1]  = positions[length * 8 + 3] =
			positions[length * 8 + 5]  = positions[length * 8 + 7] = position.low;

			directions_and_pixel_offsets[length * 4 + 0] = vec4(-1, -1, b->get_pixel_offset());
			directions_and_pixel_offsets[length * 4 + 1] = vec4( 1, -1, b->get_pixel_offset());
			directions_and_pixel_offsets[length * 4 + 2] = vec4( 1,  1, b->get_pixel_offset());
			directions_and_pixel_offsets[length * 4 + 3] = vec4(-1,  1, b->get_pixel_offset());

			unsigned int image_index = b->get_image_index();
			const RectangleD billboard_texture_coordinates = 
				m_render_resources.texture_atlas->get_texture_coordinates(image_index);

			int texture_height	  = m_render_resources.texture_atlas->get_texture()->get_description().get_height();
			double height		  = b->get_height() != Billboard::IMAGE_HEIGHT ? 
				b->get_height() :
				billboard_texture_coordinates.get_height() * texture_height;
			double image_height  = std::ceil(height * 0.5);
			
			int texture_width   = m_render_resources.texture_atlas->get_texture()->get_description().get_width();
			double width		= b->get_width() != Billboard::IMAGE_WIDTH ? 
				b->get_width() :
				billboard_texture_coordinates.get_width() * texture_width;
			double image_width  = std::ceil(width * 0.5);

			texture_coordinates_and_image_sizes[length * 4 + 0] = vec4(
				billboard_texture_coordinates.get_lower_left().x,
				billboard_texture_coordinates.get_lower_left().y,
				image_width,
				image_height);

			texture_coordinates_and_image_sizes[length * 4 + 1] = vec4(
				billboard_texture_coordinates.get_upper_right().x,
				billboard_texture_coordinates.get_lower_left().y,
				image_width,
				image_height);

			texture_coordinates_and_image_sizes[length * 4 + 2] = vec4(
				billboard_texture_coordinates.get_upper_right().x,
				billboard_texture_coordinates.get_upper_right().y,
				image_width,
				image_height);

			texture_coordinates_and_image_sizes[length * 4 + 3] = vec4(
				billboard_texture_coordinates.get_lower_left().x,
				billboard_texture_coordinates.get_upper_right().y,
				image_width,
				image_height);

			colors[length * 4 + 0] = colors[length * 4 + 1] =
			colors[length * 4 + 2] = colors[length * 4 + 3] = BlittableRGBA(b->get_color());

			origins[length * 4 + 0] = origins[length * 4 + 1] =
			origins[length * 4 + 2] = origins[length * 4 + 3] = billboard_origin(b);

			pick_colors[length * 4 + 0] = pick_colors[length * 4 + 1] =
			pick_colors[length * 4 + 2] = pick_colors[length * 4 + 3] = b->get_pick_id(context)->color;

			++length;
			previous_buffer_offset = b->get_vertex_buffer_offset();

            // Expand the base bounding volume for 3D.
            if (m_mode == SceneMode::Scene3D)
            {
                m_base_volume = m_base_volume->expand(b->get_actual_position());
                m_bounding_volume_dirty = true;
            }

            m_all_sized_in_meters   &= b->get_size_in_meters();
            m_max_size               = std::max(m_max_size, image_height);
            m_max_scale              = std::max(m_max_scale, b->get_scale());
            m_all_horizontal_center &= b->get_horizontal_origin() == HorizontalOrigin::Center;
            m_all_vertical_center   &= b->get_vertical_origin() == VerticalOrigin::Center;
            m_max_pixel_offset       = ::fmax(m_max_pixel_offset, b->get_pixel_offset().x);
            m_max_pixel_offset       = ::fmax(m_max_pixel_offset, b->get_pixel_offset().y);

			b->clean();
		}

		copy_billboards_from_system_memory(
			positions,
			colors,
			pick_colors,
			origins,
			texture_coordinates_and_image_sizes,
			directions_and_pixel_offsets,
			buffer_offset,
			length * 4);

		m_dirty_billboards.clear();
	}

	void BillboardCollection::copy_billboards_from_system_memory(
		std::vector<vec3> positions,
		std::vector<BlittableRGBA> colors,
		std::vector<BlittableRGBA> pick_colors,
		std::vector<unsigned char> origins,
		std::vector<vec4> texture_coordinates_and_image_sizes,
		std::vector<vec4> directions_and_pixel_offsets,
		int buffer_offset,
		int length)
	{
		m_render_resources.positions_buffer->copy_from_system(
			positions,
			buffer_offset * sizeof(vec3) * 2,
			length * sizeof(vec3) * 2);

		m_render_resources.texture_coordinates_and_image_size_buffer->copy_from_system(
			texture_coordinates_and_image_sizes,
			buffer_offset * sizeof(vec4),
			length * sizeof(vec4));

		m_render_resources.direction_and_pixel_offset_buffer->copy_from_system(
			directions_and_pixel_offsets,
			buffer_offset * sizeof(vec4),
			length * sizeof(vec4));

		m_render_resources.color_buffer->copy_from_system(
			colors,
			buffer_offset * sizeof(BlittableRGBA),
			length * sizeof(BlittableRGBA));

		m_render_resources.pick_color_buffer->copy_from_system(
			pick_colors,
			buffer_offset * sizeof(BlittableRGBA),
			length * sizeof(BlittableRGBA));

		m_render_resources.origin_buffer->copy_from_system(
			origins,
			buffer_offset,
			length);		
	}

	void BillboardCollection::compute_bounding_volume()
	{
		std::vector<vec3d> positions;
		for (Billboard* billboard : m_billboards)
		{
			positions.push_back(billboard->get_position());
		}
		m_base_volume = BoundingSphere::from_points(positions);
	}

	unsigned char BillboardCollection::billboard_origin(Billboard* billboard)
	{
		return static_cast<unsigned char>(billboard->get_horizontal_origin()) |
			  (static_cast<unsigned char>(billboard->get_vertical_origin()) << 2);
	}

	bool BillboardCollection::get_show()
	{
		return m_show;
	}

	void BillboardCollection::set_show(bool value)
	{
		m_show = value;
	}

	bool BillboardCollection::get_depth_test_enabled()
	{
		return m_draw_command.get_render_state()->get_depth_test().enabled; 
	}

	void BillboardCollection::set_depth_test_enabled(bool value)
	{
		m_render_parameters.depth_test = DepthTest(value);
		m_draw_command.set_render_state(
			RenderState::from_cache(m_render_parameters));
	}

	bool BillboardCollection::get_depth_write()
	{
		return m_draw_command.get_render_state()->get_depth_mask();
	}

	void BillboardCollection::set_depth_write(bool value)
	{
		m_render_parameters.depth_mask = value;
		m_draw_command.set_render_state(
			RenderState::from_cache(m_render_parameters));
	}

	Billboard* BillboardCollection::add(
		const std::string& image_path,
		const std::string& image_id,
		const vec3d& position,
		const vec2i& pixel_offset,
		const unsigned int& width,
		const unsigned int& height)
	{
		Billboard* billboard = new Billboard(
			this,
			image_path,
			image_id,
			position,
			pixel_offset,
			width,
			height);

		m_billboards.push_back(billboard);
		m_render_resources.create_vertex_array = true;

		return billboard;
	}

	Billboard* BillboardCollection::add(
		Bitmap* image,
		const std::string& image_id,
		const vec3d& position,
		const vec2i& pixel_offset,
		const unsigned int& width,
		const unsigned int& height)
	{
		Billboard* billboard = new Billboard(
			this,
			image,
			image_id,
			position,
			pixel_offset,
			width,
			height);

		m_billboards.push_back(billboard);
		m_render_resources.create_vertex_array = true;

		return billboard;
	}

	bool BillboardCollection::remove(Billboard* item)
	{
		int size_before = m_billboards.size();

		m_billboards.erase(std::remove(
			m_billboards.begin(),
			m_billboards.end(),
			item),
			m_billboards.end());

		int size_after = m_billboards.size();

		bool removed = size_before > size_after;

		if (removed)
		{
			remove_billboard(item);
		}

		return removed;
	}

	int BillboardCollection::get_count()
	{
		return m_billboards.size();
	}

    Scene* BillboardCollection::get_scene()
    {
        return m_scene;
    }

	void BillboardCollection::clear()
	{
		m_billboards.clear();
		m_dirty_billboards.clear();
		m_render_resources.create_vertex_array = true;
	}

	bool BillboardCollection::contains(Billboard* item)
	{
		return std::find(
			m_billboards.begin(),
			m_billboards.end(),
			item) != m_billboards.end();
	}

	void BillboardCollection::copy_to(std::vector<Billboard*> array, int array_index)
	{
		array.insert(
			array.begin() + array_index,
			m_billboards.begin(),
			m_billboards.end());
	}


	void BillboardCollection::add_billboard(Billboard* billboard)
	{
		if (billboard == nullptr)
		{
			throw std::invalid_argument("billboard");
		}

		if (billboard->get_group() != nullptr)
		{
			if (billboard->get_group() != this)
			{
				throw std::invalid_argument("billboard is already in another BillboardCollection.");
			}
			else
			{
				throw std::invalid_argument("billboard was already added to this BillboardCollection.");
			}
		}

		m_render_resources.create_vertex_array = true;
	}

	void BillboardCollection::remove_billboard(Billboard* billboard)
	{
		m_billboards.erase(std::remove(
			m_billboards.begin(),
			m_billboards.end(),
			billboard),
			m_billboards.end());

		m_dirty_billboards.erase(std::remove(
			m_dirty_billboards.begin(),
			m_dirty_billboards.end(),
			billboard),
			m_dirty_billboards.end());

		m_render_resources.create_vertex_array = true;
		release_billboard(billboard);
	}

	void BillboardCollection::notify_dirty(
		Billboard* billboard,
		const unsigned int& index)
	{
        if(!billboard->m_dirty)
        {
            m_dirty_billboards.push_back(billboard);
        }
		m_properties[index] = true;
	}

	TextureAtlas* BillboardCollection::get_texture_atlas()
	{
		return m_render_resources.texture_atlas.get();
	}

	void BillboardCollection::release_billboard(Billboard* billboard)
	{
		delete billboard;
	}

	void BillboardCollection::recompute_actual_positions(
			FrameState *frame_state,
			const mat4d &model_matrix,
			std::vector<Billboard*>& billboards,
            const bool &recompute_bounding_volume)
	{
		BoundingSphere* bounding_volume;

		if (frame_state->get_mode() == SceneMode::Scene3D)
		{
            bounding_volume = m_base_volume.get();
            m_bounding_volume_dirty = true;
		}
		else
		{
            bounding_volume = m_base_volume_2D.get();
		}

		std::vector<vec3d> positions;
		for(Billboard* billboard : billboards)
        {
			vec3d position        = billboard->get_position();
			vec3d actual_position = billboard->compute_actual_position(position, frame_state, model_matrix);
			if (!vec_undefined(actual_position))
            {
				billboard->set_actual_position(actual_position);
				if (recompute_bounding_volume)
                {
					positions.push_back(actual_position);
				}
                else
                {
                    bounding_volume->expand(actual_position)->clone(bounding_volume);
				}
			}
		}

		if (recompute_bounding_volume)
        {
            BoundingSphere::from_points(positions)->clone(bounding_volume);
		}
	}

    void BillboardCollection::update_mode(FrameState *frame_state)
    {
        SceneMode mode = frame_state->get_mode();

        if ((m_render_resources.create_vertex_array) ||
            (m_mode != mode) ||
            (mode   != SceneMode::Scene3D && m_actual_model_matrix != m_model_matrix))
        {
            m_mode = mode;
            m_actual_model_matrix = m_model_matrix;
            m_render_resources.create_vertex_array = true;

            if (mode == SceneMode::Scene3D || mode == SceneMode::Scene2D || mode == SceneMode::SceneCV)
            {
                recompute_actual_positions(
                        frame_state,
                        m_model_matrix,
                        m_billboards,
                        true);
            }
        }
        else if (mode == SceneMode::Morphing)
        {
            recompute_actual_positions(
                    frame_state,
                    m_model_matrix,
                    m_billboards,
                    true);
        }
        else if (mode == SceneMode::Scene2D || mode == SceneMode::SceneCV)
        {
            recompute_actual_positions(
                    frame_state,
                    m_model_matrix,
                    m_dirty_billboards,
                    false);
        }
    }

    void BillboardCollection::update_bounding_volume(
            BoundingSphere* bounding_volume,
            FrameState *frame_state)
    {
        double pixel_scale = 1.0;
        if (!m_all_sized_in_meters || m_max_pixel_offset != 0.0)
        {
            pixel_scale = frame_state->get_camera()->get_pixel_size(
                    bounding_volume,
                    frame_state->get_context()->get_drawing_surface_width(),
                    frame_state->get_context()->get_drawing_surface_height());
        }

        double size = pixel_scale * m_max_scale * m_max_size* 2.0;
        if (m_all_horizontal_center && m_all_vertical_center)
        {
            size *= 0.5;
        }

        double offset = pixel_scale * m_max_pixel_offset + m_max_eye_offset;
        bounding_volume->set_radius(bounding_volume->get_radius() + size + offset);
    }
}