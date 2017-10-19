#pragma once

#include "Texture2D.h"
#include "FrameBuffer.h"
#include "DrawCommand.h"
#include "ClearCommand.h"
#include "Context.h"

#include <memory>

namespace argeo
{
	class PickDepth
	{
	public:
		PickDepth();
		~PickDepth();

		void update(
			Context* context,
			Texture2D* texture_to_copy);
		
		void execute(
			Context* context, 
			PassState* pass_state);

		FrameBuffer* get_frame_buffer();
		Texture2D* get_depth_texture();

	private:
		void destroy_textures();
		void destroy_frame_buffers();

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
			Texture2D* depth_texture);

		void update_copy_commands(
			Context* context,
			Texture2D* depth_texture);

	private:
		std::unique_ptr<Texture2D> m_depth_texture;
		Texture2D* m_texture_to_copy;

		std::unique_ptr<FrameBuffer> m_frame_buffer;

		std::unique_ptr<DrawCommand>  m_copy_depth_command;
	};
}