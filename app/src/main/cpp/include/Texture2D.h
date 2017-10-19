#pragma once

#include "Texture.h"
#include "ImageDataType.h"
#include "ImageFormat.h"
#include "ReadPixelBuffer.h"
#include "WritePixelBuffer.h"

#include <string>
#include <memory>

namespace argeo
{
	class Texture2D : public Texture
	{
	public:

		virtual ~Texture2D() { }

		virtual void set_sampler(TextureSampler* value) = 0;
		virtual TextureSampler* get_sampler() = 0;

		template<class T>
		T read_pixel(
			int x_offset,
			int y_offset,
			ImageFormat format      = ImageFormat::RedGreenBlueAlpha,
			ImageDataType data_type = ImageDataType::UnsignedByte) const;

		virtual void read_pixel(
			void* pixel,
			int x_offset,
			int y_offset,
			ImageFormat format      = ImageFormat::RedGreenBlueAlpha,
			ImageDataType data_type = ImageDataType::UnsignedByte) const = 0;

		virtual void copy_from_buffer(
			WritePixelBuffer* pixel_buffer,
			ImageFormat format,
			ImageDataType data_type) const = 0;

		virtual void copy_from_buffer(
			WritePixelBuffer* pixel_buffer,
			ImageFormat format,
			ImageDataType data_type,
			int row_alignment) const = 0;

		virtual void copy_from_buffer(
			WritePixelBuffer* pixel_buffer,
			int x_offset,
			int y_offset,
			int width,
			int height,
			ImageFormat format,
			ImageDataType data_type,
			int row_alignment) const = 0;

		virtual std::unique_ptr<ReadPixelBuffer> copy_to_buffer(
			ImageFormat format,
			ImageDataType data_type) const = 0;

		virtual std::unique_ptr<ReadPixelBuffer> copy_to_buffer(
			ImageFormat format,
			ImageDataType data_type,
			int row_alignment) const = 0;
	};

	template<class T>
	inline T Texture2D::read_pixel(
		int x_offset,
		int y_offset,
		ImageFormat format,
		ImageDataType data_type) const
	{
		T pixel;
		read_pixel(&pixel, x_offset, y_offset, format, data_type);
		return pixel;
	}
}
