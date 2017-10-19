#pragma once

#include "PixelBuffer.h"
#include "BufferTarget.h"
#include "BufferHint.h"

#include <vector>

namespace argeo
{
	class PixelBufferGL : public PixelBuffer
	{
		friend class WritePixelBufferGL;
		friend class ReadPixelBufferGL;

	protected:

		///
		/// Constructs a OpenGL buffer.
		/// @constructor
		/// @param {BufferTarget} target The buffer target.
		/// @param {BufferHint} hin The use hint of the buffer.
		/// @param {std::size_t} size_in_bytes The size of the buffer in bytes.
		///
		PixelBufferGL(
			const std::size_t& size_in_bytes,
			const BufferTarget& target,
			const BufferHint& hint);

	public:
		~PixelBufferGL();

		///
		/// @copydoc PixelBuffer::bind().
		///
		void bind();

		///
		/// @copydoc PixelBuffer::unbind().
		///
		void unbind();


		///
		/// @copydoc PixelBuffer::get_size_in_bytes().
		///
		std::size_t get_size_in_bytes() const;

		///
		/// @copydoc PixelBuffer::get_hint().
		///
		BufferHint get_hint() const;

		///
		/// @copydoc PixelBuffer::copy_from_device(void*, const unsigned int&, const std::size_t&).
		///
		void copy_from_device(
			void* buffer, 
			const unsigned int& offset_in_bytes, 
			const std::size_t& size_in_bytes);

		///
		/// @copydoc PixelBuffer::copy_from_system(const void*, const unsigned int&, const std::size_t&).
		///
		void copy_from_system(
			const void* buffer, 
			const unsigned int& offset_in_bytes, 
			const std::size_t& size_in_bytes);

		///
		/// @copydoc PixelBuffer::map_range(const unsigned int&, const std::size_t&, const BufferAccess&).
		///
		void* map_range(
			const unsigned int& offset_in_bytes,
			const std::size_t& size_in_bytes,
			const BufferAccess& access);

		///
		/// @copydoc PixelBuffer::unmap().
		///
		void unmap();

		///
		/// @copydoc PixelBuffer::copy_from_bitmap(Bitmap*).
		///
		void copy_from_bitmap(Bitmap* bitmap);

		///
		/// @copydoc PixelBuffer::copy_to_bitmap(const unsigned int&, const unsigned int&, const PixelFormat&).
		///
		std::unique_ptr<Bitmap> copy_to_bitmap(
			const unsigned int& width,
			const unsigned int& height,
			const PixelFormat& pixel_format);

		///
		/// Gets the handle to the openGL object.
		/// @return {unsigned int} The handle to this buffer object.
		///
		unsigned int get_handle() const;

	private:
		unsigned int m_handle;
		std::size_t m_size_in_bytes;
		BufferTarget m_target;
		BufferHint m_hint;
	};


	
}
