#pragma once

#include "ImageDataType.h"
#include "ImageFormat.h"
#include "ReadPixelBuffer.h"
#include "WritePixelBuffer.h"

#include "Texture2D.h"
#include "TextureDescription.h"
#include "TextureTarget.h"

#include <string>
#include <memory>

namespace argeo
{
	class Texture2DArray : public Texture
	{
	public:

		virtual ~Texture2DArray() { }

		virtual void set_sampler(TextureSampler* value) = 0;
		virtual TextureSampler* get_sampler() = 0;

		virtual void copy_from_buffer(
			WritePixelBuffer* pixel_buffer,
			ImageFormat format,
			ImageDataType data_type) const = 0;

		virtual void copy_from_buffer(
			WritePixelBuffer* pixel_buffer,
			int layer,
			ImageFormat format,
			ImageDataType data_type) const = 0;

		virtual void copy_from_buffer(
			WritePixelBuffer* pixel_buffer,
			int layer,
			ImageFormat format,
			ImageDataType data_type,
			int row_alignment) const = 0;

		virtual void copy_from_buffer(
			WritePixelBuffer* pixel_buffer,
			int x_offset,
			int y_offset,
			int width,
			int height,
			int layer,
			ImageFormat format,
			ImageDataType data_type,
			int row_alignment) const = 0;

		virtual std::unique_ptr<ReadPixelBuffer> copy_to_buffer(
			ImageFormat format,
			ImageDataType data_type,
			int layer) const = 0;

		virtual std::unique_ptr<ReadPixelBuffer> copy_to_buffer(
			ImageFormat format,
			ImageDataType data_type,
			int layer,
			int row_alignment) const = 0;

	};
}
