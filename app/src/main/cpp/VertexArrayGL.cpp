#include "VertexArrayGL.h"

#include "IndexBufferGL.h"

#include <unordered_set>

#include "GL.h"

namespace argeo
{
	VertexArrayGL::VertexArrayGL()
		  : m_index_buffer(nullptr)
		  , m_attributes(new VertexBufferAttributeCollectionGL())
	{
		glGenVertexArrays(1, &m_vao);
	}


	VertexArrayGL::~VertexArrayGL()
	{
		if (m_dispose_buffers)
		{
			// Multiple components may share the same vertex buffer, so
			// find the unique set of vertex buffers used by this vertex array.
			std::unordered_set<VertexBuffer*> vertex_buffers;
			for (int i = 0; i < m_attributes->get_maximum_array_index(); i++)
			{
				VertexBufferAttribute* vertex_buffer_attribute = m_attributes->get_attribute(i);
				if (vertex_buffer_attribute != nullptr)
				{
					VertexBuffer* vertex_buffer = vertex_buffer_attribute->get_vertex_buffer();
					if (vertex_buffer != nullptr)
					{
						vertex_buffers.insert(vertex_buffer);
					}
				}
			}

			for (int i = 0; i < vertex_buffers.size(); i++)
			{
				VertexBuffer* vertex_buffer = m_attributes->get_attribute(i)->get_vertex_buffer();
				delete vertex_buffer;
			}

			if (m_index_buffer != nullptr)
			{
				delete m_index_buffer;
			}
		}

		glDeleteVertexArrays(1, &m_vao);
	}

	void VertexArrayGL::bind()
	{
		glBindVertexArray(m_vao);
	}

	void VertexArrayGL::unbind()
	{
		glBindVertexArray(0);
	}

	int VertexArrayGL::get_maximum_array_index() const
	{
		return m_attributes->get_maximum_array_index();
	}

	VertexBufferAttributeCollection* VertexArrayGL::get_attributes()
	{
		return m_attributes.get();
	}

	IndexBuffer* VertexArrayGL::get_index_buffer() const
	{
		return m_index_buffer;
	};

	void VertexArrayGL::set_index_buffer(IndexBuffer* buffer)
	{
		m_index_buffer = buffer;
		m_dirty_index_buffer = true;
	};

	bool VertexArrayGL::get_dispose_buffers() const
	{
		return m_dispose_buffers;
	}

	void VertexArrayGL::set_dispose_buffers(bool dispose)
	{
		m_dispose_buffers = dispose;
	}

	void VertexArrayGL::clean()
	{
		m_attributes->clean();

		if (m_dirty_index_buffer)
		{
			if (m_index_buffer != nullptr)
			{
				IndexBufferGL* bufferObjectGL = static_cast<IndexBufferGL*>(m_index_buffer);
				bufferObjectGL->bind();
			}
			else
			{
				IndexBufferGL::unbind();
			}

			m_dirty_index_buffer = false;
		}
	}
}