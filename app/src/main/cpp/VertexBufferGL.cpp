#include "VertexBufferGL.h"

#include "GL.h"

namespace argeo
{
	VertexBufferGL::VertexBufferGL(
		const std::size_t& size_in_bytes,
		const BufferHint& hint)
		: m_buffer_object(new BufferGL(BufferTarget::VertexArray, hint, size_in_bytes))
	{ }

	VertexBufferGL::~VertexBufferGL()
	{ }

	void VertexBufferGL::copy_from_system(
		const void* buffer, 
		const unsigned int& offset_in_bytes,
		const std::size_t& size_in_bytes)
	{
		m_buffer_object->copy_from_system(buffer, offset_in_bytes, size_in_bytes);
	}

	void VertexBufferGL::copy_from_device(
		void* buffer, 
		const unsigned int& offset_in_bytes,
		const std::size_t& size_in_bytes)
	{
		m_buffer_object->copy_from_device(buffer, offset_in_bytes, size_in_bytes);
	}

	/* ****************************************************
	 *						GETTERS
	 * ****************************************************/

	unsigned int VertexBufferGL::get_handle() const
	{
		return m_buffer_object->get_handle();
	}

	std::size_t VertexBufferGL::get_size_in_bytes() const
	{
		return m_buffer_object->get_size_in_bytes();
	}

	BufferHint VertexBufferGL::get_hint() const
	{
		return m_buffer_object->get_hint();
	}
}