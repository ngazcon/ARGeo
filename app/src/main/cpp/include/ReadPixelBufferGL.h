#pragma once

#include "ReadPixelBuffer.h"
#include "PixelBufferGL.h"
#include "ImageFormat.h"
#include "ImageDataType.h"

#include <memory>

namespace argeo
{
	class ReadPixelBufferGL : public ReadPixelBuffer
	{
		friend class Device;
		friend class Texture2DArrayGL;
		friend class Texture2DGL;
		friend class TextureExternal;

	protected:

		ReadPixelBufferGL(
			const PixelBufferHint& hint, 
			const std::size_t& size_in_bytes);

		~ReadPixelBufferGL();

	public:

		void bind();
		static void unbind();

		void copy_from_system(
			const void* buffer,
			const std::size_t& offset_in_bytes,
			const std::size_t& size_in_bytes);

		void copy_from_device(
			void* buffer,
			const std::size_t& offset_in_bytes,
			const std::size_t& size_in_bytes);

		void* map_range(
			const std::size_t& offset_in_bytes,
			const std::size_t& size_in_bytes,
			const BufferAccess& access);

		void unmap();

		std::size_t get_size_in_bytes() const;

		PixelBufferHint get_hint() const;

		unsigned int get_handle() const;

		void transfer(
			FrameBuffer* frame_buffer,
			const unsigned int& x,
			const unsigned int& y,
			const unsigned int& width,
			const unsigned int& height,
			const ImageFormat& format = ImageFormat::RedGreenBlueAlpha,
			const ImageDataType& data_type = ImageDataType::UnsignedByte,
			const FrameBufferAttachment& attachment = FrameBufferAttachment::ColorAttachment0);

	private:
		std::unique_ptr<PixelBufferGL> m_buffer_object;
		PixelBufferHint  m_hint;
	};
}
