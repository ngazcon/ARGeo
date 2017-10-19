#include "Mesh.h"

namespace argeo
{
	Mesh::Mesh(
		const bool& dispose_attributes,
		const bool& dispose_indices)
		: m_indices(nullptr)
		, m_primitive_type(PrimitiveType::Triangles)
		, m_front_face_winding_order(WindingOrder::CounterClockwise)
		, m_dispose_attributes(dispose_attributes)
		, m_dispose_indices(dispose_indices)
	{ }

	Mesh::~Mesh()
	{ 
		if (m_dispose_attributes)
		{
			for (auto attribute : m_attributes)
			{
				delete attribute.second;
			}
		}
		
		if (m_dispose_indices)
		{
			delete m_indices;
		}
	}

	IIndices* Mesh::get_indices()
	{
		return m_indices;
	}

	void Mesh::set_indices(IIndices* value)
	{
		m_indices = value;
	}

	PrimitiveType Mesh::get_primitive_type()
	{
		return m_primitive_type;
	}

	void Mesh::set_primitive_type(const PrimitiveType& value)
	{
		m_primitive_type = value;
	}

	WindingOrder Mesh::get_front_face_winding_order()
	{
		return m_front_face_winding_order;
	}

	void Mesh::set_front_face_winding_order(const WindingOrder& value)
	{
		m_front_face_winding_order = value;
	}

	VertexAttributeCollection Mesh::get_attributes()
	{
		return m_attributes;
	}

	bool Mesh::add_attribute(IVertexAttribute* attribute)
	{
		return m_attributes.emplace(attribute->get_name(), attribute).second;
	}

	bool Mesh::remove_attribute(IVertexAttribute* attribute)
	{
		return m_attributes.erase(attribute->get_name());
	}
}