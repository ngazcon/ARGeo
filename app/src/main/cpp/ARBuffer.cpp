#include "Device.h"
#include "ARBuffer.h"
#include "UniformSampler.h"
#include "ClearCommand.h"

namespace argeo
{
	ARBuffer::ARBuffer()
		: m_render_state(nullptr)
		, m_mix_command(nullptr)
	{ }


	ARBuffer::~ARBuffer()
	{ }

	void ARBuffer::execute_mix_reality(
		Context* context,
		PassState* pass_state)
	{ 
		if (m_mix_command != nullptr)
		{
			m_mix_command->execute(context, pass_state);
		}
	}

	FrameBuffer* ARBuffer::get_reality_frame_buffer()
	{
		return m_reality_resources.frame_buffer.get();
	}

	FrameBuffer* ARBuffer::get_virtuality_frame_buffer()
	{
		return m_virtuality_resources.frame_buffer.get();
	}

	FrameBuffer* ARBuffer::get_mixed_reality_frame_buffer()
	{
		return m_mixed_reality_resources.frame_buffer.get();
	}

	void ARBuffer::update(
		Context* context,
		const Rectangle& viewport)
	{
		const unsigned int width  = viewport.get_width();
		const unsigned int height = viewport.get_height();

		update_frame_buffers(context, width, height);
		update_mix_commands(context, width, height);
	}

	void ARBuffer::update_frame_buffers(
		Context* context,
		const unsigned int& width,
		const unsigned int& height)
	{
		bool texture_changed =
			m_reality_resources.color_texture == nullptr ||
			m_reality_resources.color_texture->get_description().get_width() != width ||
			m_reality_resources.color_texture->get_description().get_height() != height;

		if (m_reality_resources.frame_buffer == nullptr || texture_changed)
		{
			create_textures(context, width, height);
			create_frame_buffers(context, width, height);
		}
	}

	void ARBuffer::create_textures(
		Context* context,
		const unsigned int& width,
		const unsigned int& height)
	{
        // TODO: BGRA
		m_reality_resources.color_texture = Device::create_texture2D(
			TextureDescription(
				width,
				height,
				TextureFormat::RedGreenBlueAlpha8));

		m_reality_resources.depth_texture = Device::create_texture2D(
			TextureDescription(
				width,
				height,
				TextureFormat::Depth24));

		m_virtuality_resources.color_texture = Device::create_texture2D(
			TextureDescription(
				width,
				height,
				TextureFormat::RedGreenBlueAlpha8));

		m_virtuality_resources.depth_texture = Device::create_texture2D(
			TextureDescription(
				width,
				height,
				TextureFormat::Depth24));

		m_mixed_reality_resources.color_texture = Device::create_texture2D(
			TextureDescription(
				width,
				height,
				TextureFormat::RedGreenBlueAlpha8));

		m_mixed_reality_resources.depth_texture = Device::create_texture2D(
			TextureDescription(
				width,
				height,
				TextureFormat::Depth24));
	}

	void ARBuffer::create_frame_buffers(
		Context* context,
		const unsigned int& width,
		const unsigned int& height)
	{
		m_virtuality_resources.frame_buffer = context->create_frame_buffer();
		m_virtuality_resources.frame_buffer->get_color_attachments()->set_attachment(0, m_virtuality_resources.color_texture.get());
		m_virtuality_resources.frame_buffer->set_depth_attachment_texture(m_virtuality_resources.depth_texture.get());

		m_reality_resources.frame_buffer = context->create_frame_buffer();
		m_reality_resources.frame_buffer->get_color_attachments()->set_attachment(0, m_reality_resources.color_texture.get());
		m_reality_resources.frame_buffer->set_depth_attachment_texture(m_reality_resources.depth_texture.get());

		m_mixed_reality_resources.frame_buffer = context->create_frame_buffer();
		m_mixed_reality_resources.frame_buffer->get_color_attachments()->set_attachment(0, m_mixed_reality_resources.color_texture.get());
		m_mixed_reality_resources.frame_buffer->set_depth_attachment_texture(m_mixed_reality_resources.depth_texture.get());
	}

