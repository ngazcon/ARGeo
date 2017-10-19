#include "PolylineCollection.h"

#include "Device.h"
#include "Polyline.h"
#include "PolylineShaders.h"
#include "BlittableRGBA.h"
#include "ArgeoMath.h"

#include <algorithm>
#include <sstream>

namespace argeo
{
	const ShaderVertexAttributeLocations PolylineCollection::attribute_locations =
	{
		{ "texCoordExpandWidthAndShow",	  0 },
		{ "positionHigh",				  1 },
		{ "positionLow",				  2 },
		{ "prevPositionHigh",			  3 },
		{ "prevPositionLow",			  4 },
		{ "nextPositionHigh",			  5 },
		{ "nextPositionLow",			  6 },
		{ "pickColor",					  7 },
	};

	PolylineCollection::PolylineCollection()
		: m_show(true)
	{ 
		m_properties.resize(Polyline::number_of_properties, false);
	}


	PolylineCollection::~PolylineCollection()
	{
		for (Polyline* p : m_polylines)
		{
			release_polyline(p);
		}
		dispose_vertex_arrays();
	}

	void PolylineCollection::create_render_states()
	{
		m_render_resources.create_render_states = false;

		RenderStateParameters render_params;
		render_params.blending.enabled    = true;
		render_params.depth_test.enabled  = true;
		render_params.depth_test.function = DepthTestFunction::LessOrEqual;

		m_render_resources.render_state = RenderState::from_cache(render_params);
	}

	void PolylineCollection::update(FrameState* frame_state)
	{
		if (m_polylines.empty())
		{
			return;
		}

		Context* context = frame_state->get_context();

		if (m_render_resources.create_render_states)
		{
			create_render_states();
		}

		// if a polyline's positions size changes, we need to recreate the vertex arrays and vertex buffers because the indices will be different.
		// if a polyline's material changes, we need to recreate the VAOs and VBOs because they will be batched differenty.
		if (m_render_resources.create_vertex_arrays)
		{
			create_vertex_arrays(context);
		}
		else if (m_dirty_polylines.size() > 0)
		{
			for (Polyline* polyline : m_dirty_polylines)
			{
				polyline->update();
			}

			if (m_properties[Polyline::material_index] || m_properties[Polyline::position_size_index])
			{
				create_vertex_arrays(context);
			}
			else
			{
				for (Polyline* polyline : m_dirty_polylines)
				{
					std::vector<bool> properties	   = polyline->get_properties();
					std::vector<LocatorBucket> buckets = polyline->get_locator_buckets();
					unsigned int index = 0;

					for (LocatorBucket bucket : buckets)
					{
						if (properties[Polyline::position_index] || properties[Polyline::show_index] || properties[Polyline::width_index])
						{
							bucket.vertex_array_bucket_locator->bucket->write_update(
								index,
								polyline, 
								m_render_resources.position_buffer.get(),
								m_render_resources.texture_coordinates_expand_direction_width_and_show_buffer.get());
						}
						index += bucket.vertex_array_bucket_locator->bucket->get_length_of_positions();
					}

					polyline->clean();
				}
			}

			m_dirty_polylines.clear();
		}

		std::replace(
			m_properties.begin(),
			m_properties.end(),
			true,
			false);

		if (m_show)
		{
			create_command_list(
				frame_state,
				frame_state->get_pass() == Pass::Render ? m_draw_commands : m_pick_commands);
		}
	}

