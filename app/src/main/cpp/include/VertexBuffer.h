#pragma once

#include "BufferHint.h"
#include "Buffer.h"

#include <vector>

namespace argeo
{
	class VertexBuffer : public Buffer
	{
		/// 
		/// A vertex buffer is an untyped buﬀer, containing one or more attributes.
		/// It is interpreted once assigned to a vertex array.
		/// 

	public:

		///
		/// Copies the data given in buffer to device memory.
		/// Uses a zero stride offset and the total size of this buffer.
		///
		template<class T>
		void copy_from_system(const std::vector<T>& buffer);

		///
		/// Copies the data given in buffer to device memory.
		/// Uses the given offset and the total size of this buffer.
		/// The offset_in_bytes must be in bytes.
		///
		template<class T>
		void copy_from_system(
			const std::vector<T>& buffer,
			const unsigned int& offset_in_bytes);

		///
		/// Copies the data given in buffer to device memory.
		/// Uses the given offset and size.
		/// The stride must be in bytes.
		/// The size must be lesser than this buffer size.
		///
		template<class T>
		void copy_from_system(
			const std::vector<T>& buffer, 
			const unsigned int& offset_in_bytes,
			const std::size_t& size_in_bytes);

		virtual void copy_from_system(
			const void* buffer, 
			const unsigned int& offset_in_bytes,
			const std::size_t& size_in_bytes) = 0;

		// Copies the data stored in this buffer to system memory.
		// Uses a zero offset and the total size of this buffer.
		template<class T>
		std::vector<T> copy_from_device();

		// Copies the data stored in this buffer to system memory.
		// Uses the given offset and the total size of this buffer.
		// The offset_in_bytes must be in bytes.
		template<class T>
		std::vector<T> copy_from_device(const unsigned int& offset_in_bytes);

		// Copies the data stored in this buffer to system memory.
		// Uses the given offset_in_bytes offset and size.
		// The stride must be in bytes.
		// The size must be lesser than this buffer size.
		template<class T>
		std::vector<T> copy_from_device(
			const unsigned int& offset_in_bytes,
			const std::size_t& size_in_bytes);

		virtual void copy_from_device(
			void* buffer, 
			const unsigned int& offset_in_bytes,
			const std::size_t& size_in_bytes) = 0;

		// The size of this buffer in bytes.
		virtual std::size_t get_size_in_bytes() const = 0;

		// A hint that is used by the driver to improve performance in some scenarios.
		virtual BufferHint get_hint() const = 0;
		
	};

	template<class T>
	inline void VertexBuffer::copy_from_system(const std::vector<T>& buffer)
	{
		copy_from_system<T>(buffer, 0, get_size_in_bytes());
	}

	template<class T>
	inline void VertexBuffer::copy_from_system(
		const std::vector<T>& buffer, 
		const unsigned int& offset_in_bytes)
	{
		copy_from_system<T>(buffer, offset_in_bytes, get_size_in_bytes());
	}

	template<class T>
	inline void VertexBuffer::copy_from_system(
		const std::vector<T>& buffer,
		const unsigned int& offset_in_bytes,
		const std::size_t& size_in_bytes)
	{
		copy_from_system(buffer.data(), offset_in_bytes, size_in_bytes);
	}

	template<class T>
	inline std::vector<T> VertexBuffer::copy_from_device()
	{
		return copy_from_device<T>(0, get_size_in_bytes());
	}

	template<class T>
	inline std::vector<T> VertexBuffer::copy_from_device(const unsigned int& offset_in_bytes)
	{
		return copy_from_device<T>(offset_in_bytes, get_size_in_bytes());
	}

	template<class T>
	inline std::vector<T> VertexBuffer::copy_from_device(
		const unsigned int& offset_in_bytes,
		const std::size_t& size_in_bytes)
	{
		std::vector<T> buffer;
		buffer.resize(size_in_bytes / sizeof(T));
		copy_from_device(buffer.data(), offset_in_bytes, size_in_bytes);
		return buffer;
	}
}
