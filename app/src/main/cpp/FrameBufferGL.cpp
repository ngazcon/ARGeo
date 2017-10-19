#include "FrameBufferGL.h"
#include "TypeConverterGL.h"
#include "Texture2DGL.h"
#include "RenderBufferGL.h"
#include "Log.h"

#include "GL.h"

namespace argeo
{
	FrameBufferGL::FrameBufferGL()
		: m_dirty_flags(DirtyFlags::None)
		, m_stencil_attachment_texture(nullptr)
		, m_depth_attachment_texture(nullptr)
		, m_depth_stencil_attachment_texture(nullptr)
		, m_depth_attachment_render_buffer(nullptr)
		, m_stencil_attachment_render_buffer(nullptr)
		, m_depth_stencil_attachment_render_buffer(nullptr)
		, m_color_attachments(new ColorAttachmentsGL())
	{
		glGenFramebuffers(1, &m_handle);
	}


	FrameBufferGL::~FrameBufferGL()
	{
		glDeleteFramebuffers(1, &m_handle);
	}

	void FrameBufferGL::bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
	}

	void FrameBufferGL::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	FrameBufferStatus FrameBufferGL::get_status()
	{
		return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE ?
			FrameBufferStatus::Complete :
			FrameBufferStatus::Incomplete;
	}

	void FrameBufferGL::clean()
	{
		if (m_dirty_flags != DirtyFlags::None)
		{
			if ((m_dirty_flags & DirtyFlags::DepthAttachmentTexture) == DirtyFlags::DepthAttachmentTexture)
			{
				attach(FrameBufferAttachment::DepthAttachment, m_depth_attachment_texture);
			}

			if ((m_dirty_flags & DirtyFlags::DepthAttachmentRenderBuffer) == DirtyFlags::DepthAttachmentRenderBuffer)
			{
				attach(FrameBufferAttachment::DepthAttachment, m_depth_attachment_render_buffer);
			}

			if ((m_dirty_flags & DirtyFlags::StencilAttachmentTexture) == DirtyFlags::StencilAttachmentTexture)
			{
				attach(FrameBufferAttachment::StencilAttachment, m_stencil_attachment_texture);
			}

			if ((m_dirty_flags & DirtyFlags::StencilAttachmentRenderBuffer) == DirtyFlags::StencilAttachmentRenderBuffer)
			{
				attach(FrameBufferAttachment::StencilAttachment, m_stencil_attachment_render_buffer);
			}

			// The depth-stencil attachment overrides the depth attachment:
			//
			//    "Attaching a level of a texture to GL_DEPTH_STENCIL_ATTACHMENT 
			//     is equivalent to attaching that level to both the 
			//     GL_DEPTH_ATTACHMENT and the GL_STENCIL_ATTACHMENT attachment 
			//     points simultaneously."
			//
			// We do not expose just a stencil attachment because TextureFormat
			// does not contain a stencil only format.
			if ((m_dirty_flags & DirtyFlags::DepthStencilAttachmentTexture) == DirtyFlags::DepthStencilAttachmentTexture)
			{
				attach(
					FrameBufferAttachment::DepthStencilAttachment, 
					m_depth_stencil_attachment_texture);
			}

			if ((m_dirty_flags & DirtyFlags::DepthStencilAttachmentRenderBuffer) == DirtyFlags::DepthStencilAttachmentRenderBuffer)
			{
				attach(
					FrameBufferAttachment::DepthStencilAttachment,
					m_depth_stencil_attachment_render_buffer);
			}

			m_dirty_flags = DirtyFlags::None;
		}

		if (m_color_attachments->get_dirty())
		{
			std::vector<ColorAttachmentGL> color_attachments = m_color_attachments->get_attachments();

			std::unique_ptr<GLenum> draw_buffers(new GLenum[m_color_attachments->get_maximum_attachments()]);

			int draw_buffers_count = 0;

			for (int i = 0; i < m_color_attachments->get_maximum_attachments(); ++i)
			{
				if (color_attachments[i].dirty)
				{
					attach(
						FrameBufferAttachment::ColorAttachment0 + i, 
						color_attachments[i].texture);

					color_attachments[i].dirty = false;
				}

				if (color_attachments[i].texture != nullptr)
				{
					draw_buffers.get()[i] = GL_COLOR_ATTACHMENT0 + i;
					draw_buffers_count++;
				}
			}

			if (get_status() != FrameBufferStatus::Complete)
			{
				throw std::runtime_error("Error binding the framebuffer.");
			}

			glDrawBuffers(
				draw_buffers_count, 
				draw_buffers.get());

			m_color_attachments->set_dirty(false);
		}		
	}

	ColorAttachments* FrameBufferGL::get_color_attachments()
	{
		return m_color_attachments.get();
	}

	Texture* FrameBufferGL::get_depth_attachment_texture() const
	{
		return m_depth_attachment_texture;
	}

	void FrameBufferGL::set_depth_attachment_texture(Texture* value)
	{
		if (m_depth_attachment_texture != value)
		{
			if (m_depth_attachment_render_buffer != nullptr  && value != nullptr)
			{
				throw std::invalid_argument(
					"Cannot have both a depth texture and depth renderbuffer attachment.");
			}

			if ((value != nullptr) && (!value->get_description().get_depth_renderable()))
			{
				throw std::invalid_argument(
					"Texture must be depth renderable but the Description.DepthRenderable property is false.");
			}

			m_depth_attachment_texture = value;
			m_dirty_flags     |= DirtyFlags::DepthAttachmentTexture;
		}
	}

	Texture* FrameBufferGL::get_depth_stencil_attachment_texture() const
	{
		return m_depth_stencil_attachment_texture;
	}

	void FrameBufferGL::set_stencil_attachment_texture(Texture* value)
	{
		if (m_stencil_attachment_texture != value)
		{
			if ((m_stencil_attachment_render_buffer != nullptr || m_depth_stencil_attachment_render_buffer != nullptr || m_depth_stencil_attachment_texture != nullptr) &&
				value != nullptr)
			{
				throw std::invalid_argument(
					"Cannot have both a stencil texture and depth renderbuffer attachment.");
			}

			if ((value != nullptr) && (!value->get_description().get_stencil_renderable()))
			{
				throw std::invalid_argument(
					"Texture must be stencil renderable but the Description.StencilRenderable property is false.");
			}

			m_stencil_attachment_texture = value;
			m_dirty_flags |= DirtyFlags::StencilAttachmentTexture;
		}
	}

	Texture* FrameBufferGL::get_stencil_attachment_texture() const
	{
		return m_stencil_attachment_texture;
	}

	void FrameBufferGL::set_depth_stencil_attachment_texture(Texture* value)
	{
		if (m_depth_stencil_attachment_texture != value)
		{
			if (m_depth_stencil_attachment_render_buffer != nullptr && value != nullptr)
			{
				throw std::invalid_argument(
					"Cannot have both a depth-stencil texture and depth-stencil renderbuffer attachment.");
			}

			if ((value != nullptr) && (!value->get_description().get_depth_stencil_renderable()))
			{
				throw std::invalid_argument("Texture must be depth/stencil renderable but the Description.DepthStencilRenderable property is false.");
			}

			m_depth_stencil_attachment_texture = value;
			m_dirty_flags |= DirtyFlags::DepthStencilAttachmentTexture;
		}
	}

	RenderBuffer* FrameBufferGL::get_depth_attachment_render_buffer() const
	{
		return m_depth_attachment_render_buffer;
	}

	void FrameBufferGL::set_stencil_attachment_render_buffer(RenderBuffer* value)
	{
		if (m_stencil_attachment_render_buffer != value)
		{
			if ((m_stencil_attachment_texture != nullptr || m_depth_stencil_attachment_render_buffer != nullptr || m_depth_stencil_attachment_texture != nullptr) &&
				value != nullptr)
			{
				throw std::invalid_argument(
					"Cannot have both a depth texture and depth renderbuffer attachment.");
			}

			if ((value != nullptr) && (!value->get_description().get_stencil_renderable()))
			{
				throw std::invalid_argument(
					"Render Buffer must be stencil renderable but the Description.StencilRenderable property is false.");
			}

			m_stencil_attachment_render_buffer = value;
			m_dirty_flags |= DirtyFlags::StencilAttachmentRenderBuffer;
		}
	}

	RenderBuffer* FrameBufferGL::get_stencil_attachment_render_buffer() const
	{
		return m_stencil_attachment_render_buffer;
	}

	void FrameBufferGL::set_depth_attachment_render_buffer(RenderBuffer* value)
	{
		if (m_depth_attachment_render_buffer != value)
		{
			if (m_depth_attachment_texture != nullptr && value != nullptr)
			{
				throw std::invalid_argument(
					"Cannot have both a depth texture and depth renderbuffer attachment.");
			}

			if ((value != nullptr) && (!value->get_description().get_depth_renderable()))
			{
				throw std::invalid_argument(
					"Render Buffer must be depth renderable but the Description.DepthRenderable property is false.");
			}

			m_depth_attachment_render_buffer = value;
			m_dirty_flags |= DirtyFlags::DepthAttachmentRenderBuffer;
		}
	}

	RenderBuffer* FrameBufferGL::get_depth_stencil_attachment_render_buffer() const
	{
		return m_depth_stencil_attachment_render_buffer;
	}

	void FrameBufferGL::set_depth_stencil_attachment_render_buffer(RenderBuffer* value)
	{
		if (m_depth_stencil_attachment_render_buffer != value)
		{
			if (m_depth_stencil_attachment_texture != nullptr && value != nullptr)
			{
				throw std::invalid_argument(
					"Cannot have both a depth-stencil texture and depth-stencil renderbuffer attachment.");
			}

			if ((value != nullptr) && (!value->get_description().get_depth_stencil_renderable()))
			{
				throw std::invalid_argument("Render buffer must be depth/stencil renderable but the Description.DepthStencilRenderable property is false.");
			}

			m_depth_stencil_attachment_render_buffer = value;
			m_dirty_flags |= DirtyFlags::DepthStencilAttachmentRenderBuffer;
		}
	}

	unsigned int FrameBufferGL::get_handle() const
	{
		return m_handle;
	}

	void FrameBufferGL::attach(
		FrameBufferAttachment attach_point, 
		const Texture * texture, 
		const int layer)
	{
		if (texture != nullptr)
		{
			// TODO:  Mipmap level
			const TextureGL * texture_gl = dynamic_cast<const TextureGL *>(texture);

			glFramebufferTextureLayer(
				GL_FRAMEBUFFER,
				TypeConverterGL::ToEnum(attach_point),
				texture_gl->get_handle(),
				0,
				layer);
		}
		else
		{
			glFramebufferTextureLayer(
				GL_FRAMEBUFFER,
				TypeConverterGL::ToEnum(attach_point),
				0,
				0,
				layer);
		}
	}

	void FrameBufferGL::attach(
		FrameBufferAttachment attach_point, 
		const Texture * texture)
	{
		if (texture != nullptr)
		{
			// TODO:  Mipmap level
			const TextureGL * texture_gl = dynamic_cast<const TextureGL *>(texture);

			glFramebufferTexture2D(
				GL_FRAMEBUFFER,
				TypeConverterGL::ToEnum(attach_point),
				TypeConverterGL::ToEnum(texture->get_target()),
				texture_gl->get_handle(),
				0);
		}
		else
		{
			glFramebufferTexture2D(
				GL_FRAMEBUFFER,
				TypeConverterGL::ToEnum(attach_point),
				TypeConverterGL::ToEnum(texture->get_target()),
				0,
				0);
		}
	}

	void FrameBufferGL::attach(
		FrameBufferAttachment attach_point,
		const RenderBuffer * render_buffer)
	{
		if (render_buffer != nullptr)
		{
			// TODO:  Mipmap level
			const RenderBufferGL * render_buffer_gl = dynamic_cast<const RenderBufferGL *>(render_buffer);

			glFramebufferRenderbuffer(
				GL_FRAMEBUFFER,
				TypeConverterGL::ToEnum(attach_point),
				GL_RENDERBUFFER,
				render_buffer_gl->get_handle());
		}
		else
		{
			glFramebufferRenderbuffer(
				GL_FRAMEBUFFER,
				TypeConverterGL::ToEnum(attach_point),
				GL_RENDERBUFFER,
				0);
		}
	}
}