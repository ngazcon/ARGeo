#include "PixelBufferGL.h"
#include "TypeConverterGL.h"
#include "BitmapAlgorithms.h"
#include "Vector.h"

#include "GL.h"

namespace argeo
{
	PixelBufferGL::PixelBufferGL(
		const std::size_t& size_in_bytes,
		const BufferTarget& target, 
		const BufferHint& hint)
		: m_size_in_bytes(size_in_bytes)
		, m_target(target)
		, m_hint(hint)
	{
		GLenum target_buffer = TypeConverterGL::ToEnum(m_target);
		GLenum buffer_hint   = TypeConverterGL::ToEnum(m_hint);

		// Generate the pbo.
		glGenBuffers(1, &m_handle);

		// Bind to the recently created pbo.
		glBindBuffer(target_buffer, m_handle);

		// Allocate device memory for the vertex buffer.
		glBufferData(
			target_buffer,
			m_size_in_bytes,
			nullptr,
			buffer_hint);

		// Unbind the pbo from the array buffer.
		glBindBuffer(target_buffer, 0);
	}

	PixelBufferGL::~PixelBufferGL()
	{
		glDeleteBuffers(1, &m_handle);
	}

	void PixelBufferGL::bind()
	{
		GLenum target_buffer = TypeConverterGL::ToEnum(m_target);
		glBindBuffer(target_buffer, m_handle);
	}

	void PixelBufferGL::unbind()
	{
		GLenum target_buffer = TypeConverterGL::ToEnum(m_target);
		glBindBuffer(target_buffer, 0);
	}

	void PixelBufferGL::copy_from_system(
		const void* buffer, 
		const unsigned int& offset_in_bytes,
		const std::size_t& size_in_bytes)
	{
		if (offset_in_bytes + size_in_bytes > m_size_in_bytes)
			throw std::out_of_range("offset_in_bytes + size_in_bytes must be less than or equal to SizeInBytes.");

		GLenum target_buffer = TypeConverterGL::ToEnum(m_target);

		// Bind the pbo
		bind();

		// Load the data to device memory.
		void* ptr = glMapBufferRange(
			target_buffer,
			offset_in_bytes,
			size_in_bytes,
			GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

		memcpy(ptr, buffer, size_in_bytes);

		glUnmapBuffer(target_buffer);

		unbind();
	}

	void PixelBufferGL::copy_from_device(
		void* buffer,
		const unsigned int& offset_in_bytes,
		const std::size_t& size_in_bytes)
	{
		if (offset_in_bytes + size_in_bytes > m_size_in_bytes)
			throw std::out_of_range("offset_in_bytes + size_in_bytes must be less than or equal to SizeInBytes.");

		GLenum target_buffer = TypeConverterGL::ToEnum(m_target);

		// Bind the pbo
		bind();

		void* ptr = glMapBufferRange(
			target_buffer,
			offset_in_bytes,
			size_in_bytes,
			GL_MAP_READ_BIT);

		memcpy(buffer, ptr, size_in_bytes);

		glUnmapBuffer(target_buffer);

		unbind();
	}	

	void* PixelBufferGL::map_range(
		const unsigned int& offset_in_bytes,
		const std::size_t& size_in_bytes,
		const BufferAccess& access)
	{
		bind();

		return glMapBufferRange(
			TypeConverterGL::ToEnum(m_target),
			offset_in_bytes,
			size_in_bytes,
			TypeConverterGL::ToEnum(access));
	}

	void PixelBufferGL::unmap()
	{
		glUnmapBuffer(TypeConverterGL::ToEnum(m_target));
		PixelBufferGL::unbind();
	}

	void PixelBufferGL::copy_from_bitmap(Bitmap* bitmap)
	{
		unsigned int size_in_bytes = bitmap->get_size_in_bytes();

		if (size_in_bytes > m_size_in_bytes)
		{
			throw std::invalid_argument(
				"bitmap's size in bytes must be less than or equal to SizeInBytes.");
		}

		GLenum target_buffer = TypeConverterGL::ToEnum(m_target);

		bind();

		void* ptr = glMapBufferRange(
			target_buffer,
			0,
			size_in_bytes,
			GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

		bitmap->get_pixels(static_cast<unsigned char*>(ptr));

		glUnmapBuffer(target_buffer);

		unbind();
	}

	std::unique_ptr<Bitmap> PixelBufferGL::copy_to_bitmap(
		const unsigned int& width,
		const unsigned int& height,
		const PixelFormat& pixel_format)
	{
		if (width <= 0)
		{
			throw std::out_of_range("width must be greater than zero.");
		}

		if (height <= 0)
		{
			throw std::out_of_range("height must be greater than zero.");
		}

		Bitmap* bitmap = new Bitmap(width, height, pixel_format);

		int size_in_bytes = bitmap->get_size_in_bytes();
		GLenum target_buffer = TypeConverterGL::ToEnum(m_target);

		bind();

		void* ptr = glMapBufferRange(
			target_buffer,
			0,
			size_in_bytes,
			GL_MAP_READ_BIT);

		bitmap->set_pixels(
			static_cast<unsigned char*>(ptr));

		glUnmapBuffer(target_buffer);

		return std::unique_ptr<Bitmap>(bitmap);
	}

	std::size_t PixelBufferGL::get_size_in_bytes() const
	{
		return m_size_in_bytes;
	}

	BufferHint PixelBufferGL::get_hint() const
	{
		return m_hint;
	}

	unsigned int PixelBufferGL::get_handle() const
	{
		return m_handle;
	}
}