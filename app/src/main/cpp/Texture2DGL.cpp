#include "Texture2DGL.h"
#include "WritePixelBufferGL.h"
#include "ReadPixelBufferGL.h"
#include "TextureHelper.h"
#include "TextureSamplers.h"
#include "FrameBufferGL.h"
#include "Device.h"

#include "GL.h"

namespace argeo
{
	Texture2DGL::Texture2DGL(
		TextureDescription description, 
		TextureTarget texture_target,
		TextureSampler* sampler)
		: m_description(description)
		, m_target(texture_target)
	{
		if (description.get_generate_mipmaps())
		{
			if (texture_target == TextureTarget::TextureRectangle)
			{
				throw std::invalid_argument("GenerateMipmaps cannot be true for texture rectangles.");
			}
		}

		// Create the texture in device memory.
		glGenTextures(1, &m_handle);

		m_last_texture_unit = GL_TEXTURE0 + Device::get_number_of_texture_units() - 1;

		// TexImage2D is just used to allocate the texture so a PBO can't be bound.
		WritePixelBufferGL::unbind();
		bind_to_last_texture_unit();

		glTexImage2D(
			TypeConverterGL::ToEnum(m_target),
			0,
			TypeConverterGL::ToEnum(description.get_texture_format()),
			description.get_width(),
			description.get_height(),
			0,
			TypeConverterGL::TextureToPixelFormat(description.get_texture_format()),
			TypeConverterGL::TextureToPixelType(description.get_texture_format()),
			0);

		// Default sampler, compatiable when attaching a non-mimapped 
		// texture to a frame buffer object.
		set_sampler(sampler != nullptr ?
			sampler :
			Device::get_texture_samplers()
			->get_linear_clamp());
	}


	Texture2DGL::~Texture2DGL()
	{
		// Delete the texture from device memory.
		glDeleteTextures(1, &m_handle);
	}

	void Texture2DGL::set_sampler(TextureSampler* value)
	{
		m_sampler = static_cast<TextureSamplerGL*>(value);
		apply_sampler(value);
	}

	TextureSampler* Texture2DGL::get_sampler()
	{
		return m_sampler;
	}

	void Texture2DGL::apply_sampler(TextureSampler * sampler)
	{
		glTexParameterf(
			TypeConverterGL::ToEnum(m_target),
			GL_TEXTURE_MIN_FILTER,
			TypeConverterGL::ToEnum(sampler->get_minification_filter()));

		glTexParameterf(
			TypeConverterGL::ToEnum(m_target),
			GL_TEXTURE_MAG_FILTER,
			TypeConverterGL::ToEnum(sampler->get_magnification_filter()));

		glTexParameteri(
			TypeConverterGL::ToEnum(m_target),
			GL_TEXTURE_WRAP_S,
			TypeConverterGL::ToEnum(sampler->get_wrap_s()));

		glTexParameteri(
			TypeConverterGL::ToEnum(m_target),
			GL_TEXTURE_WRAP_T,
			TypeConverterGL::ToEnum(sampler->get_wrap_t()));
	}

	void Texture2DGL::generate_mipmaps() const
	{
		if (m_description.get_generate_mipmaps())
		{
			glGenerateMipmap(TypeConverterGL::ToEnum(m_target));
		}
	}

	void Texture2DGL::bind() const
	{
		glBindTexture(
			TypeConverterGL::ToEnum(m_target),
			m_handle);
	}

	void Texture2DGL::bind_to_last_texture_unit() const
	{
		glActiveTexture(m_last_texture_unit);
		bind();
	}

	void Texture2DGL::read_pixel(
		void* pixel,
		int x_offset,
		int y_offset,
		ImageFormat format,
		ImageDataType data_type) const
	{
		if (format == ImageFormat::StencilIndex)
		{
			throw std::invalid_argument(
				"StencilIndex is not supported by CopyToBuffer.  Try DepthStencil instead.");
		}

		std::unique_ptr<FrameBufferGL> framebuffer(new FrameBufferGL());

		if (m_description.get_depth_renderable())
		{
			framebuffer->set_depth_attachment_texture(const_cast<Texture2DGL*>(this));
		}
		else if (m_description.get_depth_stencil_renderable())
		{
			framebuffer->set_depth_stencil_attachment_texture(const_cast<Texture2DGL*>(this));
		}
		else
		{
			framebuffer->get_color_attachments()->set_attachment(0, const_cast<Texture2DGL*>(this));
		}		

		framebuffer->bind();
		framebuffer->clean();
        //ReadPixelBufferGL::unbind();

		// Read from the currently framebuffer using the pbo.
		// Specifies the format of the pixel data. The following symbolic values are accepted: GL_ALPHA, GL_RGB, and GL_RGBA. 
		// Type : Specifies the data type of the pixel data. Must be one of GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_4_4_4_4, or GL_UNSIGNED_SHORT_5_5_5_1. 
		glReadPixels(
			x_offset,
			y_offset,
			1,
			1,
			TypeConverterGL::ToEnum(format),
			TypeConverterGL::ToEnum(data_type),
			pixel);

		FrameBufferGL::unbind();
	}

