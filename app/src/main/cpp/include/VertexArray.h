#pragma once

#include "VertexBufferAttributeCollection.h"
#include "IndexBuffer.h"


namespace argeo
{
	class VertexArray
	{
		///
		/// A vertex array defines the actual components making up a vertex, which are pulled from one or more vertex buffers.
		/// A vertex array also references an optional index buffer that indexes into these vertex buffers.
		///

	public:

		///
		/// The attributes within the vertex array. 
		/// These are accessed using zero based index and have it contains up to the maximum number of attributes supported by the driver.
		///
		virtual VertexBufferAttributeCollection* get_attributes() = 0;

		///
		/// Get the indices for the vertex buffers.
		/// It's opcional so it can be NULL.
		///
		virtual IndexBuffer* get_index_buffer() const = 0;

		///
		/// Set the indices for the vertex buffers.
		/// It's opcional so it can be NULL.
		///
		virtual void set_index_buffer(IndexBuffer* buffer) = 0;

		///
		///
		///
		virtual void set_dispose_buffers(bool dispose) = 0;

		///
		///
		///
		virtual bool get_dispose_buffers() const = 0;
	};
}
