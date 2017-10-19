#pragma once

#include "Vector.h"
#include "Ellipsoid.h"
#include "FrameState.h"
#include "DrawCommand.h"
#include "RenderState.h"
#include "Context.h"
#include "VertexArray.h"

#include <string>
#include <memory>
#include <vector>

namespace argeo
{
	class ARBuffer
	{
	public:
		ARBuffer();
		~ARBuffer();

		void update(
			Context* context,
			const Rectangle& viewport);

		void execute_mix_reality(
			Context* context,
			PassState* pass_state);

		void clear(
			Context* context,
			PassState* pass_state,
			const Color& clear_color,
			const bool&  clear_reality_framebuffer_buffer = true);

		FrameBuffer* get_reality_frame_buffer();
		FrameBuffer* get_virtuality_frame_buffer();
		FrameBuffer* get_mixed_reality_frame_buffer();

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

		void update_mix_commands(
			Context* context,
			const unsigned int& width,
			const unsigned int& height);

	private:

		struct RealityResources
		{
			RealityResources()
				: frame_buffer(nullptr)
				, color_texture(nullptr)
				, depth_texture(nullptr)
				, clear_command(nullptr)
			{ }

			std::unique_ptr<FrameBuffer> frame_buffer;
			std::unique_ptr<Texture2D>   color_texture;
			std::unique_ptr<Texture2D>   depth_texture;

			std::unique_ptr<ClearCommand> clear_command;
		} m_reality_resources;

		struct VirtualityResources
		{
			VirtualityResources()
				: frame_buffer(nullptr)
				, color_texture(nullptr)
				, depth_texture(nullptr)
				, clear_command(nullptr)
			{ }

			std::unique_ptr<FrameBuffer> frame_buffer;
			std::unique_ptr<Texture2D>   color_texture;
			std::unique_ptr<Texture2D>   depth_texture;

			std::unique_ptr<ClearCommand> clear_command;
		} m_virtuality_resources;

		struct MixedRealityResources
		{
			MixedRealityResources()
				: frame_buffer(nullptr)
				, color_texture(nullptr)
				, depth_texture(nullptr)
				, clear_command(nullptr)
			{ }

			std::unique_ptr<FrameBuffer> frame_buffer;
			std::unique_ptr<Texture2D>   color_texture;
			std::unique_ptr<Texture2D>   depth_texture;

			std::unique_ptr<ClearCommand> clear_command;
		} m_mixed_reality_resources;


		std::unique_ptr<DrawCommand>  m_mix_command;

		RenderState* m_render_state;
		Rectangle    m_viewport;
	};
}
