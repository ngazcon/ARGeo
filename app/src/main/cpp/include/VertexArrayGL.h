#pragma once

#include "VertexArray.h"
#include "VertexBufferAttributeCollectionGL.h"

#include <memory>

namespace argeo
{
	class VertexArrayGL : public VertexArray
	{
	public:

		VertexArrayGL();

		~VertexArrayGL();

		int get_maximum_array_index() const;

		VertexBufferAttributeCollection* get_attributes();

		IndexBuffer* get_index_buffer() const;
		void set_index_buffer(IndexBuffer* buffer);

		bool get_dispose_buffers() const;
		void set_dispose_buffers(bool dispose);

		void bind();
		static void unbind();
		void clean();

	private:

		unsigned int m_vao;

		bool m_dispose_buffers;

		bool m_dirty_index_buffer;

		IndexBuffer* m_index_buffer;

		std::unique_ptr<VertexBufferAttributeCollectionGL> m_attributes;
	};
}
