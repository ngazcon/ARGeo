#include "TerrainDepth.h"
#include "PostProcessFilters.h"
#include "Device.h"
#include "UniformSampler.h"

namespace argeo
{
	TerrainDepth::TerrainDepth()
		: m_render_state(nullptr)
		, m_color_texture(nullptr)
		, m_depth_stencil_texture(nullptr)
		, m_terrain_depth_texture(nullptr)
		, m_copy_depth_frame_buffer(nullptr)
		, m_frame_buffer(nullptr)
		, m_clear_color_command(nullptr)
		, m_copy_color_command(nullptr)
		, m_copy_depth_command(nullptr)
	{ }


	TerrainDepth::~TerrainDepth()
	{
		delete m_render_state;
	}

	FrameBuffer* TerrainDepth::get_frame_buffer()
	{
		return m_frame_buffer.get();
	}

	Texture2D* TerrainDepth::get_depth_stencil_texture()
	{
		return m_depth_stencil_texture.get();
	}

	Texture2D* TerrainDepth::get_color_texture()
	{
		return m_color_texture.get();
	}

	void TerrainDepth::update(
		Context* context,
		const Rectangle& viewport)
	{
		const unsigned int width  = viewport.get_width();
		const unsigned int height = viewport.get_height();

		update_frame_buffers(context, width, height);
		update_copy_commands(context, width, height);

		context->get_uniform_state()->set_terrain_depth_texture(nullptr);
	}

	void TerrainDepth::execute_copy_depth(
		Context* context,
		PassState* pass_state)
	{
		if (m_copy_depth_command != nullptr)
		{
			m_copy_depth_command->execute(context, pass_state);
			context->get_uniform_state()->set_terrain_depth_texture(m_terrain_depth_texture.get());
		}
	}

	void TerrainDepth::execute_copy_color(
		Context* context,
		PassState* pass_state)
	{
		if (m_copy_color_command != nullptr)
		{
			m_copy_color_command->execute(context, pass_state);
		}
	}

	void TerrainDepth::clear(
		Context* context,
		PassState* pass_state,
		const Color& clear_color)
	{
		if (m_clear_color_command != nullptr)
		{
			m_clear_color_command->color = clear_color;
			m_clear_color_command->execute(context, pass_state);
		}
	}

	void TerrainDepth::create_textures(
		Context* context,
		const unsigned int& width,
		const unsigned int& height)
	{
		m_color_texture = Device::create_texture2D(
			TextureDescription(
				width,
				height,
				TextureFormat::RedGreenBlueAlpha8));

		m_depth_stencil_texture = Device::create_texture2D(
			TextureDescription(
				width,
				height,
				TextureFormat::Depth24));

		m_terrain_depth_texture = Device::create_texture2D(
			TextureDescription(
				width,
				height,
				TextureFormat::RedGreenBlueAlpha8));
	}

	void TerrainDepth::create_frame_buffers(
		Context* context,
		const unsigned int& width,
		const unsigned int& height)
	{
		m_frame_buffer = context->create_frame_buffer();
		m_frame_buffer->get_color_attachments()->set_attachment(0, m_color_texture.get());
		m_frame_buffer->set_depth_attachment_texture(m_depth_stencil_texture.get());
			
		m_copy_depth_frame_buffer = context->create_frame_buffer();
		m_copy_depth_frame_buffer->get_color_attachments()->set_attachment(0, m_terrain_depth_texture.get());
	}

	void TerrainDepth::update_frame_buffers(
		Context* context,
		const unsigned int& width,
		const unsigned int& height)
	{
		bool texture_changed =
			m_color_texture == nullptr ||
			m_color_texture->get_description().get_width() != width ||
			m_color_texture->get_description().get_height() != height;

		if (m_frame_buffer == nullptr || texture_changed)
		{
			create_textures(context, width, height);
			create_frame_buffers(context, width, height);
		}
	}

	void TerrainDepth::update_copy_commands(
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
				nullptr,
				new UniformMap 
			{
				{
					"u_texture",
					[&](IUniform* uniform)
				{ static_cast<UniformSampler*>(uniform)->set_texture(m_depth_stencil_texture.get()); }
				}
			}
			));
		}

		m_copy_depth_command->set_framebuffer(m_copy_depth_frame_buffer.get());

		if (m_copy_color_command == nullptr) 
		{
			m_copy_color_command = std::unique_ptr<DrawCommand>(context->create_viewport_quad_command(
				pass_through,
				nullptr,
				new UniformMap
			{
				{
					"u_texture",
					[&](IUniform* uniform)
				{ static_cast<UniformSampler*>(uniform)->set_texture(m_color_texture.get()); }
				}
			}
			));
		}

		m_copy_depth_command->set_render_state(m_render_state);
		m_copy_color_command->set_render_state(m_render_state);

		if (m_clear_color_command == nullptr) 
		{
			m_clear_color_command = std::unique_ptr<ClearCommand>(new ClearCommand());
			m_clear_color_command->color   = Color(0, 0, 0, 0);
			m_clear_color_command->stencil = 0.0;
		}

		m_clear_color_command->frame_buffer = m_frame_buffer.get();
	}
}
