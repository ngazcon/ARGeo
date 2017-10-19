#pragma once

#include "Texture2D.h"
#include "FrameBuffer.h"
#include "DrawCommand.h"
#include "ClearCommand.h"
#include "RenderState.h"
#include "Context.h"

#include <memory>

namespace argeo
{
	class TerrainDepth
	{
	public:
		TerrainDepth();
		~TerrainDepth();

		Texture2D* get_color_texture();
		Texture2D* get_depth_stencil_texture();
		FrameBuffer* get_frame_buffer();

		void update(
			Context* context,
			const Rectangle& viewport);
		
		void execute_copy_depth(
			Context* context, 
			PassState* pass_state);

		void execute_copy_color(
			Context* context,
			PassState* pass_state);

		void clear(
			Context* context,
			PassState* pass_state,
			const Color& clear_color);

	private:
		void create_textures(
			Context* context,
			const unsigned int& width,
			const unsigned int& height);

		void create_frame_buffers(
			Context* context,
			const unsigned int& width,
			const unsigned int& height);

		void update_frame_buffers(
			Context* context,
			const unsigned int& width,
			const unsigned int& height);

		void update_copy_commands(
			Context* context,
			const unsigned int& width,
			const unsigned int& height);

	private:
		std::unique_ptr<Texture2D> m_color_texture;
		std::unique_ptr<Texture2D> m_depth_stencil_texture;
		std::unique_ptr<Texture2D> m_terrain_depth_texture;

		std::unique_ptr<FrameBuffer> m_copy_depth_frame_buffer;
		std::unique_ptr<FrameBuffer> m_frame_buffer;

		std::unique_ptr<ClearCommand> m_clear_color_command;
		std::unique_ptr<DrawCommand>  m_copy_color_command;
		std::unique_ptr<DrawCommand>  m_copy_depth_command;

		RenderState* m_render_state;
		Rectangle m_viewport;
	};
}