#pragma once

#include "Material.h"
#include "Matrix.h"
#include "ShaderProgram.h"
#include "ShaderVertexAttributeLocations.h"
#include "BlittableRGBA.h"
#include "VertexArrayBucketLocator.h"
#include "VertexBuffer.h"

#include <memory>
#include <vector>

namespace argeo
{
	class Polyline;
	
	class PolylineBucket
	{
	public:
		PolylineBucket(
			Material* material,
			mat4d     model_matrix);

		~PolylineBucket();

		std::vector<Polyline*> get_polylines();
		void add_polyline(Polyline* polyline);

		void create_shaders(
			Context* context,
			ShaderVertexAttributeLocations attribute_locations);
		
		void release_shaders(Context* context);

		unsigned int update_indices(
			std::vector<std::vector<unsigned short>>& total_indices,
			std::vector<unsigned int>& vertex_buffer_offsets,
			std::vector<std::vector<VertexArrayBucketLocator*>>& vertex_array_bucket_locators,
			unsigned int offset);

		void write_update(
			unsigned int index,
			Polyline* polyline,
			VertexBuffer* position_buffer,
			VertexBuffer* texture_coordinates_expand_direction_width_and_show_buffer);

		void write(
			Context* context,
			std::vector<vec3>& positions,
			std::vector<vec4>& texture_coordinates_expand_directions_widths_and_shows,
			std::vector<BlittableRGBA>& pick_colors);

		ShaderProgram* get_shader_program();
		ShaderProgram* get_pick_shader_program();

		unsigned int get_length_of_positions() const;

	private:
		unsigned int get_polyline_positions_length(Polyline* polyline);
		unsigned int get_polyline_start_index(Polyline* polyline);

	private:
		unsigned int m_length_of_positions;

		bool m_shaders_created;

		Material* m_material;
		mat4d     m_model_matrix;
		std::vector<Polyline*> m_polylines;

		ShaderProgram* m_shader_program;
		ShaderProgram* m_pick_shader_program;
	};
}