	void Texture2DGL::copy_from_buffer(
		WritePixelBuffer* pixel_buffer,
		ImageFormat format,
		ImageDataType data_type) const
	{
		copy_from_buffer(
			pixel_buffer,
			0,
			0,
			m_description.get_width(),
			m_description.get_height(),
			format, 
			data_type,
			4);
	}

	void Texture2DGL::copy_from_buffer(
		WritePixelBuffer* pixel_buffer,
		ImageFormat format,
		ImageDataType data_type,
		int row_alignment) const
	{
		copy_from_buffer(
			pixel_buffer,
			0, 
			0, 
			m_description.get_width(),
			m_description.get_height(),
			format, 
			data_type,
			row_alignment);
	}


	void Texture2DGL::copy_from_buffer(
		WritePixelBuffer* pixel_buffer,
		int x_offset,
		int y_offset,
		int width,
		int height,
		ImageFormat format,
		ImageDataType data_type,
		int row_alignment) const
	{
		if (pixel_buffer->get_size_in_bytes() <
			TextureHelper::required_size_in_bytes(width, height, format, data_type, row_alignment))
		{
			throw std::invalid_argument("Pixel buffer is not big enough for provided width, height, format, and datatype.");
		}

		if (x_offset < 0)
		{
			throw std::out_of_range("xOffset must be greater than or equal to zero.");
		}

		if (y_offset < 0)
		{
			throw std::out_of_range("yOffset must be greater than or equal to zero.");
		}

		if (x_offset + width > m_description.get_width())
		{
			throw std::out_of_range("xOffset + width must be less than or equal to Description.Width");
		}

		if (y_offset + height > m_description.get_height())
		{
			throw std::out_of_range("yOffset + height must be less than or equal to Description.Height");
		}

		verify_row_alignment(row_alignment);

		WritePixelBufferGL* buffer_object_GL = static_cast<WritePixelBufferGL*>(pixel_buffer);

		buffer_object_GL->bind();
		bind_to_last_texture_unit();

		glPixelStorei(GL_UNPACK_ALIGNMENT, row_alignment);

		glTexSubImage2D(
			TypeConverterGL::ToEnum(m_target),
			0,
			x_offset,
			y_offset,
			width,
			height,
			TypeConverterGL::ToEnum(format),
			TypeConverterGL::ToEnum(data_type),
			nullptr);

		generate_mipmaps();
	}

	std::unique_ptr<ReadPixelBuffer> Texture2DGL::copy_to_buffer(
		ImageFormat format,
		ImageDataType data_type) const
	{
		return copy_to_buffer(format, data_type, 4);
	}

	std::unique_ptr<ReadPixelBuffer> Texture2DGL::copy_to_buffer(
		ImageFormat format,
		ImageDataType data_type,
		int row_alignment) const
	{
		if (format == ImageFormat::StencilIndex)
		{
			throw std::invalid_argument(
				"StencilIndex is not supported by CopyToBuffer.  Try DepthStencil instead.");
		}

		verify_row_alignment(row_alignment);

		ReadPixelBufferGL* pixel_buffer = new ReadPixelBufferGL(
			PixelBufferHint::Stream,
			TextureHelper::required_size_in_bytes(
				m_description.get_width(),
				m_description.get_height(),
				format,
				data_type,
				row_alignment));

		std::unique_ptr<FrameBufferGL> framebuffer(new FrameBufferGL());
		framebuffer->bind();
		framebuffer->attach(FrameBufferAttachment::ColorAttachment0, this);

		pixel_buffer->bind();
		glPixelStorei(GL_PACK_ALIGNMENT, row_alignment);

		// Read from the currently framebuffer using the pbo.
		// Specifies the format of the pixel data. The following symbolic values are accepted: GL_ALPHA, GL_RGB, and GL_RGBA. 
		// Type : Specifies the data type of the pixel data. Must be one of GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_4_4_4_4, or GL_UNSIGNED_SHORT_5_5_5_1. 
		glReadPixels(
			0,
			0,
			m_description.get_width(),
			m_description.get_height(),
			TypeConverterGL::ToEnum(format),
			TypeConverterGL::ToEnum(data_type),
			0);

		FrameBufferGL::unbind();

		return std::unique_ptr<ReadPixelBuffer>(pixel_buffer);
	}

	void Texture2DGL::verify_row_alignment(int row_alignment) const
	{
		if ((row_alignment != 1) &&
			(row_alignment != 2) &&
			(row_alignment != 4) &&
			(row_alignment != 8))
		{
			throw std::invalid_argument("row_alignment");
		}
	}

	TextureDescription Texture2DGL::get_description() const
	{
		return m_description;
	}

	TextureTarget Texture2DGL::get_target() const
	{
		return m_target;
	}

	unsigned int Texture2DGL::get_handle() const
	{
		return m_handle;
	}

}