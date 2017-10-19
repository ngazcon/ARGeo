#pragma once

#include "BufferHint.h"
#include "IndexDataType.h"
#include "Buffer.h"

#include <vector>

namespace argeo
{
	
	class IndexBuffer : public Buffer
	{
		///
		/// An index buffer is fully typed, having an explicit Datatype property.
		/// It can be unsigned int or unsigned short.
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
		/// Uses the given stride offset and the total size of this buffer.
		/// The stride_in_bytes must be in bytes.
		///
		template<class T>
		void copy_from_system(
			const std::vector<T>& buffer,
			const unsigned int& stride_in_bytes);

		///
		/// Copies the data given in buffer to device memory.
		/// Uses the given stride offset and size.
		/// @param {unsigned int} stride_in_bytes The stride in bytes.
		/// @param The size must be lesser than this buffer size.
		///
		template<class T>
		void copy_from_system(
			const std::vector<T>& buffer,
			const unsigned int& stride_in_bytes,
			const std::size_t& size_in_bytes);


		///
		/// Copies the data given in buffer to device memory.
		/// Uses the given stride offset and size.
		/// The stride must be in bytes.
		/// The size must be lesser than this buffer size.
		///
		virtual void copy_from_system(
			const unsigned int* buffer,
			const unsigned int& stride_in_bytes,
			const std::size_t& size_in_bytes) = 0;


		///
		/// Copies the data given in buffer to device memory.
		/// Uses the given stride offset and size.
		/// The stride must be in bytes.
		/// The size must be lesser than this buffer size.
		///
		virtual void copy_from_system(
			const unsigned short* buffer,
			const unsigned int& stride_in_bytes,
			const std::size_t& size_in_bytes) = 0;

		///
		/// Copies the data stored in this buffer to system memory.
		/// Uses a zero stride offset and the total size of this buffer.
		///
		template<class T>
		std::vector<T> copy_from_device();

		///
		/// Copies the data stored in this buffer to system memory.
		/// Uses the given stride offset and the total size of this buffer.
		/// The stride_in_bytes must be in bytes.
		///
		template<class T>
		std::vector<T> copy_from_device(const unsigned int& stride_in_bytes);

		///
		/// Copies the data stored in this buffer to system memory.
		/// Uses the given stride_in_bytes offset and size.
		/// The stride must be in bytes.
		/// The size must be lesser than this buffer size.
		///
		template<class T>
		std::vector<T> copy_from_device(
			const unsigned int& stride_in_bytes, 
			const std::size_t& size_in_bytes);

		///
		/// Copies the data stored in this buffer to system memory.
		/// Uses the given stride_in_bytes offset and size.
		/// The stride must be in bytes.
		/// The size must be lesser than this buffer size.
		///
		virtual void copy_from_device(
			void* buffer, 
			const unsigned int& stride_in_bytes,
			const std::size_t& size_in_bytes) = 0;

		///
		/// Gets the size in bytes of this buffer.
		///
		virtual std::size_t get_size_in_bytes() const = 0;

		///
		/// Gets the usage hint for this buffer.
		/// It's a hint for the driver for improve performance.
		///
		virtual BufferHint get_hint() const = 0;

		///
		/// Gets the the type of the elements in this buffer. (e.g. Unsigned Integer, Unsigned Short)
		///
		virtual IndexDataType get_type() const = 0;
	};

	template<class T>
	inline void IndexBuffer::copy_from_system(const std::vector<T>& buffer)
	{
		copy_from_system<T>(buffer, 0, get_size_in_bytes());
	}

	template<class T>
	inline void IndexBuffer::copy_from_system(
		const std::vector<T>& buffer, 
		const unsigned int& stride_in_bytes)
	{
		copy_from_system<T>(buffer, stride_in_bytes, get_size_in_bytes());
	}

	template<>
	inline void IndexBuffer::copy_from_system(
		const std::vector<unsigned int>& buffer, 
		const unsigned int& stride_in_bytes, 
		const std::size_t& size_in_bytes)
	{
		copy_from_system(reinterpret_cast<const unsigned int*>(buffer.data()), stride_in_bytes, size_in_bytes);
	}

	template<>
	inline void IndexBuffer::copy_from_system(
		const std::vector<unsigned short>& buffer,
		const unsigned int& stride_in_bytes,
		const std::size_t& size_in_bytes)
	{
		copy_from_system(reinterpret_cast<const unsigned short*>(buffer.data()), stride_in_bytes, size_in_bytes);
	}


	template<class T>
	inline std::vector<T> IndexBuffer::copy_from_device()
	{
		return copy_from_device<T>(0, get_size_in_bytes());
	}

	template<class T>
	inline std::vector<T> IndexBuffer::copy_from_device(const unsigned int& stride_in_bytes)
	{
		return copy_from_device<T>(stride_in_bytes, get_size_in_bytes());
	}

	template<class T>
	inline std::vector<T> IndexBuffer::copy_from_device(
		const unsigned int& stride_in_bytes,
		const std::size_t& size_in_bytes)
	{
		std::vector<T> buffer;
		buffer.resize(size_in_bytes / sizeof(T));
		copy_from_device(buffer.data(), stride_in_bytes, size_in_bytes);
		return buffer;
	}
}
