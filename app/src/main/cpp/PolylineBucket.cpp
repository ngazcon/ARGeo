#include "PolylineBucket.h"

#include "Polyline.h"
#include "PolylineShaders.h"
#include "Device.h"

namespace argeo
{
	PolylineBucket::PolylineBucket(
		Material* material,
		mat4d     model_matrix)
		: m_material(material)
		, m_model_matrix(model_matrix)
		, m_length_of_positions(0)
		, m_shaders_created(false)
	{ }


	PolylineBucket::~PolylineBucket()
	{ }


	std::vector<Polyline*> PolylineBucket::get_polylines()
	{
		return m_polylines;
	}

	void PolylineBucket::add_polyline(Polyline* polyline)
	{
		m_polylines.push_back(polyline);
		polyline->set_vertex_buffer_offset(get_polyline_positions_length(polyline));
		m_length_of_positions += polyline->get_vertex_buffer_offset();
	}

	void PolylineBucket::release_shaders(Context* context)
	{
		m_shaders_created = false;
		context->get_shader_cache()->release_shader_program(m_shader_program);
		context->get_shader_cache()->release_shader_program(m_pick_shader_program);
	}

	void PolylineBucket::create_shaders(
		Context* context,
		ShaderVertexAttributeLocations attribute_locations)
	{
		if (m_shaders_created)
		{
			return;
		}

		std::string material_fragment_shader = m_material->get_shader_source() + polyline_fs;

		m_shader_program = context->get_shader_cache()->get_shader_program(
			polyline_vs,
			material_fragment_shader,
			attribute_locations);

		m_pick_shader_program = context->get_shader_cache()->get_shader_program(
			polyline_vs,
			Device::create_fragment_pick_shader_source(material_fragment_shader, "in"),
			attribute_locations);
		
		m_shaders_created = true;
	}


	unsigned int PolylineBucket::get_polyline_start_index(Polyline* polyline)
	{
		unsigned int position_index = 0;
		unsigned int length         = m_polylines.size();
		for (unsigned int i = 0; i < length; ++i) 
		{
			Polyline* p = m_polylines[i];
			if (p == polyline) 
			{
				break;
			}
			position_index += polyline->get_vertex_buffer_offset();;
		}
		return position_index;
	}

	unsigned int PolylineBucket::get_polyline_positions_length(Polyline* polyline)
	{
		return polyline->get_length() * 4.0 - 4.0;
	}

	ShaderProgram* PolylineBucket::get_shader_program()
	{
		return m_shader_program;
	}

	ShaderProgram* PolylineBucket::get_pick_shader_program()
	{
		return m_pick_shader_program;
	}

	void PolylineBucket::write_update(
		unsigned int index,
		Polyline* polyline,
		VertexBuffer* position_buffer,
		VertexBuffer* texture_coordinates_expand_direction_width_and_show_buffer)
	{
		unsigned int positions_length = polyline->get_length();

		if(positions_length > 0)
		{
			index += get_polyline_start_index(polyline);
			
			unsigned int positions_vector_length = 6 * positions_length * 3;

			std::vector<vec3> positions;
			std::vector<vec4> texture_coordinates_expand_directions_widths_and_shows;

			//positions.resize(positions_vector_length);
			//texture_coordinates_expand_directions_widths_and_shows.resize(positions_length * 4);

			std::vector<vec3d> points = polyline->get_actual_points();
			int positions_length      = polyline->get_length();
			int segment_count		  = 0;
			int segment_length		  = polyline->get_length();
			bool show = polyline->get_show() && polyline->get_width() > 0;

			for (int j = 0; j < positions_length; j++)
			{
				evec3d position;
				evec3d next_position;
				evec3d prev_position;

				if (j == 0)
				{
					if (polyline->get_loop())
					{
						prev_position = evec3d(points[positions_length - 2]);
					}
					else
					{
						prev_position = evec3d(
							points[0] -
							points[1] +
							points[0]);
					}
				}
				else
				{
					prev_position = evec3d(points[j - 1]);
				}

				position = evec3d(points[j]);

				if (j == positions_length - 1)
				{
					if (polyline->get_loop())
					{
						next_position = evec3d(points[1]);
					}
					else
					{
						next_position = evec3d(
							points[positions_length - 1] -
							points[positions_length - 2] +
							points[positions_length - 1]);
					}
				}
				else
				{
					next_position = evec3d(points[j + 1]);
				}

				if (j == segment_count + segment_length)
				{
					segment_count += segment_length;
				}

				bool segment_start = j - segment_count == 0;
				bool segment_end = j == segment_count + segment_length - 1;

				int start_k = (segment_start) ? 2 : 0;
				int end_k = (segment_end) ? 2 : 4;

				for (int k = start_k; k < end_k; ++k)
				{
					positions.push_back(position.high);
					positions.push_back(position.low);

					positions.push_back(prev_position.high);
					positions.push_back(prev_position.low);

					positions.push_back(next_position.high);
					positions.push_back(next_position.low);

					float direction = (k - 2 < 0) ? -1.0 : 1.0;
					texture_coordinates_expand_directions_widths_and_shows.push_back(vec4(
						(float) j / (positions_length - 1),
						2 * (k % 2) - 1,
						direction * polyline->get_width(),
						show));
				}
			}

			position_buffer->copy_from_system(
				positions, 
				6 * 3 * sizeof(vec3) * index);

			texture_coordinates_expand_direction_width_and_show_buffer->copy_from_system(
				texture_coordinates_expand_directions_widths_and_shows,
				4 * sizeof(vec4) * index);
		}
	}

