#pragma once

#include "BufferHint.h"
#include "BufferTarget.h"
#include "BufferAccess.h"

#include <vector>

namespace argeo
{
	class BufferGL
	{
		friend class IndexBufferGL;
		friend class VertexBufferGL;
		friend class UniformBufferGL;

		///
		/// A buffer containing raw data.
		///

	protected:

		///
		/// Constructs a OpenGL buffer.
		/// @constructor
		/// @param {BufferTarget} target The buffer target.
		/// @param {BufferHint} hin The use hint of the buffer.
		/// @param {std::size_t} size_in_bytes The size of the buffer in bytes.
		///
		BufferGL(
			const BufferTarget& target, 
			const BufferHint& hint, 
			const std::size_t& size_in_bytes);


	public:

		///
		/// @destructor
		///
		~BufferGL();

		
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
		/// @param {unsigned int} offset_in_bytes The offset in bytes of the device's buffer.
		/// @exception {std::invalid_argument} The stride must be greater or equal to zero.
		///
		template<class T>
		void copy_from_system(
			const std::vector<T>& buffer,
			const unsigned int& offset_in_bytes);

		///
		/// Copies the data given in buffer to device memory.
		/// Uses the given stride offset and size.
		/// The size must be lesser than this buffer size.
		/// @param {std::vector<T>} buffer The system buffer with the data.
		/// @param {unsigned int} offset_in_bytes The offset in bytes of the device's buffer.
		/// @param {std::size_t} size_in_bytes The size in bytes of the device's buffer.
		/// @exception {std::invalid_argument} The stride must be greater or equal to zero.
		/// @exception {std::invalid_argument} The size must be greater than zero.
		///
		template<class T>
		void copy_from_system(
			const std::vector<T>& buffer,
			const unsigned int& offset_in_bytes,
			const std::size_t& size_in_bytes);


		///
		/// Copies the data given in buffer to device memory.
		/// Uses the given stride offset and size.
		/// The size must be lesser than this buffer size.
		/// @param {void*} buffer The system buffer with the data.
		/// @param {unsigned int} offset_in_bytes The offset in bytes of the device's buffer.
		/// @param {std::size_t} size_in_bytes The size in bytes of the device's buffer.
		/// @exception {std::invalid_argument} The stride must be greater or equal to zero.
		/// @exception {std::invalid_argument} The size must be greater than zero.
		///
		void copy_from_system(
			const void* buffer,
			const unsigned int& offset_in_bytes,
			const std::size_t& size_in_bytes);

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
		/// Uses the given offset and size.
		/// @param {unsigned int} offset_in_bytes The offset in bytes of the device's buffer.
		/// @param {std::size_t} size_in_bytes The size in bytes of the device's buffer.
		/// @return {std::vector<T>} The buffer with device's buffer data.
		/// @exception {std::invalid_argument} The offset must be greater or equal to zero.
		/// @exception {std::invalid_argument} The size must be greater than zero.
		///
		template<class T>
		std::vector<T> copy_from_device(
			const unsigned int& offset_in_bytes,
			const std::size_t& size_in_bytes);

		///
		/// Copies the data stored in this buffer to system memory.
		/// Uses the given offset and size.
		/// @param {void*} buffer The system buffer with the device's buffer data.
		/// @param {unsigned int} offset_in_bytes The offset in bytes of the device's buffer.
		/// @param {std::size_t} size_in_bytes The size in bytes of the device's buffer.
		/// @exception {std::invalid_argument} The offset must be greater or equal to zero.
		/// @exception {std::invalid_argument} The size must be greater than zero.
		///
		void copy_from_device(
			void* buffer,
			const unsigned int& offset_in_bytes,
			const std::size_t& size_in_bytes);


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
		void* map_range(
			const unsigned int& offset_in_bytes,
			const std::size_t& size_in_bytes,
			const BufferAccess& access);


		///
		/// Unmap this buffer.
		///
		void unmap();

		///
		/// The size of this buffer in bytes.
		/// @return {std::size_t} The size of this buffer in bytes.
		///
		std::size_t get_size_in_bytes() const;

		///
		/// A hint that is used by the driver to improve performance in some scenarios.
		/// @return {BufferHint} The hint of this buffer.
		///
		BufferHint get_hint() const;

		///
		/// Gets the handle to the openGL object.
		/// @return {unsigned int} The handle to this buffer object.
		///
		unsigned int get_handle() const;

		///
		/// Gets the target buffer.
		/// @return {BufferTarget} The buffer target.
		///
		BufferTarget get_target() const;

		///
		/// Bind this Buffer to the current context.
		///
		void bind();

		///
		/// Unbind this Buffer from the current context.
		///
		void unbind();

		///
		/// Unbind the buffer target from the current context.
		///
		static void unbind(BufferTarget target);

	private:
		unsigned int m_handle;
		std::size_t m_size_in_bytes;
		BufferTarget m_target;
		BufferHint m_hint;
	};

	template<class T>
	inline void BufferGL::copy_from_system(const std::vector<T>& buffer)
	{
		copy_from_device<T>(buffer, 0, get_size_in_bytes());
	}

	template<class T>
	inline void BufferGL::copy_from_system(
		const std::vector<T>& buffer, 
		const unsigned int&  stride_in_bytes)
	{
		copy_from_device<T>(buffer, stride_in_bytes, get_size_in_bytes());
	}

	template<class T>
	inline void BufferGL::copy_from_system(
		const std::vector<T>& buffer, 
		const unsigned int&  stride_in_bytes,
		const std::size_t& size_in_bytes)
	{
		copy_from_device(buffer.data(), stride_in_bytes, size_in_bytes);
	}

	template<class T>
	inline std::vector<T> BufferGL::copy_from_device()
	{
		return copy_from_device<T>(0, get_size_in_bytes());
	}

	template<class T>
	inline std::vector<T> BufferGL::copy_from_device(const unsigned int&  stride_in_bytes)
	{
		return copy_from_device<T>(stride_in_bytes, get_size_in_bytes());
	}

	template<class T>
	inline std::vector<T> BufferGL::copy_from_device(
		const unsigned int&  stride_in_bytes, 
		const std::size_t& size_in_bytes)
	{
		std::vector<T> buffer;
		buffer.resize(size_in_bytes / sizeof(T));
		copy_from_device(buffer.data(), stride_in_bytes, size_in_bytes);
		return buffer;
	}
}
