#pragma once

#include "BufferHint.h"
#include "BufferAccess.h"

#include <vector>

namespace argeo
{
	/*
	 * A Buffer Object that is used to store uniform data for a shader program is called a Uniform Buffer Object. They can be used to share uniforms between different programs, as well as quickly change between sets of uniforms for the same program object.
	 * The term "Uniform Buffer Object" refers to the OpenGL buffer object that is used to provide storage for uniforms. The term "uniform blocks" refer to the GLSL language grouping of uniforms whose storage come from buffer objects.
	 */
	class UniformBuffer
	{
	public:

		// Virtual destructor
		virtual ~UniformBuffer() { };

		// Copies the data given in buffer to device memory.
		// Uses a zero stride offset and the total size of this buffer.
		template<class T>
		void copy_from_system(std::vector<T>& buffer);

		// Copies the data given in buffer to device memory.
		// Uses the given stride offset and the total size of this buffer.
		// The stride_in_bytes must be in bytes.
		template<class T>
		void copy_from_system(std::vector<T>& buffer, int stride_in_bytes);

		// Copies the data given in buffer to device memory.
		// Uses the given stride offset and size.
		// The stride must be in bytes.
		// The size must be lesser than this buffer size.
		template<class T>
		void copy_from_system(std::vector<T>& buffer, int stride_in_bytes, int size_in_bytes);

		virtual void copy_from_system(void* buffer, int stride_in_bytes, int size_in_bytes) = 0;

		// Copies the data stored in this buffer to system memory.
		// Uses a zero stride offset and the total size of this buffer.
		template<class T>
		std::vector<T> copy_from_device();

		// Copies the data stored in this buffer to system memory.
		// Uses the given stride offset and the total size of this buffer.
		// The stride_in_bytes must be in bytes.
		template<class T>
		std::vector<T> copy_from_device(int stride_in_bytes);

		// Copies the data stored in this buffer to system memory.
		// Uses the given stride_in_bytes offset and size.
		// The stride must be in bytes.
		// The size must be lesser than this buffer size.
		template<class T>
		std::vector<T> copy_from_device(int stride_in_bytes, int size_in_bytes);

		virtual void copy_from_device(void* buffer, int stride_in_bytes, int size_in_bytes) = 0;

		template<class T>
		T* map_range(
			int offset_in_bytes,
			int size_in_bytes,
			BufferAccess access);

		virtual void* map_range(
			int offset_in_bytes,
			int size_in_bytes,
			BufferAccess access) = 0;

		virtual void unmap() = 0;

		// The size of this buffer in bytes.
		virtual int get_size_in_bytes() const = 0;

		// A hint that is used by the driver to improve performance in some scenarios.
		virtual BufferHint get_hint() const = 0;
	};

	template<class T>
	inline void UniformBuffer::copy_from_system(std::vector<T>& buffer)
	{
		copy_from_system<T>(buffer, 0, get_size_in_bytes());
	}

	template<class T>
	inline void UniformBuffer::copy_from_system(std::vector<T>& buffer, int stride_in_bytes)
	{
		copy_from_system<T>(buffer, stride_in_bytes, get_size_in_bytes());
	}

	template<class T>
	inline void UniformBuffer::copy_from_system(std::vector<T>& buffer, int stride_in_bytes, int size_in_bytes)
	{
		copy_from_system(&buffer[0], stride_in_bytes, size_in_bytes);
	}

	template<class T>
	inline std::vector<T> UniformBuffer::copy_from_device()
	{
		return copy_from_device<T>(0, get_size_in_bytes());
	}

	template<class T>
	inline std::vector<T> UniformBuffer::copy_from_device(int stride_in_bytes)
	{
		return copy_from_device<T>(stride_in_bytes, get_size_in_bytes());
	}

	template<class T>
	inline std::vector<T> UniformBuffer::copy_from_device(int stride_in_bytes, int size_in_bytes)
	{
		std::vector<T> buffer;
		buffer.resize(size_in_bytes / sizeof(T));
		copy_from_device(&buffer[0], stride_in_bytes, size_in_bytes);
		return buffer;
	}

	template<class T>
	inline T* UniformBuffer::map_range(
		int offset_in_bytes,
		int size_in_bytes,
		BufferAccess access)
	{
		return static_cast<T*>(map_range(
			offset_in_bytes,
			size_in_bytes,
			access));
	}
}
