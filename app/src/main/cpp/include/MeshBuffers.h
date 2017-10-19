#pragma once

#include "VertexBufferAttributeCollection.h"
#include "MeshVertexBufferAttributeCollection.h"
#include "IndexBuffer.h"

#include <memory>

namespace argeo
{
	class MeshBuffers
	{
	public:

		MeshBuffers(bool dispose_buffers = false);
		~MeshBuffers();

		// Get the buffer attribute collection of this mesh.
		VertexBufferAttributeCollection* get_attributes() const;

		// Get the index buffer of this mesh.
		IndexBuffer* get_index_buffer();

		// Set the index buffer of this mesh.
		void set_index_buffer(IndexBuffer* value);
		
	private:

		bool m_dispose_attributes;

		std::unique_ptr<MeshVertexBufferAttributeCollection> m_attributes;

		IndexBuffer* m_index_buffer;
	};
}
