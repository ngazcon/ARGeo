#pragma once

#include "ComponentDataType.h"
#include "VertexArraySizes.h"
#include "VertexBuffer.h"

namespace argeo
{
	class VertexBufferAttribute
	{
	public:

		VertexBufferAttribute(
			VertexBuffer* vertex_buffer,
			ComponentDataType component_type, 
			int number_of_components,
			bool normalized     = false,
			int offset_in_bytes = 0,
			int stride_in_bytes = 0);

		~VertexBufferAttribute();

		VertexBuffer* get_vertex_buffer() const;

		ComponentDataType get_component_type() const;

		int get_offset_in_bytes() const;
		int get_stride_in_bytes() const;
		int get_number_of_vertices() const;
		int get_number_of_components() const;

		bool get_normalized() const;


	private:

		VertexBuffer* m_vertex_buffer;

		ComponentDataType m_component_type;

		int m_number_of_components;

		int m_stride_in_bytes;

		int m_offset_in_bytes;

		bool m_normalized;
	};
}
