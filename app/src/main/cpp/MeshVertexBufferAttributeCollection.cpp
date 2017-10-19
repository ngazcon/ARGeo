#include "MeshVertexBufferAttributeCollection.h"
#include "Device.h"

namespace argeo
{
	MeshVertexBufferAttributeCollection::MeshVertexBufferAttributeCollection()
		: m_count(0)
		, m_maximum_count(Device::get_maximum_number_of_vertex_attributes())
	{
		m_attributes.resize(m_maximum_count);

		for (int i = 0; i < m_maximum_count; ++i)
		{
			m_attributes[i] = nullptr;
		}
	}


	MeshVertexBufferAttributeCollection::~MeshVertexBufferAttributeCollection()
	{ }

	VertexBufferAttribute* MeshVertexBufferAttributeCollection::get_attribute(const unsigned int& index)
	{
		return m_attributes[index];
	}

	void MeshVertexBufferAttributeCollection::set_attribute(
		VertexBufferAttribute* value, 
		const unsigned int& index)
	{
		if ((m_attributes[index] != nullptr) && (value == nullptr))
		{
			m_count--;
		}
		else if ((m_attributes[index] == nullptr) && (value != nullptr))
		{
			m_count++;
		}

		m_attributes[index] = value;
	}

	unsigned int MeshVertexBufferAttributeCollection::get_count() const
	{
		return m_count;
	}

	unsigned int MeshVertexBufferAttributeCollection::get_maximum_count() const
	{
		return m_maximum_count;
	}
}