	void PolylineCollection::create_command_list(
		FrameState* frame_state,
		std::vector<DrawCommand*>& command_list)
	{
		unsigned int command_index   = 0;
		unsigned int commands_length = command_list.size();

		for (VertexArrayFacade vertex_array_facade : m_render_resources.vertex_arrays)
		{
			std::vector<VertexArrayBucketLocator*> buckets = vertex_array_facade.buckets;

			for (VertexArrayBucketLocator* bucket_locator : buckets)
			{
				unsigned int offset  = bucket_locator->offset;
				unsigned int count   = 0;
				DrawCommand* command = nullptr;

				std::string current_id;
				Material*   current_material = nullptr;

				ShaderProgram* shader_program = frame_state->get_pass() == Pass::Render ? 
					bucket_locator->bucket->get_shader_program() :
					bucket_locator->bucket->get_pick_shader_program();

				RenderState* render_state = m_render_resources.render_state;

				std::vector<Polyline*> polylines = bucket_locator->bucket->get_polylines();

				for (Polyline* polyline : polylines)
				{
					std::string id = get_material_id(polyline->get_material());

					if (id.compare(current_id) != 0)
					{
						if (!current_id.empty() && count > 0)
						{
							if (command_index >= commands_length)
							{
								command = new DrawCommand(PrimitiveType::Triangles);
								command_list.push_back(command);
							}
							else
							{
								command = command_list[command_index];
							}
							++command_index;

							command->set_vertex_array(vertex_array_facade.vertex_array);
							command->set_render_state(render_state);
							command->set_shader_program(shader_program);
							command->set_uniform_map(&current_material->get_uniform_map());
							command->set_count(count);
							command->set_offset(offset);

							offset += count;
							count   = 0;

							frame_state->get_command_queue().push_back(command);
						}

						current_id = id;
						current_material = polyline->get_material();
						current_material->update(frame_state);
					}

					std::vector<LocatorBucket> locators	= polyline->get_locator_buckets();
					for (LocatorBucket locator_bucket : locators)
					{
						if (locator_bucket.vertex_array_bucket_locator == bucket_locator)
						{
							count += locator_bucket.count;
						}
					}
				}

				if (!current_id.empty() && count > 0)
				{
					if (command_index >= commands_length)
					{
						command = new DrawCommand(PrimitiveType::Triangles);
						command_list.push_back(command);
					}
					else
					{
						command = command_list[command_index];
					}
					++command_index;

					command->set_vertex_array(vertex_array_facade.vertex_array);
					command->set_render_state(render_state);
					command->set_shader_program(shader_program);
					command->set_uniform_map(&current_material->get_uniform_map());
					command->set_count(count);
					command->set_offset(offset);

					frame_state->get_command_queue().push_back(command);
				}

				current_id = std::string();
				
			}
		}
	}

	std::string PolylineCollection::get_material_id(Material* material)
	{
		UniformValues values = material->get_uniform_values();

		std::ostringstream stream;
		for (auto entry : values) 
		{
			stream << entry.first << entry.second->to_string();
		}

		return material->get_name() + ":" +  stream.str();
	}

	void PolylineCollection::dispose_vertex_arrays()
	{
		for (VertexArrayFacade vertex_array_facade : m_render_resources.vertex_arrays)
		{
			delete vertex_array_facade.vertex_array;
			for (VertexArrayBucketLocator* bucket : vertex_array_facade.buckets)
			{
				delete bucket;
			}
		}
		m_render_resources.vertex_arrays.clear();
	}

	void PolylineCollection::release_shader_programs(Context* context)
	{
		for (auto entry : m_polyline_buckets)
		{
			entry.second->release_shaders(context);
		}
	}

