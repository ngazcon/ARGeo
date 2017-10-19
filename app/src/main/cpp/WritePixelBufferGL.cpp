#include "WritePixelBufferGL.h"
#include "TypeConverterGL.h"

#include "GL.h"

namespace argeo
{
	WritePixelBufferGL::WritePixelBufferGL(
		const PixelBufferHint& hint,
		const std::size_t& size_in_bytes)
		: m_buffer_object(new PixelBufferGL(size_in_bytes, BufferTarget::PixelUnPack, TypeConverterGL::WritePixelHintToBufferHint(hint)))
		, m_hint(hint)
	{ }

	WritePixelBufferGL::~WritePixelBufferGL()
	{ }


	void WritePixelBufferGL::bind()
	{
		m_buffer_object->bind();
	}

	void WritePixelBufferGL::unbind()
	{
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	}

	void WritePixelBufferGL::copy_from_system(
		const void* buffer,
		const unsigned int& offset_in_bytes,
		const std::size_t& size_in_bytes)
	{
		m_buffer_object->copy_from_system(buffer, offset_in_bytes, size_in_bytes);
	}

	void WritePixelBufferGL::copy_from_device(
		void* buffer,
		const unsigned int& offset_in_bytes,
		const std::size_t& size_in_bytes)
	{
		m_buffer_object->copy_from_device(buffer, offset_in_bytes, size_in_bytes);
	}

	void* WritePixelBufferGL::map_range(
		const unsigned int& offset_in_bytes,
		const std::size_t& size_in_bytes,
		const BufferAccess& access)
	{
		bind();
		return m_buffer_object->map_range(
			offset_in_bytes,
			size_in_bytes,
			access);
	}

	std::size_t WritePixelBufferGL::get_size_in_bytes() const
	{
		return m_buffer_object->get_size_in_bytes();
	}

	void WritePixelBufferGL::unmap()
	{
		m_buffer_object->unmap();
		WritePixelBufferGL::unbind();
	}

	PixelBufferHint WritePixelBufferGL::get_hint() const
	{
		return m_hint;
	}

	unsigned int WritePixelBufferGL::get_handle() const
	{
		return m_buffer_object->get_handle();
	}

	void WritePixelBufferGL::copy_from_bitmap(Bitmap* bitmap)
	{
		m_buffer_object->copy_from_bitmap(bitmap);
	}

	std::unique_ptr<Bitmap> WritePixelBufferGL::copy_to_bitmap(
		const unsigned int& width,
		const unsigned int& height,
		const PixelFormat& pixel_format)
	{
		return m_buffer_object->copy_to_bitmap(width, height, pixel_format);
	}
}