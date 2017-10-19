#pragma once

#include "ColorAttachments.h"

#include "RenderBuffer.h"
#include "FrameBufferStatus.h"
#include "Texture.h"

namespace argeo
{
	class FrameBuffer
	{
	public:
		
		virtual FrameBufferStatus get_status() = 0;

		// Get the color attachments of this framebuffer.
		virtual ColorAttachments* get_color_attachments() = 0;

		// Get the depth attachment texture of this framebuffer.
		virtual Texture* get_depth_attachment_texture() const = 0;

		// Set the depth attachment texture of this framebuffer.
		virtual void set_depth_attachment_texture(Texture* value) = 0;

		// Get the depth stencil attachment texture of this framebuffer.
		virtual Texture* get_depth_stencil_attachment_texture() const = 0;

		// Set the depth attachment texture of this framebuffer.
		virtual void set_stencil_attachment_texture(Texture* value) = 0;

		// Get the depth stencil attachment texture of this framebuffer.
		virtual Texture* get_stencil_attachment_texture() const = 0;

		// Set the depth stencil attachment texture of this framebuffer.
		virtual void set_depth_stencil_attachment_texture(Texture* value) = 0;

		// Get the depth attachment render buffer of this framebuffer.
		virtual RenderBuffer* get_depth_attachment_render_buffer() const = 0;

		// Set the depth attachment render buffer of this framebuffer.
		virtual void set_depth_attachment_render_buffer(RenderBuffer* value) = 0;

		// Get the depth attachment render buffer of this framebuffer.
		virtual RenderBuffer* get_stencil_attachment_render_buffer() const = 0;

		// Set the depth attachment render buffer of this framebuffer.
		virtual void set_stencil_attachment_render_buffer(RenderBuffer* value) = 0;

		// Get the depth stencil attachment render buffer of this framebuffer.
		virtual RenderBuffer* get_depth_stencil_attachment_render_buffer() const = 0;

		// Set the depth stencil attachment render buffer of this framebuffer.
		virtual void set_depth_stencil_attachment_render_buffer(RenderBuffer* value) = 0;

		virtual void bind() const = 0;
	};
}
