#pragma once

#include "Context.h"
#include "VertexBuffer.h"
#include "DrawCommand.h"

#include "Vector.h"
#include "BlittableRGBA.h"
#include "TextureAtlas.h"
#include "Billboard.h"
#include "BoundingSphere.h"

#include "IPrimitive.h"

#include <memory>
#include <vector>

namespace argeo
{
	class BillboardCollection 
		: public IPrimitive
	{
	public:
		BillboardCollection(
                Scene* scene,
                const unsigned int& capacity = 10);

		~BillboardCollection();

		void create_vertex_array(Context* context);
		void dispose_vertex_array();

		void update(FrameState* frame_state);
		void update_all(Context* context);
		void update_dirty(Context* context);
		void compute_bounding_volume();

		void copy_billboards_from_system_memory(
			std::vector<vec3> positions,
			std::vector<BlittableRGBA> colors,
			std::vector<BlittableRGBA> pick_colors,
			std::vector<unsigned char> origins,
			std::vector<vec4> texture_coordinates_and_image_sizes,
			std::vector<vec4> directions_and_pixel_offsets,
			int buffer_offset,
			int length);
		
		static unsigned char billboard_origin(Billboard* billboard);

		TextureAtlas* get_texture_atlas();

		bool get_show();
		void set_show(bool value);

        Scene* get_scene();

		bool get_depth_test_enabled();
		void set_depth_test_enabled(bool value);

		bool get_depth_write();
		void set_depth_write(bool value);

		Billboard* add(
			const std::string& image_path,
			const std::string& image_id = std::string(),
			const vec3d& position		= vec3d::zero(),
			const vec2i& pixel_offset   = vec2i::zero(),
			const unsigned int& width   = Billboard::IMAGE_WIDTH,
			const unsigned int& height  = Billboard::IMAGE_HEIGHT);

		Billboard* add(
			Bitmap* image = nullptr,
			const std::string& image_id = std::string(),
			const vec3d& position		= vec3d::zero(),
			const vec2i& pixel_offset   = vec2i::zero(),
			const unsigned int& width   = Billboard::IMAGE_WIDTH,
			const unsigned int& height  = Billboard::IMAGE_HEIGHT);

		bool remove(Billboard* item);

		int get_count();
		void clear();

		bool contains(Billboard* item);
		void copy_to(
			std::vector<Billboard*> array,
			int array_index);

		void notify_dirty(
			Billboard* billboard,
			const unsigned int& index);

	private:
		static void release_billboard(Billboard* billboard);
		void add_billboard(Billboard* billboard);
		void remove_billboard(Billboard* billboard);
        void update_mode(FrameState* frame_state);

        void recompute_actual_positions(
                FrameState* frame_state,
                const mat4d& model_matrix,
				std::vector<Billboard*>& billboards,
                const bool& recompute_bounding_volume);

        void update_bounding_volume(
                BoundingSphere* bounding_volume,
                FrameState* frame_state);

	private:
        double m_max_size;
        double m_max_scale;
        double m_max_pixel_offset;
        double m_max_eye_offset;

        bool m_all_sized_in_meters;
        bool m_all_horizontal_center;
        bool m_all_vertical_center;

        mat4d m_model_matrix;
        mat4d m_actual_model_matrix;

		SceneMode m_mode;
        Scene* m_scene;

		static const ShaderVertexAttributeLocations attribute_locations;
		std::vector<Billboard*> m_billboards;
		std::vector<Billboard*> m_dirty_billboards;

		std::string m_texture_atlas_guid;
		bool m_show;
		std::vector<bool> m_properties;

		RenderStateParameters m_render_parameters;

		std::unique_ptr<BoundingSphere> m_base_volume;
        std::unique_ptr<BoundingSphere> m_base_volume_2D;
        std::unique_ptr<BoundingSphere> m_base_volume_WC;

		bool m_bounding_volume_dirty;
		DrawCommand m_draw_command;
		UniformMap  m_uniform_map;

		struct RenderResources
		{
			RenderResources()
				: create_vertex_array(true)
				, texture_atlas(nullptr)
				, index_buffer(nullptr)
				, direction_and_pixel_offset_buffer(nullptr)
				, positions_buffer(nullptr)
				, texture_coordinates_and_image_size_buffer(nullptr)
				, color_buffer(nullptr)
				, origin_buffer(nullptr)
				, vertex_array(nullptr)
				, shader_program(nullptr)
				, picking_shader_program(nullptr)
				, render_state(nullptr)
			{ }

			
			ShaderProgram* shader_program;
			ShaderProgram* picking_shader_program;

			RenderState*   render_state;

			bool create_vertex_array;
			
			std::unique_ptr<TextureAtlas> texture_atlas;
			std::unique_ptr<IndexBuffer>  index_buffer;
			std::unique_ptr<VertexBuffer> positions_buffer;
			std::unique_ptr<VertexBuffer> texture_coordinates_and_image_size_buffer;
			std::unique_ptr<VertexBuffer> color_buffer;
			std::unique_ptr<VertexBuffer> pick_color_buffer;
			std::unique_ptr<VertexBuffer> origin_buffer;
			std::unique_ptr<VertexBuffer> direction_and_pixel_offset_buffer;
			std::unique_ptr<VertexArray>  vertex_array;
		} m_render_resources;
		
	};
}
