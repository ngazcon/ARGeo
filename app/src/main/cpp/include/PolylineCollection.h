#pragma once

#include "RenderState.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "DrawCommand.h"
#include "Material.h"
#include "PolylineBucket.h"

#include "IPrimitive.h"

#include <vector>
#include <memory>

namespace argeo
{
	class Polyline;

	struct VertexArrayFacade
	{
		VertexArrayFacade(
			VertexArray* vertex_array = nullptr,
			std::vector<VertexArrayBucketLocator*> buckets = std::vector<VertexArrayBucketLocator*>())
			: vertex_array(vertex_array)
			, buckets(buckets)
		{ } 

		VertexArray* vertex_array;
		std::vector<VertexArrayBucketLocator*> buckets;
	};

	class PolylineCollection : public IPrimitive
	{
	public:
		PolylineCollection();
		~PolylineCollection();

		void notify_dirty(
			Polyline* polyline,
			const unsigned int& property);

		bool get_show();
		void set_show(bool value);

		void update(FrameState* frame_state);

		Polyline* add(
			const std::vector<vec3d>& points,
			const bool& loop    = false,
			const double& width = 1.0);

		bool remove(Polyline* item);

		int get_count();

	private:	

		void create_command_list(
			FrameState* frame_state,
			std::vector<DrawCommand*>& command_list);

		void create_render_states();
		void create_vertex_arrays(Context* context);

		void release_shader_programs(Context* context);
		void dispose_vertex_arrays();

		void sort_polylines_into_buckets();
		std::string get_material_id(Material* material);

		static void release_polyline(Polyline* polyline);
		void add_polyline(Polyline* polyline);
		void remove_polyline(Polyline* polyline);

	private:
		static const ShaderVertexAttributeLocations attribute_locations;

		std::vector<Polyline*> m_polylines;
		std::vector<Polyline*> m_dirty_polylines;

		std::vector<bool> m_properties;

		bool m_show;

		mat4d m_model_matrix;

		std::map<std::string, PolylineBucket*> m_polyline_buckets;

		

		struct RenderResources
		{
			RenderResources()
				: create_vertex_arrays(true)
				, create_render_states(true)
			{ }

			bool create_vertex_arrays;
			bool create_render_states;

			std::vector<VertexArrayFacade> vertex_arrays;

			std::unique_ptr<VertexBuffer> position_buffer;
			std::unique_ptr<VertexBuffer> pick_color_buffer;
			std::unique_ptr<VertexBuffer> texture_coordinates_expand_direction_width_and_show_buffer;

			RenderState* render_state;
		} m_render_resources;

		std::vector<DrawCommand*> m_draw_commands;
		std::vector<DrawCommand*> m_pick_commands;
	};
}