	void PolylineBucket::write(
		Context* context,
		std::vector<vec3>& positions,
		std::vector<vec4>& texture_coordinates_expand_directions_widths_and_shows,
		std::vector<BlittableRGBA>& pick_colors)
	{
		for (int i = 0; i < m_polylines.size(); ++i)
		{
			Polyline* polyline = m_polylines[i];
			
			Color pick_color = polyline->get_pick_id(context)->color;

			std::vector<vec3d> points = polyline->get_actual_points();
			int positions_length      = polyline->get_length();
			int segment_count		  = 0;
			int segment_length		  = polyline->get_length();
			bool show = polyline->get_show() && polyline->get_width() > 0;

			for (int j = 0; j < positions_length; j++)
			{
				evec3d position;
				evec3d next_position;
				evec3d prev_position;

				if (j == 0)
				{
					if (polyline->get_loop())
					{
						prev_position = evec3d(points[positions_length - 2]);
					}
					else
					{
						prev_position = evec3d(
							points[0] -
							points[1] +
							points[0]);
					}
				}
				else
				{
					prev_position = evec3d(points[j - 1]);
				}

				position = evec3d(points[j]);

				if (j == positions_length - 1)
				{
					if (polyline->get_loop())
					{
						next_position = evec3d(points[1]);
					}
					else
					{
						next_position = evec3d(
							points[positions_length - 1] -
							points[positions_length - 2] +
							points[positions_length - 1]);
					}
				}
				else
				{
					next_position = evec3d(points[j + 1]);
				}

				if (j == segment_count + segment_length)
				{
					segment_count += segment_length;
				}

				bool segment_start = j - segment_count == 0;
				bool segment_end = j == segment_count + segment_length - 1;

				int start_k = (segment_start) ? 2 : 0;
				int end_k = (segment_end) ? 2 : 4;

				for (int k = start_k; k < end_k; ++k)
				{
					positions.push_back(position.high);
					positions.push_back(position.low);

					positions.push_back(prev_position.high);
					positions.push_back(prev_position.low);

					positions.push_back(next_position.high);
					positions.push_back(next_position.low);

					float direction = (k - 2 < 0) ? -1.0 : 1.0;
					texture_coordinates_expand_directions_widths_and_shows.push_back(vec4(
						(float) j / (positions_length - 1),
						2 * (k % 2) - 1,
						direction * polyline->get_width(),
						show));

					pick_colors.push_back(pick_color);
				}
			}
		}
	}

	unsigned int PolylineBucket::update_indices(
		std::vector<std::vector<unsigned short>>& total_indices,
		std::vector<unsigned int>& vertex_buffer_offsets,
		std::vector<std::vector<VertexArrayBucketLocator*>>& vertex_array_bucket_locators,
		unsigned int offset)
	{
		int va_count = vertex_array_bucket_locators.size() - 1;
		VertexArrayBucketLocator* bucket_locator = new VertexArrayBucketLocator(0, offset, this);
		vertex_array_bucket_locators[va_count].push_back(bucket_locator);

		std::vector<unsigned short>& indices = total_indices[total_indices.size() - 1];
		unsigned short indices_count = 0;
		unsigned short count		 = 0;
		if (indices.size() > 0) 
		{
			indices_count = indices[indices.size() - 2] + 1;
		}

		for (Polyline* polyline : m_polylines) 
		{
			int number_of_segments = 1;
			if (number_of_segments > 0)
			{
				int segment_index_count = 0;
				for (int j = 0; j < number_of_segments; ++j)
				{ 
					int segment_length = polyline->get_length() - 1;

					for (int k = 0; k < segment_length; ++k)
					{
						if (indices_count + 4 >= SIXTY_FOUR_KILOBYTES - 2)
						{
							bucket_locator->count = count;

							polyline->add_locator_bucket(LocatorBucket(
								bucket_locator,
								segment_index_count));

							segment_index_count = 0;
							vertex_buffer_offsets.push_back(4);

							indices = std::vector<unsigned short>();
							total_indices.push_back(indices);
							indices_count = 0;

							count  = 0;
							offset = 0;

							bucket_locator = new VertexArrayBucketLocator(0, 0, this);
							vertex_array_bucket_locators[++va_count] = std::vector<VertexArrayBucketLocator*>();
							vertex_array_bucket_locators[++va_count].push_back(bucket_locator);
						}

						indices.push_back(indices_count + 0);
						indices.push_back(indices_count + 1);
						indices.push_back(indices_count + 3);

						indices.push_back(indices_count + 0);
						indices.push_back(indices_count + 3);
						indices.push_back(indices_count + 2);

						segment_index_count += 6;
						count  += 6;
						offset += 6;
						indices_count += 4;
					}
				}

				polyline->add_locator_bucket(LocatorBucket(
					bucket_locator,
					segment_index_count));

				if (indices_count + 4 >= SIXTY_FOUR_KILOBYTES - 2)
				{
					vertex_buffer_offsets.push_back(0);

					indices = std::vector<unsigned short>();
					total_indices.push_back(indices);
					indices_count = 0;

					bucket_locator->count = count;
					offset = 0;
					count  = 0;

					bucket_locator = new VertexArrayBucketLocator(0, 0, this);
					vertex_array_bucket_locators[++va_count] = std::vector<VertexArrayBucketLocator*>();
					vertex_array_bucket_locators[++va_count].push_back(bucket_locator);
				}
			}

			polyline->clean();
		}

		bucket_locator->count = count;
		return offset;
	}

	unsigned int PolylineBucket::get_length_of_positions() const
	{
		return m_length_of_positions;
	}
}