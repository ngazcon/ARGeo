#pragma once

#include "VertexBuffer.h"
#include "BufferGL.h"

#include <memory>

namespace argeo
{
	class VertexBufferGL : public VertexBuffer
	{
	public:

		VertexBufferGL(
			const std::size_t& size_in_bytes,
			const BufferHint& hint);

		~VertexBufferGL();

		void copy_from_system(
			const void* buffer,
			const unsigned int& stride_in_bytes,
			const std::size_t& size_in_bytes);

		void copy_from_device(
			void* buffer, 
			const unsigned int& stride_in_bytes,
			const std::size_t& size_in_bytes);

		std::size_t get_size_in_bytes() const;

		BufferHint get_hint() const;

		unsigned int get_handle() const;

	private:

		// The buffer object used by this buffer.
		// This object is never exposed for extern access.
		std::unique_ptr<BufferGL> m_buffer_object;
	};
}
