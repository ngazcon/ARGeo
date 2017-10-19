#include "BufferGL.h"
#include "TypeConverterGL.h"

#include <stdexcept>

#include "GL.h"

namespace argeo
{
	BufferGL::BufferGL(
		const BufferTarget& target, 
		const BufferHint& hint, 
		const std::size_t& size_in_bytes)
		: m_target(target)
		, m_hint(hint)
		, m_size_in_bytes(size_in_bytes)
	{
		if (size_in_bytes <= 0)
		{
			throw std::out_of_range("sizeInBytes must be greater than zero.");
		}

		GLenum target_buffer = TypeConverterGL::ToEnum(m_target);
		GLenum buffer_hint   = TypeConverterGL::ToEnum(m_hint);

		// Generate the vbo.
		glGenBuffers(1, &m_handle);

		// Bind to the recently created vbo.
		glBindBuffer(target_buffer, m_handle);

		// Allocate device memory for the vertex buffer.
		glBufferData(
			target_buffer,
			m_size_in_bytes,
			nullptr,
			buffer_hint);

		// Unbind the vbo from the array buffer.
		glBindBuffer(target_buffer, 0);
	}

	BufferGL::~BufferGL()
	{
		// Delete the buffer from device memory.
		glDeleteBuffers(1, &m_handle);
	}

	void BufferGL::bind()
	{
		GLenum target_buffer = TypeConverterGL::ToEnum(m_target);
		glBindBuffer(target_buffer, m_handle);
	}

	void BufferGL::unbind()
	{
		GLenum target_buffer = TypeConverterGL::ToEnum(m_target);
		glBindBuffer(target_buffer, 0);
	}

	void BufferGL::unbind(BufferTarget target)
	{
		GLenum target_buffer = TypeConverterGL::ToEnum(target);
		glBindBuffer(target_buffer, 0);
	}

	void BufferGL::copy_from_system(
		const void* buffer,
		const unsigned int&  offset_in_bytes,
		const std::size_t& size_in_bytes)
	{
		if (offset_in_bytes + size_in_bytes > m_size_in_bytes)
		{
			throw std::out_of_range("offset_in_bytes + size_in_bytes must be less than or equal to size_in_bytes of this buffer.");
		}

		GLenum target_buffer = TypeConverterGL::ToEnum(m_target);

		// To ensure that a vertex array is not modified.
		glBindVertexArray(0);

		// Bind to the vbo.
		glBindBuffer(target_buffer, m_handle);

		// Load the data to device memory.
		void* ptr = glMapBufferRange(
			target_buffer,
			offset_in_bytes,
			size_in_bytes,
			GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

		memcpy(ptr, buffer, size_in_bytes);

		glUnmapBuffer(target_buffer);

		//glBufferSubData(
		//	target_buffer,
		//	offset_in_bytes,
		//	size_in_bytes,
		//	buffer);

		// Unbind the vbo.
		glBindBuffer(target_buffer, 0);
	}

	void BufferGL::copy_from_device(
		void* buffer, 
		const unsigned int&  offset_in_bytes,
		const std::size_t& size_in_bytes)
	{
		if (offset_in_bytes + size_in_bytes > m_size_in_bytes)
		{
			throw std::out_of_range("offset_in_bytes + size_in_bytes must be less than or equal to size_in_bytes of this buffer.");
		}

		GLenum target_buffer = TypeConverterGL::ToEnum(m_target);

		// To ensure that a vertex array is not modified.
		glBindVertexArray(0);

		// Bind to the vbo.
		glBindBuffer(target_buffer, m_handle);

		// Load the data to system memory.
		buffer = glMapBufferRange(
			target_buffer,
			offset_in_bytes,
			size_in_bytes,
			GL_MAP_READ_BIT);

		glUnmapBuffer(target_buffer);

		// Unbind the vbo.
		glBindBuffer(target_buffer, 0);
	}

	void* BufferGL::map_range(
		const unsigned int&  offset_in_bytes,
		const std::size_t&  size_in_bytes,
		const BufferAccess& access)
	{
		return glMapBufferRange(
			TypeConverterGL::ToEnum(m_target),
			offset_in_bytes,
			size_in_bytes,
			TypeConverterGL::ToEnum(access));
	}

	void BufferGL::unmap()
	{
		glUnmapBuffer(TypeConverterGL::ToEnum(m_target));
	}

	/* ****************************************************
	 *						GETTERS
	 * ****************************************************/

	unsigned int BufferGL::get_handle() const
	{
		return m_handle;
	}

	std::size_t BufferGL::get_size_in_bytes() const
	{
		return m_size_in_bytes;
	}

	BufferHint BufferGL::get_hint() const
	{
		return m_hint;
	}

	BufferTarget BufferGL::get_target() const
	{
		return m_target;
	}
}