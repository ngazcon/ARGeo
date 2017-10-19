#include "MeshBuffers.h"

namespace argeo
{
	MeshBuffers::MeshBuffers(bool dispose_buffers)
		: m_attributes(new MeshVertexBufferAttributeCollection())
		, m_dispose_attributes(dispose_buffers)
	{ }


	MeshBuffers::~MeshBuffers()
	{ 
		if (m_dispose_attributes)
		{
			for (int i = 0; i < m_attributes->get_count(); ++i)
			{
				delete m_attributes->get_attribute(i);
			}
		}
	}

	VertexBufferAttributeCollection* MeshBuffers::get_attributes() const
	{
		return m_attributes.get();
	}

	IndexBuffer* MeshBuffers::get_index_buffer()
	{
		return m_index_buffer;
	}

	void MeshBuffers::set_index_buffer(IndexBuffer* value)
	{
		m_index_buffer = value;
	}
}