	void PolylineCollection::create_vertex_arrays(Context* context)
	{
		m_render_resources.create_vertex_arrays = false;

		release_shader_programs(context);
		dispose_vertex_arrays();

		sort_polylines_into_buckets();

		unsigned int total_length = 0;
		unsigned int offset       = 0;

		for (auto entry : m_polyline_buckets)
		{
			PolylineBucket* bucket = entry.second;
			bucket->create_shaders(
				context,
				attribute_locations);
			total_length += bucket->get_length_of_positions();
		}

		//stores all of the individual indices arrays.
		std::vector<std::vector<unsigned short>> total_indices =
		{ std::vector<unsigned short>() };

		std::vector<unsigned int> vertex_buffer_offsets = 
		{ 0 };

		std::vector<std::vector<VertexArrayBucketLocator*>> vertex_array_bucket_locators =
		{ std::vector<VertexArrayBucketLocator*>() };

		if (total_length > 0)
		{
			unsigned int position_index = 0;
			unsigned int color_index = 0;
			unsigned int expand_width_index = 0;

			std::vector<vec3> positions;
			std::vector<vec4> texture_coordinates_expand_directions_widths_and_shows;
			std::vector<BlittableRGBA> pick_colors;

			for (auto entry : m_polyline_buckets)
			{
				PolylineBucket* bucket = entry.second;

				bucket->write(
					context,
					positions,
					texture_coordinates_expand_directions_widths_and_shows,
					pick_colors);

				offset = bucket->update_indices(
					total_indices,
					vertex_buffer_offsets,
					vertex_array_bucket_locators,
					offset);
			}

			m_render_resources.position_buffer = Device::create_vertex_buffer(
				positions,
				BufferHint::StaticDraw);

			m_render_resources.texture_coordinates_expand_direction_width_and_show_buffer = Device::create_vertex_buffer(
				texture_coordinates_expand_directions_widths_and_shows,
				BufferHint::StaticDraw);

			m_render_resources.pick_color_buffer = Device::create_vertex_buffer(
				pick_colors,
				BufferHint::StaticDraw);

			unsigned int vbo = 0;
			unsigned int number_of_indices_arrays = total_indices.size();

			unsigned int position_size_in_bytes     = sizeof(vec3);
			unsigned int pick_color_size_in_bytes   = sizeof(BlittableRGBA);
			unsigned int expand_width_size_in_bytes = sizeof(vec4);

			for (unsigned int k = 0; k < number_of_indices_arrays; ++k)
			{
				std::vector<unsigned short> indices = total_indices[k];

				if (indices.size() > 0)
				{
					IndexBuffer* index_buffer = Device::create_index_buffer(
						BufferHint::StaticDraw,
						indices.size() * sizeof(unsigned short)).release();
					index_buffer->copy_from_system<unsigned short>(indices);

					vbo += vertex_buffer_offsets[k];

					int position_high_offset	  = 6 * (k * (position_size_in_bytes * SIXTY_FOUR_KILOBYTES) - vbo * position_size_in_bytes);
					int position_low_offset		  = position_size_in_bytes + position_high_offset;
					int prev_position_high_offset = position_size_in_bytes + position_low_offset;
					int prev_position_low_offset  = position_size_in_bytes + prev_position_high_offset;
					int next_position_high_offset = position_size_in_bytes + prev_position_low_offset;
					int next_position_low_offset  = position_size_in_bytes + next_position_high_offset;

					int pick_color_buffer_offset   = k * (pick_color_size_in_bytes * SIXTY_FOUR_KILOBYTES) - vbo * pick_color_size_in_bytes;
					int expand_width_buffer_offset = k * (expand_width_size_in_bytes * SIXTY_FOUR_KILOBYTES) - vbo * expand_width_size_in_bytes;


					VertexBufferAttribute* position_high_attribute = new VertexBufferAttribute(
						m_render_resources.position_buffer.get(),
						ComponentDataType::Float,
						3,
						false,
						position_high_offset,
						6 * position_size_in_bytes);

					VertexBufferAttribute* position_low_attribute = new VertexBufferAttribute(
						m_render_resources.position_buffer.get(),
						ComponentDataType::Float,
						3,
						false,
						position_low_offset,
						6 * position_size_in_bytes);

					VertexBufferAttribute* next_position_high_attribute = new VertexBufferAttribute(
						m_render_resources.position_buffer.get(),
						ComponentDataType::Float,
						3,
						false,
						next_position_high_offset,
						6 * position_size_in_bytes);

					VertexBufferAttribute* next_position_low_attribute = new VertexBufferAttribute(
						m_render_resources.position_buffer.get(),
						ComponentDataType::Float,
						3,
						false,
						next_position_low_offset,
						6 * position_size_in_bytes);

					VertexBufferAttribute* prev_position_high_attribute = new VertexBufferAttribute(
						m_render_resources.position_buffer.get(),
						ComponentDataType::Float,
						3,
						false,
						prev_position_high_offset,
						6 * position_size_in_bytes);

					VertexBufferAttribute* prev_position_low_attribute = new VertexBufferAttribute(
						m_render_resources.position_buffer.get(),
						ComponentDataType::Float,
						3,
						false,
						prev_position_low_offset,
						6 * position_size_in_bytes);

					VertexBufferAttribute* pick_color_attribute = new VertexBufferAttribute(
						m_render_resources.pick_color_buffer.get(),
						ComponentDataType::UnsignedByte,
						4,
						true,
						pick_color_buffer_offset);

					VertexBufferAttribute* tex_coord_expand_direction_width_and_show_attribute = new VertexBufferAttribute(
						m_render_resources.texture_coordinates_expand_direction_width_and_show_buffer.get(),
						ComponentDataType::Float,
						4,
						false,
						expand_width_buffer_offset);

					std::unique_ptr<VertexArray> vertex_array = context->create_vertex_array(
						VertexBufferAttributes
					{
						{ attribute_locations.at("positionLow"),                position_low_attribute },
						{ attribute_locations.at("positionHigh"),               position_high_attribute },
						{ attribute_locations.at("nextPositionLow"),            next_position_low_attribute },
						{ attribute_locations.at("nextPositionHigh"),           next_position_high_attribute },
						{ attribute_locations.at("prevPositionLow"),            prev_position_low_attribute },
						{ attribute_locations.at("prevPositionHigh"),           prev_position_high_attribute },
						{ attribute_locations.at("texCoordExpandWidthAndShow"),	tex_coord_expand_direction_width_and_show_attribute },
						{ attribute_locations.at("pickColor"),					pick_color_attribute },
					},
					index_buffer);
					vertex_array->set_dispose_buffers(true);

					m_render_resources.vertex_arrays.push_back(
						VertexArrayFacade(
							vertex_array.release(),
							vertex_array_bucket_locators[k]));
				}
			}
		}
	}

