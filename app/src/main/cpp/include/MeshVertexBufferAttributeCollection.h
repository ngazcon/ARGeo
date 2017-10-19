#pragma once

#include "VertexBufferAttributeCollection.h"

#include <vector>
#include <memory>

namespace argeo
{
	class MeshVertexBufferAttributeCollection : public VertexBufferAttributeCollection
	{
	public:

		MeshVertexBufferAttributeCollection();
		~MeshVertexBufferAttributeCollection();

		// Get the attribute on the given index.
		VertexBufferAttribute* get_attribute(
			const unsigned int& index);

		// Set the attribute on the given index.
		void set_attribute(
			VertexBufferAttribute* attribute,
			const unsigned int& index);

		// Get the actual count of attributes on this buffer.
		unsigned int get_count() const;

		// Get the max count of attributes on this buffer.
		unsigned int get_maximum_count() const;

	private:

		std::vector<VertexBufferAttribute*> m_attributes;
		unsigned int m_count;
		unsigned int m_maximum_count;
	};
}