	void ARBuffer::clear(
		Context* context,
		PassState* pass_state,
		const Color& clear_color,
		const bool&  clear_reality_framebuffer_buffer)
	{
		if (m_virtuality_resources.clear_command != nullptr)
		{
			m_virtuality_resources.clear_command->color = clear_color;
			m_virtuality_resources.clear_command->execute(context, pass_state);

			if (clear_reality_framebuffer_buffer)
			{
				m_reality_resources.clear_command->color = clear_color;
				m_reality_resources.clear_command->execute(context, pass_state);
			}

			m_mixed_reality_resources.clear_command->color = clear_color;
			m_mixed_reality_resources.clear_command->execute(context, pass_state);
		}
	}

	void ARBuffer::update_mix_commands(
		Context* context,
		const unsigned int& width,
		const unsigned int& height)
	{
		m_viewport.set_width(width);
		m_viewport.set_height(height);

		if (m_render_state == nullptr ||
			m_render_state->get_viewport() != m_viewport)
		{
			RenderStateParameters params;
			params.viewport = m_viewport;
			m_render_state  = RenderState::from_cache(params);
		}

		if (m_mix_command == nullptr)
		{
			std::string fs =
				R"(
				
				uniform sampler2D u_realityTexture;
				uniform sampler2D u_virtualityTexture;

				in  vec2 fsTextureCoordinates;
				out vec4 fragColor;

				void main()
				{
					vec4 realColor    = texture(u_realityTexture, fsTextureCoordinates);
					vec4 virtualColor = texture(u_virtualityTexture, fsTextureCoordinates);

					fragColor		  = mix(realColor, virtualColor, virtualColor.a );
				})";

			m_mix_command = std::unique_ptr<DrawCommand>(context->create_viewport_quad_command(
				fs,
				nullptr,
				new UniformMap
			{
			{
				"u_realityTexture",
				[&](IUniform* uniform)
				{ static_cast<UniformSampler*>(uniform)->set_texture(m_reality_resources.color_texture.get()); }
			},
			{
				"u_virtualityTexture",
					[&](IUniform* uniform)
				{ static_cast<UniformSampler*>(uniform)->set_texture(m_virtuality_resources.color_texture.get()); }
			},
			}
			));
		}

		m_mix_command->set_render_state(m_render_state);
		m_mix_command->set_framebuffer(m_mixed_reality_resources.frame_buffer.get());

		if (m_reality_resources.clear_command == nullptr)
		{
			m_reality_resources.clear_command = std::unique_ptr<ClearCommand>(new ClearCommand());
			m_reality_resources.clear_command->color = Color(0, 0, 0, 0);
			m_reality_resources.clear_command->stencil = 0.0;
			m_reality_resources.clear_command->depth = 1.0;
			m_reality_resources.clear_command->buffers = ClearBuffers::All;
		}
		m_reality_resources.clear_command->frame_buffer = m_reality_resources.frame_buffer.get();

		if (m_virtuality_resources.clear_command == nullptr)
		{
			m_virtuality_resources.clear_command = std::unique_ptr<ClearCommand>(new ClearCommand());
			m_virtuality_resources.clear_command->color = Color(0, 0, 0, 0);
			m_virtuality_resources.clear_command->stencil = 0.0;
			m_virtuality_resources.clear_command->depth   = 1.0;
			m_virtuality_resources.clear_command->buffers = ClearBuffers::All;
		}
		m_virtuality_resources.clear_command->frame_buffer = m_virtuality_resources.frame_buffer.get();

		if (m_mixed_reality_resources.clear_command == nullptr)
		{
			m_mixed_reality_resources.clear_command = std::unique_ptr<ClearCommand>(new ClearCommand());
			m_mixed_reality_resources.clear_command->color = Color(0, 0, 0, 0);
			m_mixed_reality_resources.clear_command->stencil = 0.0;
			m_mixed_reality_resources.clear_command->depth = 1.0;
			m_mixed_reality_resources.clear_command->buffers = ClearBuffers::All;
		}
		m_mixed_reality_resources.clear_command->frame_buffer = m_mixed_reality_resources.frame_buffer.get();
	}
}
