#include "VertexBufferGL.h"
#include "VertexBufferAttributeCollectionGL.h"
#include "TypeConverterGL.h"
#include "Device.h"

#include <stdexcept>
#include <algorithm>

#include "GL.h"

namespace argeo
{
	VertexBufferAttributeCollectionGL::VertexBufferAttributeCollectionGL()
		: m_maximum_count(Device::get_maximum_number_of_vertex_attributes())
	{
		// Initialize the collection with nullptrs
		m_underlying_collection.reserve(m_maximum_count);

		for (int i = 0; i < m_maximum_count; ++i)
		{
			m_underlying_collection.push_back(
				std::unique_ptr<VertexBufferAttributeGL>(new VertexBufferAttributeGL()));
		}
	}

	VertexBufferAttributeCollectionGL::VertexBufferAttributeCollectionGL(VertexBufferAttributes attributes)
		: m_maximum_count(Device::get_maximum_number_of_vertex_attributes())
	{
		// Initialize the collection with nullptrs
		m_underlying_collection.reserve(m_maximum_count);

		for (int i = 0; i < m_maximum_count; ++i)
		{
			m_underlying_collection.push_back(
				std::unique_ptr<VertexBufferAttributeGL>(new VertexBufferAttributeGL()));
		}

		for (auto entry : attributes)
		{
			set_attribute(
				entry.second,
				entry.first);
		}
	}


	VertexBufferAttributeCollectionGL::~VertexBufferAttributeCollectionGL()
	{ }

	void VertexBufferAttributeCollectionGL::attach(const unsigned int& index)
	{
		glEnableVertexAttribArray(index);

		VertexBufferAttribute* attribute = 
			m_underlying_collection[index]->vertex_buffer_attribute.get();

		VertexBufferGL* buffer = static_cast<VertexBufferGL*>(attribute->get_vertex_buffer());

		// Bind the buffer object
		glBindBuffer(GL_ARRAY_BUFFER, buffer->get_handle());

		glVertexAttribPointer(
			index,
			attribute->get_number_of_components(),
			TypeConverterGL::ToEnum(attribute->get_component_type()),
			attribute->get_normalized(),
			attribute->get_stride_in_bytes(),
			reinterpret_cast<void*>(attribute->get_offset_in_bytes()));

	}

	void VertexBufferAttributeCollectionGL::detach(const unsigned int& index)
	{
		glDisableVertexAttribArray(index);
	}

	void VertexBufferAttributeCollectionGL::clean()
	{
		if (m_dirty)
		{
			int maximum_array_index = 0;

			for (int i = 0; i < m_maximum_count; i++)
			{
				VertexBufferAttributeGL* attribute = m_underlying_collection[i].get();

				if (attribute->dirty)
				{
					if (attribute->vertex_buffer_attribute.get() != nullptr)
					{
						attach(i);
					}
					else
					{
						detach(i);
					}

					attribute->dirty = false;
				}

				if (attribute->vertex_buffer_attribute.get() != nullptr)
				{
					maximum_array_index =
						std::max(attribute->vertex_buffer_attribute->get_number_of_vertices(), maximum_array_index);
				}
			}

			m_dirty = false;
			m_maximum_array_index = maximum_array_index;
		}
	}

	VertexBufferAttribute* VertexBufferAttributeCollectionGL::get_attribute(const unsigned int& index)
	{
		return m_underlying_collection[index]->vertex_buffer_attribute.get();
	}

	void VertexBufferAttributeCollectionGL::set_attribute(
		VertexBufferAttribute* value, 
		const unsigned int& index)
	{
		if (m_underlying_collection[index]->vertex_buffer_attribute.get() != value)
		{
			if (value != nullptr)
			{
				if (value->get_number_of_components() < 1 || value->get_number_of_components() > 4)
				{
					throw std::invalid_argument(
						"Number Of Components must be between one and four.");
				}

				if (value->get_normalized())
				{
					if ((value->get_component_type() != ComponentDataType::Byte) &&
						(value->get_component_type() != ComponentDataType::UnsignedByte) &&
						(value->get_component_type() != ComponentDataType::Short) &&
						(value->get_component_type() != ComponentDataType::UnsignedShort) &&
						(value->get_component_type() != ComponentDataType::Int) &&
						(value->get_component_type() != ComponentDataType::UnsignedInt))
					{
						throw std::invalid_argument(
							"When Normalized is true, ComponentDatatype must be Byte, UnsignedByte, Short, UnsignedShort, Int, or UnsignedInt.");
					}
				}
			}
		}

		if ((m_underlying_collection[index]->vertex_buffer_attribute.get() != nullptr) && (value == nullptr))
		{
			m_count++;
		}
		else if ((m_underlying_collection[index]->vertex_buffer_attribute.get() == nullptr) && (value != nullptr))
		{
			m_count++;
		}

		m_underlying_collection[index]->vertex_buffer_attribute.reset(value);
		m_underlying_collection[index]->dirty = true;
		m_dirty = true;
	}

	unsigned int VertexBufferAttributeCollectionGL::get_maximum_array_index() const
	{
		return m_maximum_array_index;
	}

	unsigned int VertexBufferAttributeCollectionGL::get_count() const
	{
		return m_count;
	}

	unsigned int VertexBufferAttributeCollectionGL::get_maximum_count() const
	{
		return m_maximum_count;
	}
}