#pragma once

#include "Context.h"
#include "EnableCap.h"
#include "VertexArrayGL.h"
#include "FrameBufferGL.h"
#include "TextureUnitsGL.h"

#include "PassState.h"
#include "DrawCommand.h"
#include "ClearCommand.h"

#include "UniformState.h"
#include "ShaderProgramGL.h"
#include "StencilFace.h"
#include "StencilTestFace.h"
#include "FacetCulling.h"
#include "ProgramPointSize.h"
#include "RasterizationMode.h"
#include "Blending.h"
#include "Texture2D.h"

#include "TextureUnitGL.h"

#include "GL.h"

#include <memory>
#include <stack>

namespace argeo
{
	class ContextEGL : public Context
	{
	public:
		ContextEGL(
			Surface* surface,
			Context* shared_context,
			const unsigned int& width,
			const unsigned int& height);

		~ContextEGL();
		
		ShaderCache* get_shader_cache();

		unsigned int get_drawing_surface_width();
		unsigned int get_drawing_surface_height();
		
		FrameBuffer* get_frame_buffer();

		UniformState* get_uniform_state();

		std::unique_ptr<FrameBuffer> create_frame_buffer();

		std::unique_ptr<PickFramebuffer> create_pick_frame_buffer();

		std::unique_ptr<VertexArray> create_vertex_array();
		std::unique_ptr<VertexArray> create_vertex_array(
			VertexBufferAttributes attributes,
			IndexBuffer* index_buffer);
		std::unique_ptr<VertexArray> create_vertex_array(MeshBuffers* mesh_buffers);


		std::unique_ptr<VertexArray> create_vertex_array(
				Mesh* mesh,
				const ShaderVertexAttributeLocations& locations,
				const BufferHint& hint);

		std::unique_ptr<VertexArray> create_vertex_array(
			Mesh* mesh,
			ShaderVertexAttributeCollection* shader_attributes,
			const BufferHint& hint);

		void clear(
			ClearCommand* clear_command = nullptr,
			PassState*    pass_state = nullptr);

		void draw(
			DrawCommand* draw_command,
			PassState*   pass_state = nullptr);

		void begin_frame();
		void end_frame();

		void attach();
		void detach();

		DrawCommand* create_viewport_quad_command(
			const std::string& fragment_shader = viewport_quad_fs,
			RenderState* render_state = nullptr,
			UniformMap*  uniform_map  = nullptr);

		std::vector<vec4ub> read_pixels(
			int   x_offset,
			int   y_offset,
			int   width,
			int   height,
			FrameBuffer* frame_buffer);

		IPickable* get_object_by_pick_color(Color color);

		std::unique_ptr<PickId> create_pick_id(IPickable* object);
		void destroy_pick_id(PickId* pick_id);

		Texture2D* get_default_texture2D();

		Surface* get_surface();
		EGLContext get_native_context();

	private:
		VertexArray* get_viewport_quad_vertex_array();
		ShaderProgram* get_viewport_quad_shader_program();

	private:

		void apply_render_state(
			RenderState* render_state,
			PassState*   pass_state,
			bool clear = false);

		void apply_frame_buffer(FrameBuffer* frame_buffer);
		void apply_vertex_array(VertexArray* vertexArray);

		void apply_textures();

		void apply_shader_program(
			DrawCommand* draw_command,
			PassState*   pass_state);

		void apply_before_draw(
			DrawCommand* draw_command,
			PassState*   pass_state);

		void apply_after_draw(
			DrawCommand* draw_command,
			PassState*   pass_state);

		void verify_draw(
			DrawCommand* draw_command,
			PassState*   pass_state,
			FrameBuffer* frame_buffer);

	private:
		float		   m_clear_depth;
		int			   m_clear_stencil;
		Color          m_clear_color;
		ClearCommand*  m_default_clear_command;

		RenderState* m_default_render_state;
		RenderState* m_current_render_state;

		PassState*   m_default_pass_state;
		PassState*   m_current_pass_state;

		UniformState m_uniform_state;

		ShaderProgramGL* m_current_shader_program;
		FrameBufferGL*   m_current_framebuffer;

		unsigned int m_max_texture_unit_index;

		// Cached
		static const ShaderVertexAttributeLocations attribute_locations;
		std::unique_ptr<VertexArray> m_viewport_quad_vertex_array;
		std::unique_ptr<Texture2D> m_default_texture2D;

		// Picking
		std::stack<unsigned int> m_reusable_ids;
		unsigned int m_next_id;
		std::map<unsigned int, IPickable*> m_pickable_objects;

		Surface*   m_surface;
		EGLContext m_context;

		std::unique_ptr<ShaderCache> m_shader_cache;
	};
}
