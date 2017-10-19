#pragma once

#include "IndexBuffer.h"
#include "BufferGL.h"
#include "VertexArraySizes.h"

#include <memory>

namespace argeo
{
	class IndexBufferGL : public IndexBuffer
	{
		friend class Device;

	protected:

		IndexBufferGL(
			const std::size_t& size_in_bytes,
			const BufferHint& hint);

		~IndexBufferGL();

	public:
		
		///
		/// @copydoc IndexBuffer::copy_from_system(const unsigned int*, const unsigned int&, const std::size_t&).
		///
		void copy_from_system(
			const unsigned int* buffer, 
			const unsigned int&  offset_in_bytes,
			const std::size_t& size_in_bytes);

		///
		/// @copydoc IndexBuffer::copy_from_system(const unsigned short*, const unsigned int&, const std::size_t&).
		///
		void copy_from_system(
			const unsigned short* buffer,
			const unsigned int& offset_in_bytes,
			const std::size_t& size_in_bytes);

		void copy_from_device(
			void* buffer, 
			const unsigned int&  offset_in_bytes,
			const std::size_t& size_in_bytes);

		void bind();

		static void unbind();

		std::size_t get_size_in_bytes() const;

		BufferHint get_hint() const;

		IndexDataType get_type() const;

		int get_count() const;

	private:

		// The buffer object used by this buffer.
		// This object is never exposed for extern access.
		std::unique_ptr<BufferGL> m_buffer_object;

		// The index data type, it can be unsigned short or unsigned int.
		IndexDataType m_type;
	};
}