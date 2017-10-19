#include "IndexBufferGL.h"

#include "GL.h"

namespace argeo
{
	IndexBufferGL::IndexBufferGL(
		const std::size_t& size_in_bytes,
		const BufferHint& hint)
		: m_buffer_object(new BufferGL(BufferTarget::ElementArray, hint, size_in_bytes))
	{ }

	IndexBufferGL::~IndexBufferGL()
	{ }

	void IndexBufferGL::bind()
	{
		m_buffer_object->bind();
	}

	void IndexBufferGL::unbind()
	{
		BufferGL::unbind(BufferTarget::ElementArray);
	}

	void IndexBufferGL::copy_from_system(
		const unsigned int* buffer, 
		const unsigned int&  offset_in_bytes,
		const std::size_t& size_in_bytes)
	{
		m_buffer_object->copy_from_system(buffer, offset_in_bytes, size_in_bytes);
		m_type = IndexDataType::UnsignedInteger;
	}

	void IndexBufferGL::copy_from_system(
		const unsigned short* buffer, 
		const unsigned int& offset_in_bytes,
		const std::size_t& size_in_bytes)
	{
		m_buffer_object->copy_from_system(buffer, offset_in_bytes, size_in_bytes);
		m_type = IndexDataType::UnsignedShort;
	}

	void IndexBufferGL::copy_from_device(
		void* buffer, 
		const unsigned int& offset_in_bytes,
		const std::size_t& size_in_bytes)
	{
		m_buffer_object->copy_from_device(buffer, offset_in_bytes, size_in_bytes);
	}

	/* ****************************************************
	*						GETTERS
	* ****************************************************/

	std::size_t IndexBufferGL::get_size_in_bytes() const
	{
		return m_buffer_object->get_size_in_bytes();
	}

	BufferHint IndexBufferGL::get_hint() const
	{
		return m_buffer_object->get_hint();
	}

	IndexDataType IndexBufferGL::get_type() const
	{
		return m_type;
	}

	int IndexBufferGL::get_count() const
	{
		return m_buffer_object->get_size_in_bytes() / VertexArraySizes::size_of(m_type);
	}

}