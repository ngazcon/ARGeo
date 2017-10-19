#pragma once

#include "FrameBuffer.h"
#include "ColorAttachmentsGL.h"
#include "FrameBufferAttachment.h"

#include "Texture2DArray.h"
#include "Texture2D.h"

#include <memory>

namespace argeo
{
	
	class FrameBufferGL : public FrameBuffer
	{
		friend class ContextGL;
		friend class ContextEGL;

		friend class TextureExternal;
		friend class Texture2DArrayGL;
		friend class Texture2DGL;

	public:

		enum class DirtyFlags : unsigned int
		{
			None							   = 1 << 0,
			DepthAttachmentTexture			   = 1 << 1,
			StencilAttachmentTexture		   = 1 << 2,
			DepthStencilAttachmentTexture	   = 1 << 3,
			DepthAttachmentRenderBuffer		   = 1 << 4,
			StencilAttachmentRenderBuffer	   = 1 << 5,
			DepthStencilAttachmentRenderBuffer = 1 << 6,
		};

		//enum class Target : unsigned int
		//{
		//	Read,
		//	Write,
		//	ReadAndWrite
		//};

	protected:
		FrameBufferGL();

	public:
		~FrameBufferGL();

		void bind() const;
		static void unbind();
		void clean();

		FrameBufferStatus get_status();

		ColorAttachments* get_color_attachments();

		Texture* get_stencil_attachment_texture() const;
		void set_stencil_attachment_texture(Texture* value);

		Texture* get_depth_attachment_texture() const;
		void set_depth_attachment_texture(Texture* value);

		Texture* get_depth_stencil_attachment_texture() const;
		void set_depth_stencil_attachment_texture(Texture* value);

		RenderBuffer* get_depth_attachment_render_buffer() const;
		void set_depth_attachment_render_buffer(RenderBuffer* value);

		RenderBuffer* get_stencil_attachment_render_buffer() const;
		void set_stencil_attachment_render_buffer(RenderBuffer* value);

		RenderBuffer* get_depth_stencil_attachment_render_buffer() const;
		void set_depth_stencil_attachment_render_buffer(RenderBuffer* value);

		static void attach(FrameBufferAttachment attach_point, const RenderBuffer * render_buffer);
		static void attach(FrameBufferAttachment attach_point, const Texture * texture, const int layer);
		static void attach(FrameBufferAttachment attach_point, const Texture * texture);

		unsigned int get_handle() const;

	private:

		unsigned int m_handle;

		std::unique_ptr<ColorAttachmentsGL> m_color_attachments;

		Texture* m_depth_attachment_texture;
		Texture* m_stencil_attachment_texture;
		Texture* m_depth_stencil_attachment_texture;

		RenderBuffer* m_stencil_attachment_render_buffer;
		RenderBuffer* m_depth_attachment_render_buffer;
		RenderBuffer* m_depth_stencil_attachment_render_buffer;

		DirtyFlags m_dirty_flags;
	};

	inline FrameBufferGL::DirtyFlags operator|(FrameBufferGL::DirtyFlags a, FrameBufferGL::DirtyFlags b)
	{
		return static_cast<FrameBufferGL::DirtyFlags>(static_cast<int>(a) | static_cast<int>(b));
	}

	inline FrameBufferGL::DirtyFlags operator&(FrameBufferGL::DirtyFlags a, FrameBufferGL::DirtyFlags b)
	{
		return static_cast<FrameBufferGL::DirtyFlags>(static_cast<int>(a) & static_cast<int>(b));
	}

	inline FrameBufferGL::DirtyFlags operator|=(FrameBufferGL::DirtyFlags &a, const FrameBufferGL::DirtyFlags &b)
	{
		a = static_cast<FrameBufferGL::DirtyFlags>(static_cast<int>(a) | static_cast<int>(b));
		return a;
	}

	inline FrameBufferGL::DirtyFlags operator&=(FrameBufferGL::DirtyFlags &a, const FrameBufferGL::DirtyFlags &b)
	{
		a = static_cast<FrameBufferGL::DirtyFlags>(static_cast<int>(a) & static_cast<int>(b));
		return a;
	}
}
