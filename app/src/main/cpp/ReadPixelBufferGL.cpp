#include "ReadPixelBufferGL.h"
#include "TypeConverterGL.h"
#include "FrameBufferGL.h"

#include "GL.h"

namespace argeo
{
	ReadPixelBufferGL::ReadPixelBufferGL(
		const PixelBufferHint& hint, 
		const std::size_t& size_in_bytes)
		: m_buffer_object(new PixelBufferGL(size_in_bytes, BufferTarget::PixelPack, TypeConverterGL::ReadPixelHintToBufferHint(hint))),
		  m_hint(hint)
	{ }

	ReadPixelBufferGL::~ReadPixelBufferGL()
	{
        unbind();
    }

	void ReadPixelBufferGL::bind()
	{
		m_buffer_object->bind();
	}

	void ReadPixelBufferGL::unbind()
	{
		glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
	}

	void ReadPixelBufferGL::transfer(
		FrameBuffer* frame_buffer,
		const unsigned int& x,
		const unsigned int& y,
		const unsigned int& width,
		const unsigned int& height,
		const ImageFormat& format,
		const ImageDataType& data_type,
		const FrameBufferAttachment& attachment)
	{
		static_cast<FrameBufferGL*>(frame_buffer)->clean();

		frame_buffer->bind();
		bind();

		glReadBuffer(TypeConverterGL::ToEnum(attachment));
		//glPixelStorei(GL_PACK_ALIGNMENT, 4);
		glReadPixels(
			x,
			y,
			width,
			height,
			TypeConverterGL::ToEnum(format),
			TypeConverterGL::ToEnum(data_type),
			0);

		glReadBuffer(GL_COLOR_ATTACHMENT0);

		unbind();
	}

	void ReadPixelBufferGL::copy_from_system(
		const void* buffer,
		const std::size_t& offset_in_bytes,
		const std::size_t& size_in_bytes)
	{
		m_buffer_object->copy_from_system(buffer, offset_in_bytes, size_in_bytes);
	}

	void ReadPixelBufferGL::copy_from_device(
		void* buffer,
		const std::size_t& offset_in_bytes,
		const std::size_t& size_in_bytes)
	{
		m_buffer_object->copy_from_device(buffer, offset_in_bytes, size_in_bytes);
	}

	void* ReadPixelBufferGL::map_range(
		const std::size_t& offset_in_bytes,
		const std::size_t& size_in_bytes,
		const BufferAccess& access)
	{
		return m_buffer_object->map_range(
			offset_in_bytes,
			size_in_bytes,
			access);
	}

	void ReadPixelBufferGL::unmap()
	{
		m_buffer_object->unmap();
	}

	std::size_t ReadPixelBufferGL::get_size_in_bytes() const
	{
		return m_buffer_object->get_size_in_bytes();
	}

	PixelBufferHint ReadPixelBufferGL::get_hint() const
	{
		return m_hint;
	}

	unsigned int ReadPixelBufferGL::get_handle() const
	{
		return m_buffer_object->get_handle();
	}
	
}