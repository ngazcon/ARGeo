#pragma once

#include "Texture2D.h"
#include "TextureTarget.h"
#include "TextureSamplerGL.h"
#include "TextureGL.h"

namespace argeo
{
	class TextureExternal : public Texture2D, public TextureGL
	{
	public:

		TextureExternal(
			TextureSampler* sampler = nullptr);

		~TextureExternal();

		void set_sampler(TextureSampler* value);

		TextureSampler* get_sampler();

		void bind() const;

		void read_pixel(
			void* pixel,
			int x_offset,
			int y_offset,
			ImageFormat format,
			ImageDataType data_type) const;

		void copy_from_buffer(
			WritePixelBuffer* pixel_buffer,
			ImageFormat format,
			ImageDataType data_type) const;

		void copy_from_buffer(
			WritePixelBuffer* pixel_buffer,
			ImageFormat format,
			ImageDataType data_type,
			int row_alignment) const;

		void copy_from_buffer(
			WritePixelBuffer* pixel_buffer,
			int x_offset,
			int y_offset,
			int width,
			int height,
			ImageFormat format,
			ImageDataType data_type,
			int row_alignment) const;

		std::unique_ptr<ReadPixelBuffer> copy_to_buffer(
			ImageFormat format,
			ImageDataType data_type) const;

		std::unique_ptr<ReadPixelBuffer> copy_to_buffer(
			ImageFormat format,
			ImageDataType data_type,
			int row_alignment) const;

		TextureDescription get_description() const;
		TextureTarget get_target() const;

		unsigned int get_handle() const;

	protected:

		void apply_sampler(TextureSampler* sampler);
		void verify_row_alignment(int row_alignment) const;
		void generate_mipmaps() const;

		void bind_to_last_texture_unit() const;

	private:
		TextureSamplerGL* m_sampler;
		unsigned int m_last_texture_unit;
		unsigned int m_handle;
		TextureTarget m_target;
		TextureDescription m_description;
	};
}