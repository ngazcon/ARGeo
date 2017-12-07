#pragma once

#include "WritePixelBuffer.h"
#include "PixelBufferGL.h"

#include <memory>

namespace argeo
{
	class WritePixelBufferGL : public WritePixelBuffer
	{
		friend class Device;
		friend class Texture2DArrayGL;
		friend class Texture2DGL;

	protected:

		WritePixelBufferGL(
			const PixelBufferHint& hint, 
			const std::size_t& size_in_bytes);

		
	public:

		~WritePixelBufferGL();

		void bind();

		static void unbind();

		void copy_from_system(
			const void* buffer,
			const unsigned int& offset_in_bytes,
			const std::size_t& size_in_bytes);

		void copy_from_device(
			void* buffer,
			const unsigned int& offset_in_bytes,
			const std::size_t& size_in_bytes);

		void* map_range(
			const unsigned int& offset_in_bytes,
			const std::size_t& size_in_bytes,
			const BufferAccess& access);

		void unmap();

		std::size_t get_size_in_bytes() const;

		PixelBufferHint get_hint() const;

		unsigned int get_handle() const;

		void copy_from_bitmap(
				Bitmap* bitmap,
				const bool& flip = false);

		std::unique_ptr<Bitmap> copy_to_bitmap(
			const unsigned int& width,
			const unsigned int& height,
			const PixelFormat& pixel_format);

	private:
		std::unique_ptr<PixelBufferGL> m_buffer_object;
		PixelBufferHint m_hint;
	};
}