	void PolylineCollection::sort_polylines_into_buckets()
	{
		m_polyline_buckets.clear();

		for (Polyline* polyline : m_polylines) 
		{
			if (polyline->get_actual_points().size())
			{
				polyline->update();
				Material* material = polyline->get_material();

				if (m_polyline_buckets.find(material->get_name()) == m_polyline_buckets.end())
				{
					m_polyline_buckets[material->get_name()] = new PolylineBucket(
						material,
						m_model_matrix);
				}
				auto value = m_polyline_buckets[material->get_name()];
				value->add_polyline(polyline);
			}
		}
	}

	void PolylineCollection::notify_dirty(
		Polyline* polyline,
		const unsigned int& property)
	{
		m_properties[property] = true;
		m_dirty_polylines.push_back(polyline);
	}

	bool PolylineCollection::get_show()
	{
		return m_show;
	}

	void PolylineCollection::set_show(bool value)
	{
		m_show = value;
	}

	Polyline* PolylineCollection::add(
		const std::vector<vec3d>& points,
		const bool& loop,
		const double& width)
	{
		Polyline* polyline = new Polyline(
			points,
			loop,
			width);
		
		add_polyline(polyline);
		m_polylines.push_back(polyline);

		return polyline;
	}

	bool PolylineCollection::remove(Polyline* item)
	{
		int size_before = m_polylines.size();

		m_polylines.erase(std::remove(
			m_polylines.begin(),
			m_polylines.end(),
			item),
			m_polylines.end());

		int size_after = m_polylines.size();

		bool removed = size_before > size_after;

		if (removed)
		{
			remove_polyline(item);
		}

		return removed;
	}

	int PolylineCollection::get_count()
	{
		return m_polylines.size();
	}

	void PolylineCollection::add_polyline(Polyline* polyline)
	{
		if (polyline == nullptr)
		{
			throw std::invalid_argument("polyline");
		}

		if (polyline->get_group() != nullptr)
		{
			if (polyline->get_group() != this)
			{
				throw std::invalid_argument("polyline is already in another BillboardCollection.");
			}
			else
			{
				throw std::invalid_argument("polyline was already added to this BillboardCollection.");
			}
		}

		polyline->set_group(this);
		m_render_resources.create_vertex_arrays = true;
	}

	void PolylineCollection::remove_polyline(Polyline* polyline)
	{
		{
			m_polylines.erase(std::remove(
				m_polylines.begin(),
				m_polylines.end(),
				polyline),
				m_polylines.end());
		}

		m_render_resources.create_vertex_arrays = true;
		release_polyline(polyline);
	}

	void PolylineCollection::release_polyline(Polyline* polyline)
	{
		polyline->clean();
		polyline->set_group(nullptr);
		polyline->set_vertex_buffer_offset(0);
	}
}