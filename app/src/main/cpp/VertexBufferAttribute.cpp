#include "VertexBufferAttribute.h"

namespace argeo
{
	VertexBufferAttribute::VertexBufferAttribute(
		VertexBuffer* vertex_buffer,
		ComponentDataType component_type,
		int number_of_components,
		bool normalized,
		int offset_in_bytes,
		int stride_in_bytes)
		: m_vertex_buffer(vertex_buffer)
		, m_component_type(component_type)
		, m_number_of_components(number_of_components)
		, m_normalized(normalized)
		, m_offset_in_bytes(offset_in_bytes)
		, m_stride_in_bytes(stride_in_bytes)
	{

		if (m_stride_in_bytes == 0)
		{
			// Tightly packed
			m_stride_in_bytes = m_number_of_components * VertexArraySizes::size_of(m_component_type);
		}
	}

	VertexBufferAttribute::~VertexBufferAttribute()
	{ }

	VertexBuffer* VertexBufferAttribute::get_vertex_buffer() const
	{
		return m_vertex_buffer;
	}

	int VertexBufferAttribute::get_number_of_components() const
	{
		return m_number_of_components;
	}

	ComponentDataType VertexBufferAttribute::get_component_type() const
	{
		return m_component_type;
	}

	int VertexBufferAttribute::get_offset_in_bytes() const
	{
		return m_offset_in_bytes;
	}

	int VertexBufferAttribute::get_stride_in_bytes() const
	{
		return m_stride_in_bytes;
	}

	int VertexBufferAttribute::get_number_of_vertices() const
	{
		return m_vertex_buffer->get_size_in_bytes() / m_stride_in_bytes;
	}

	bool VertexBufferAttribute::get_normalized() const
	{
		return m_normalized;
	}
}
