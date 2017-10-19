#pragma once

#include "PixelBufferHint.h"
#include "BufferAccess.h"
#include "FrameBuffer.h"
#include "FrameBufferAttachment.h"
#include "ImageFormat.h"
#include "ImageDataType.h"

#include <vector>

namespace argeo
{
	class ReadPixelBuffer
	{
	public:
		// Copies the data given in buffer to device memory.
		// Uses a zero stride offset and the total size of this buffer.
		template<class T>
		void copy_from_system(const std::vector<T>& buffer);

		// Copies the data given in buffer to device memory.
		// Uses the given stride offset and the total size of this buffer.
		// The stride_in_bytes must be in bytes.
		template<class T>
		void copy_from_system(
			const std::vector<T>& buffer,
            const std::size_t& offset_in_bytes);

		// Copies the data given in buffer to device memory.
		// Uses the given stride offset and size.
		// The stride must be in bytes.
		// The size must be lesser than this buffer size.
		template<class T>
		void copy_from_system(
			const std::vector<T>& buffer,
            const std::size_t& offset_in_bytes,
			const std::size_t& size_in_bytes);

		virtual void copy_from_system(
			const void* buffer,
            const std::size_t& offset_in_bytes,
			const std::size_t& size_in_bytes) = 0;

		// Copies the data stored in this buffer to system memory.
		// Uses a zero stride offset and the total size of this buffer.
		template<class T>
		std::vector<T> copy_from_device();

		// Copies the data stored in this buffer to system memory.
		// Uses the given stride offset and the total size of this buffer.
		// The stride_in_bytes must be in bytes.
		template<class T>
		std::vector<T> copy_from_device(const std::size_t& offset_in_bytes);

		// Copies the data stored in this buffer to system memory.
		// Uses the given stride_in_bytes offset and size.
		// The stride must be in bytes.
		// The size must be lesser than this buffer size.
		template<class T>
		std::vector<T> copy_from_device(
			const std::size_t& offset_in_bytes,
			const std::size_t& size_in_bytes);

		virtual void copy_from_device(
			void* buffer,
			const std::size_t& offset_in_bytes,
			const std::size_t& size_in_bytes) = 0;

		template<class T>
		T* map_range(
			const std::size_t& offset_in_bytes,
			const std::size_t& size_in_bytes,
			const BufferAccess& access);

		virtual void* map_range(
			const std::size_t& offset_in_bytes,
			const std::size_t& size_in_bytes,
			const BufferAccess& access) = 0;

		virtual void unmap() = 0;

		virtual void bind() = 0;

		// The size of this buffer in bytes.
		virtual std::size_t get_size_in_bytes() const = 0;

		// A hint that is used by the driver to improve performance in some scenarios.
		virtual PixelBufferHint get_hint() const = 0;

		virtual void transfer(
			FrameBuffer* frame_buffer,
			const unsigned int& x,
			const unsigned int& y,
			const unsigned int& width,
			const unsigned int& height,
			const ImageFormat & format = ImageFormat::RedGreenBlueAlpha,
			const ImageDataType& data_type = ImageDataType::UnsignedByte,
			const FrameBufferAttachment& attachment = FrameBufferAttachment::ColorAttachment0) = 0;
	};

	template<class T>
	inline void ReadPixelBuffer::copy_from_system(const std::vector<T>& buffer)
	{
		copy_from_system<T>(buffer, 0, get_size_in_bytes());
	}

	template<class T>
	inline void ReadPixelBuffer::copy_from_system(
		const std::vector<T>& buffer,
		const std::size_t& offset_in_bytes)
	{
		copy_from_system<T>(buffer, offset_in_bytes, get_size_in_bytes());
	}

	template<class T>
	inline void ReadPixelBuffer::copy_from_system(
		const std::vector<T>& buffer,
		const std::size_t& offset_in_bytes,
		const std::size_t& size_in_bytes)
	{
		copy_from_system(buffer.data(), offset_in_bytes, size_in_bytes);
	}

	template<class T>
	inline std::vector<T> ReadPixelBuffer::copy_from_device()
	{
		return copy_from_device<T>(0, get_size_in_bytes());
	}

	template<class T>
	inline std::vector<T> ReadPixelBuffer::copy_from_device(const std::size_t& offset_in_bytes)
	{
		return copy_from_device<T>(offset_in_bytes, get_size_in_bytes());
	}

	template<class T>
	inline std::vector<T> ReadPixelBuffer::copy_from_device(
			const std::size_t& offset_in_bytes,
			const std::size_t& size_in_bytes)
	{
		std::vector<T> buffer;
		buffer.resize(size_in_bytes / sizeof(T));
		copy_from_device(buffer.data(), offset_in_bytes, size_in_bytes);
		return buffer;
	}

	template<class T>
	inline T* ReadPixelBuffer::map_range(
		const std::size_t& offset_in_bytes,
		const std::size_t& size_in_bytes,
		const BufferAccess& access)
	{
		return static_cast<T*>(map_range(
			offset_in_bytes,
			size_in_bytes,
			access));
	}

}
