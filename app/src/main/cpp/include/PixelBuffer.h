#pragma once

#include "Buffer.h"
#include "BufferHint.h"
#include "Bitmap.h"
#include "BufferAccess.h"

#include <vector>
#include <memory>

namespace argeo
{
	class PixelBuffer : public Buffer
	{
		///
		/// A buffer containing pixel data.
		///

	public:

		///
		/// Copies the data given in buffer to device memory.
		/// Uses a zero stride offset and the total size of this buffer.
		/// @param {std::vector<T>} buffer The system's buffer to fill with the data.
		///
		template<class T>
		void copy_from_system(const std::vector<T>& buffer);

		///
		/// Copies the data given in buffer to device memory.
		/// Uses the given stride offset and the total size of this buffer.
		/// @param {std::vector<T>} buffer The system buffer with the data.
		/// @param {unsigned int} stride_in_bytes The stride in bytes of the device's buffer.
		/// @exception {std::invalid_argument} The stride must be greater or equal to zero.
		///
		template<class T>
		void copy_from_system(
			const std::vector<T>& buffer, 
			const unsigned int& stride_in_bytes);

		///
		/// Copies the data given in buffer to device memory.
		/// Uses the given stride offset and size.
		/// The size must be lesser than this buffer size.
		/// @param {std::vector<T>} buffer The system buffer with the data.
		/// @param {unsigned int} stride_in_bytes The stride in bytes of the device's buffer.
		/// @param {std::size_t} size_in_bytes The size in bytes of the device's buffer.
		/// @exception {std::invalid_argument} The stride must be greater or equal to zero.
		/// @exception {std::invalid_argument} The size must be greater than zero.
		///
		template<class T>
		void copy_from_system(
			const std::vector<T>& buffer,
			const unsigned int& stride_in_bytes,
			const std::size_t& size_in_bytes);


		///
		/// Copies the data given in buffer to device memory.
		/// Uses the given stride offset and size.
		/// The size must be lesser than this buffer size.
		/// @param {void*} buffer The system buffer with the data.
		/// @param {unsigned int} stride_in_bytes The stride in bytes of the device's buffer.
		/// @param {std::size_t} size_in_bytes The size in bytes of the device's buffer.
		/// @exception {std::invalid_argument} The stride must be greater or equal to zero.
		/// @exception {std::invalid_argument} The size must be greater than zero.
		///
		virtual void copy_from_system(
			const void* buffer, 
			const unsigned int& stride_in_bytes,
			const std::size_t& size_in_bytes) = 0;

		///
		/// Copies the data stored in this buffer to system memory.
		/// Uses a zero stride offset and the total size of this buffer.
		/// @return {std::vector<T>} The buffer with device's buffer data.
		///
		template<class T>
		std::vector<T> copy_from_device();

		///
		/// Copies the data stored in this buffer to system memory.
		/// Uses the given stride offset and the total size of this buffer.
		/// The stride_in_bytes must be in bytes.
		/// @param {unsigned int} stride_in_bytes The stride in bytes of the device's buffer.
		/// @return {std::vector<T>} The buffer with device's buffer data.
		/// @exception {std::invalid_argument} The stride must be greater or equal to zero.
		///
		template<class T>
		std::vector<T> copy_from_device(const unsigned int& stride_in_bytes);

		///
		/// Copies the data stored in this buffer to system memory.
		/// Uses the given stride_in_bytes offset and size.
		/// @param {unsigned int} stride_in_bytes The stride in bytes of the device's buffer.
		/// @param {std::size_t} size_in_bytes The size in bytes of the device's buffer.
		/// @return {std::vector<T>} The buffer with device's buffer data.
		/// @exception {std::invalid_argument} The stride must be greater or equal to zero.
		/// @exception {std::invalid_argument} The size must be greater than zero.
		///
		template<class T>
		std::vector<T> copy_from_device(
			const unsigned int& stride_in_bytes,
			const std::size_t& size_in_bytes);
		
