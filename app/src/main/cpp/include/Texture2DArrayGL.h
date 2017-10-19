#pragma once

#include "Texture2DArray.h"
#include "TextureTarget.h"

#include "TextureSamplerGL.h"
#include "TextureGL.h"

namespace argeo
{
	class Texture2DArrayGL : public Texture2DArray, public TextureGL
	{
	public:

		Texture2DArrayGL(
			TextureDescription description,
			TextureTarget texture_target,
			TextureSampler* sampler = nullptr);

		~Texture2DArrayGL();

		void set_sampler(TextureSampler* value);
		TextureSampler* get_sampler();

		void bind() const;

		void copy_from_buffer(
			WritePixelBuffer* pixel_buffer,
			ImageFormat format,
			ImageDataType data_type) const;

		void copy_from_buffer(
			WritePixelBuffer* pixel_buffer,
			int layer,
			ImageFormat format,
			ImageDataType data_type) const;

		void copy_from_buffer(
			WritePixelBuffer* pixel_buffer,
			int layer,
			ImageFormat format,
			ImageDataType data_type,
			int row_alignment) const;

		void copy_from_buffer(
			WritePixelBuffer* pixel_buffer,
			int x_offset,
			int y_offset,
			int width,
			int height,
			int layer,
			ImageFormat format,
			ImageDataType data_type,
			int row_alignment) const;

		std::unique_ptr<ReadPixelBuffer> copy_to_buffer(
			ImageFormat format,
			ImageDataType data_type,
			int layer) const;

		std::unique_ptr<ReadPixelBuffer> copy_to_buffer(
			ImageFormat format,
			ImageDataType data_type,
			int layer,
			int row_alignment) const;

		TextureDescription get_description() const;
		TextureTarget get_target() const;

		unsigned int get_handle() const;

	protected:

		void apply_sampler(TextureSampler * sampler);
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