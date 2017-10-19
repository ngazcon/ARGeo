#pragma once

#include "Mesh.h"
#include "VertexArray.h"
#include "PrimitiveType.h"
#include "UniformState.h"
#include "UniformMap.h"
#include "Rectangle.h"
#include "MeshBuffers.h"
#include "FrameBuffer.h"
#include "ShaderVertexAttributeCollection.h"
#include "ShaderVertexAttributeLocations.h"
#include "VertexBufferAttributes.h"
#include "ViewportQuadShaders.h"
#include "Texture2D.h"

#include "ShaderCache.h"

#include <memory>

namespace argeo
{
	class Surface;
	class ClearCommand;
	class DrawCommand;
	class PassState;
	class RenderState;
	class PickFramebuffer;
	class IPickable;
	class PickId;

	/*
	 * Represent a context of rendering.
	 * It's used for issuing rendering commands.
	 */
	class Context
	{
	public:

		virtual ShaderCache* get_shader_cache() = 0;

		// Get the current target framebuffer.
		virtual FrameBuffer* get_frame_buffer() = 0;

		virtual UniformState* get_uniform_state() = 0;

		virtual unsigned int get_drawing_surface_width()  = 0;

		virtual unsigned int get_drawing_surface_height() = 0;

		// Creates a new empty frame buffer for utilization in this context.
		// The owner of the pointer is now the caller.
		virtual std::unique_ptr<FrameBuffer> create_frame_buffer() = 0;

		// Creates a new empty frame buffer for utilization in this context.
		// The owner of the pointer is now the caller.
		virtual std::unique_ptr<PickFramebuffer> create_pick_frame_buffer() = 0;

		// Creates a new empty vertex array for utilization in this context.
		// The owner of the pointer is now the caller.
		virtual std::unique_ptr<VertexArray> create_vertex_array() = 0;

		// Creates a new vertex array for utilization in this context.
		// The vertex array is filled with the vertex and index buffers needed to represent the mesh buffers.
		// The owner of the pointer is now the caller.
		virtual std::unique_ptr<VertexArray> create_vertex_array(MeshBuffers* mesh_buffers) = 0;
		
		// Creates a new vertex array for utilization in this context.
		// The vertex array is filled with the vertex and index buffers needed to represent the mesh buffers.
		// The owner of the pointer is now the caller.
		virtual std::unique_ptr<VertexArray> create_vertex_array(
			VertexBufferAttributes buffer_locations,
			IndexBuffer* index_buffer = nullptr) = 0;

		// Creates a new vertex array for utilization in this context.
		// The vertex array is filled with the vertex and index buffers aaccording with the given mesh and the shader attributes.
		// The buffer hint is used to create the vertex and index buffers.
		// The owner of the pointer is now the caller.
		virtual std::unique_ptr<VertexArray> create_vertex_array(
			Mesh* mesh,
			ShaderVertexAttributeCollection* shader_attributes,
			const BufferHint& hint) = 0;

        // Creates a new vertex array for utilization in this context.
        // The vertex array is filled with the vertex and index buffers aaccording with the given mesh and the shader attributes.
        // The buffer hint is used to create the vertex and index buffers.
        // The owner of the pointer is now the caller.
        virtual std::unique_ptr<VertexArray> create_vertex_array(
                Mesh* mesh,
                const ShaderVertexAttributeLocations& locations,
                const BufferHint& hint) = 0;

		// Clears the buffers according to the given clear command.
		virtual void clear(
			ClearCommand* clear_command = nullptr,
			PassState*    pass_state    = nullptr) = 0;

		virtual void end_frame() = 0;

		virtual void begin_frame() = 0;

		// Draw into the target framebuffer using the given primitive type, draw state and scene state.
		virtual void draw(
			DrawCommand* draw_command,
			PassState*   pass_state = nullptr) = 0;

		// Get the windows containing this context.
		virtual Surface* get_surface() = 0;

		// Makes this context the current in the calling thread.
		virtual void attach() = 0;

		virtual void detach() = 0;

		virtual DrawCommand* create_viewport_quad_command(
			const std::string& fragment_shader = viewport_quad_fs,
			RenderState* render_state = nullptr,
			UniformMap*  uniform_map  = nullptr) = 0;

		virtual std::vector<vec4ub> read_pixels(
			int   x_offset,
			int   y_offset,
			int   width,
			int   height,
			FrameBuffer* frame_buffer) = 0;

		virtual IPickable* get_object_by_pick_color(Color color) = 0;

		virtual Texture2D* get_default_texture2D() = 0;

		/**
		* Creates a unique ID associated with the input object for use with color-buffer picking.
		* The ID has an RGBA color value unique to this context.  You must call destroy()
		* on the pick ID when destroying the input object.
		*
		* @param {Object} object The object to associate with the pick ID.
		* @returns {Object} A PickId object with a <code>color</code> property.
		*
		* @exception {RuntimeError} Out of unique Pick IDs.
		*/
		virtual std::unique_ptr<PickId> create_pick_id(IPickable* object) = 0;

		virtual void destroy_pick_id(PickId* pick_id) = 0;
	};
}