		///
		/// Copies the data stored in this buffer to system memory.
		/// Uses the given stride_in_bytes offset and size.
		/// @param {void*} buffer The system buffer with the device's buffer data.
		/// @param {unsigned int} stride_in_bytes The stride in bytes of the device's buffer.
		/// @param {std::size_t} size_in_bytes The size in bytes of the device's buffer.
		/// @exception {std::invalid_argument} The stride must be greater or equal to zero.
		/// @exception {std::invalid_argument} The size must be greater than zero.
		///
		virtual void copy_from_device(
			void* buffer, 
			const unsigned int& stride_in_bytes,
			const std::size_t& size_in_bytes) = 0;

		///
		/// Copies the pixels stored in the given bitmap to device's buffer.
		/// @param {Bitmap*} bitmap The bitmap to copy.
		///
		virtual void copy_from_bitmap(
				Bitmap* bitmap,
				const bool& flip = false) = 0;


		///
		/// Copies the data stored in this device's buffer to a bitmap.
		/// @param {unsigned int} width The width of the resulting bitmap.
		/// @param {unsigned int} height The height of the resulting bitmap.
		/// @param {PixelFormat} pixel_format The pixel format of the resulting bitmap.
		/// @return {std::unique_ptr<Bitmap>} The bitmap with the device's buffer data.
		/// @exception {std::invalid_argument} The width must be greater than zero.
		/// @exception {std::invalid_argument} The height must be greater than zero.
		///
		virtual std::unique_ptr<Bitmap> copy_to_bitmap(
			const unsigned int& width,
			const unsigned int& height,
			const PixelFormat& pixel_format) = 0;


		///
		/// Get a pointer to system's memory containing the data stored in this device's buffer.
		/// This operation establish a mapping within the device's and system's data.
		/// @param {unsigned int} offset_in_bytes The offset to device's buffer data.
		/// @param {std::size_t} size_in_bytes The size of the buffer to map.
		/// @param {BufferAccess} access The buffer access mode.
		/// @return {void*} The system's memory with device's buffer data.
		/// @exception {std::invalid_argument} The offset_in_bytes must be greater or equals to zero.
		/// @exception {std::invalid_argument} The size_in_bytes must be greater than zero.
		///
		virtual void* map_range(
			const unsigned int& offset_in_bytes,
			const std::size_t& size_in_bytes,
			const BufferAccess& access) = 0;


		///
		/// Unmap this buffer.
		///
		virtual void unmap() = 0;

		///
		/// The size of this buffer in bytes.
		/// @return {std::size_t} The size of this buffer in bytes.
		///
		virtual std::size_t get_size_in_bytes() const = 0;

		///
		/// A hint that is used by the driver to improve performance in some scenarios.
		/// @return {BufferHint} The hint of this buffer.
		///
		virtual BufferHint get_hint() const = 0;
	};

	template<class T>
	inline void PixelBuffer::copy_from_system(const std::vector<T>& buffer)
	{
		copy_from_system<T>(buffer, 0, get_size_in_bytes());
	}

	template<class T>
	inline void PixelBuffer::copy_from_system(
		const std::vector<T>& buffer, 
		const unsigned int& stride_in_bytes)
	{
		copy_from_system<T>(buffer, stride_in_bytes, get_size_in_bytes());
	}

	template<class T>
	inline void PixelBuffer::copy_from_system(
		const std::vector<T>& buffer, 
		const unsigned int& stride_in_bytes, 
		const std::size_t& size_in_bytes)
	{
		copy_from_system(buffer.data(), stride_in_bytes, size_in_bytes);
	}

	template<class T>
	inline std::vector<T> PixelBuffer::copy_from_device()
	{
		return copy_from_device<T>(0, get_size_in_bytes());
	}

	template<class T>
	inline std::vector<T> PixelBuffer::copy_from_device(const unsigned int& stride_in_bytes)
	{
		return copy_from_device<T>(stride_in_bytes, get_size_in_bytes());
	}

	template<class T>
	inline std::vector<T> PixelBuffer::copy_from_device(
		const unsigned int& stride_in_bytes, 
		const std::size_t& size_in_bytes)
	{
		std::vector<T> buffer;
		buffer.resize(size_in_bytes / sizeof(T));
		copy_from_device(buffer.data(), stride_in_bytes, size_in_bytes);
		return buffer;
	}
}
