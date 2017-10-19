#include "PickDepth.h"
#include "Device.h"
#include "UniformSampler.h"

namespace argeo
{
	PickDepth::PickDepth()
		: m_depth_texture(nullptr)
		, m_copy_depth_command(nullptr)
		, m_frame_buffer(nullptr)
		, m_texture_to_copy(nullptr)
	{ }


	PickDepth::~PickDepth()
	{ }

	Texture2D* PickDepth::get_depth_texture()
	{
		return m_depth_texture.get();
	}

	FrameBuffer* PickDepth::get_frame_buffer()
	{
		return m_frame_buffer.get();
	}

	void PickDepth::update(
		Context* context,
		Texture2D* texture_to_copy)
	{
		update_frame_buffers(context, texture_to_copy);
		update_copy_commands(context, texture_to_copy);
	}

	void PickDepth::execute(
		Context* context,
		PassState* pass_state)
	{
		if (m_copy_depth_command != nullptr)
		{
			m_copy_depth_command->execute(context, pass_state);
		}
	}

	void PickDepth::destroy_textures()
	{
		m_depth_texture.release();
	}

	void PickDepth::destroy_frame_buffers()
	{
		m_frame_buffer.release();
	}

	void PickDepth::create_textures(
		Context* context,
		const unsigned int& width,
		const unsigned int& height)
	{
		m_depth_texture = Device::create_texture2D(
			TextureDescription(
				width,
				height,
				TextureFormat::RedGreenBlueAlpha8));
	}

	void PickDepth::create_frame_buffers(
		Context* context,
		const unsigned int& width,
		const unsigned int& height)
	{
		m_frame_buffer = context->create_frame_buffer();
		m_frame_buffer->get_color_attachments()->set_attachment(0, m_depth_texture.get());
	}

	void PickDepth::update_frame_buffers(
		Context* context,
		Texture2D* depth_texture)
	{
		const unsigned int width  = depth_texture->get_description().get_width();
		const unsigned int height = depth_texture->get_description().get_height();

		bool texture_changed =
			m_depth_texture == nullptr ||
			m_depth_texture->get_description().get_width() != width ||
			m_depth_texture->get_description().get_height() != height;

		if (m_frame_buffer == nullptr || texture_changed)
		{
			destroy_textures();
			destroy_frame_buffers();
			create_textures(context, width, height);
			create_frame_buffers(context, width, height);
		}
	}

	void PickDepth::update_copy_commands(
		Context* context,
		Texture2D* depth_texture)
	{
		if (m_copy_depth_command == nullptr) 
		{
			std::string fs =
				R"(
				uniform sampler2D u_texture;
				in  vec2 fsTextureCoordinates;
				out vec4 fragColor;

				void main()
				{
					fragColor = og_packFloatToVec4(texture(u_texture, fsTextureCoordinates).r);
				})";

			m_copy_depth_command = std::unique_ptr<DrawCommand>(context->create_viewport_quad_command(
				fs,
				RenderState::from_cache(),
				new UniformMap 
			{
				{
					"u_texture",
					[&](IUniform* uniform)
				{ static_cast<UniformSampler*>(uniform)->set_texture(m_texture_to_copy); }
				}
			}
			));
		}

		m_texture_to_copy = depth_texture;
		m_copy_depth_command->set_framebuffer(m_frame_buffer.get());		
	}
}
