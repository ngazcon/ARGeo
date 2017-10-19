#pragma once

#include "ImageDataType.h"
#include "ImageFormat.h"
#include "PixelFormat.h"

#include <iostream>

namespace argeo
{
	class TextureHelper
	{
	public:
		static bool is_power_of_two(unsigned int i)
		{
			return (i != 0) && ((i & (i - 1)) == 0);
		}

		static std::size_t required_size_in_bytes(
			int width,
			int height,
			ImageFormat format,
			ImageDataType data_type,
			int row_alignment)
		{
			std::size_t row_size = width * number_of_channels(format) * size_in_bytes(data_type);

			std::size_t remainder = (row_size % row_alignment);
			row_size += (row_alignment - remainder) % row_alignment;

			return row_size * height;
		}

		static TextureFormat imaging_pixel_format_to_texture_format(PixelFormat pixel_format)
		{
			if (!supported(pixel_format))
			{
				throw std::invalid_argument("Pixel format is not supported.");
			}

			// TODO:  Not tested exhaustively
			switch (pixel_format)
			{;
			case PixelFormat::Format24bppRgb:
				return TextureFormat::RedGreenBlue8;
			case PixelFormat::Format32bppRgb:
				return TextureFormat::Red11fGreen11fBlue10f;
			case PixelFormat::Format32bppArgb:
				return TextureFormat::RedGreenBlueAlpha8;
			case PixelFormat::Format48bppRgb:
				return TextureFormat::RedGreenBlue16;
			case PixelFormat::Format64bppArgb:
				return TextureFormat::RedGreenBlueAlpha16;
			}

			throw std::invalid_argument("pixelFormat");
		}

		static ImageDataType imaging_pixel_format_to_data_type(PixelFormat pixel_format)
		{
			if (!supported(pixel_format))
			{
				throw std::invalid_argument("Pixel format is not supported.");
			}

			// TODO:  Not tested exhaustively
			switch (pixel_format)
			{
			case PixelFormat::Format16bppRgb555:
				return ImageDataType::UnsignedShort5551;
			case PixelFormat::Format16bppRgb565:
				return ImageDataType::UnsignedShort565;
			case PixelFormat::Format24bppRgb:
			case PixelFormat::Format32bppRgb:
			case PixelFormat::Format32bppArgb:
				return ImageDataType::UnsignedByte;
			case PixelFormat::Format48bppRgb:
			case PixelFormat::Format64bppArgb:
				return ImageDataType::UnsignedShort;
			case PixelFormat::Format16bppArgb1555:
				return ImageDataType::UnsignedShort1555Reversed;
			}

			throw std::invalid_argument("pixelFormat");
		}

		static ImageFormat imaging_pixel_format_to_image_format(PixelFormat pixel_format)
		{
			if (!supported(pixel_format))
			{
				throw std::invalid_argument("Pixel format is not supported.");
			}

			switch (pixel_format)
			{
			case PixelFormat::Format16bppRgb555:
			case PixelFormat::Format16bppRgb565:
			case PixelFormat::Format24bppRgb:
			case PixelFormat::Format32bppRgb:
			case PixelFormat::Format48bppRgb:
				return ImageFormat::RedGreenBlue; 
			case PixelFormat::Format16bppArgb1555:
			case PixelFormat::Format32bppArgb:
			case PixelFormat::Format64bppArgb:
				return ImageFormat::RedGreenBlueAlpha;
			}

			throw std::invalid_argument("Pixel format is not supported.");
		}

		static bool supported(PixelFormat pixelFormat)
		{
			return
				(pixelFormat != PixelFormat::DontCare) &&
				(pixelFormat != PixelFormat::Undefined) &&
				(pixelFormat != PixelFormat::Indexed) &&
				(pixelFormat != PixelFormat::Gdi) &&
				(pixelFormat != PixelFormat::Extended) &&
				(pixelFormat != PixelFormat::Format1bppIndexed) &&
				(pixelFormat != PixelFormat::Format4bppIndexed) &&
				(pixelFormat != PixelFormat::Format8bppIndexed) &&
				(pixelFormat != PixelFormat::Alpha) &&
				(pixelFormat != PixelFormat::PAlpha) &&
				(pixelFormat != PixelFormat::Format32bppPArgb) &&
				(pixelFormat != PixelFormat::Format16bppGrayScale) &&
				(pixelFormat != PixelFormat::Format64bppPArgb) &&
				(pixelFormat != PixelFormat::Canonical);
		}

		static int number_of_channels(ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::StencilIndex:
				return 1;
			case ImageFormat::DepthComponent:
				return 1;
			case ImageFormat::Red:
				return 1;
			case ImageFormat::Green:
				return 1;
			case ImageFormat::Blue:
				return 1;
			case ImageFormat::RedGreenBlue:
				return 3;
			case ImageFormat::RedGreenBlueAlpha:
				return 4;
			case ImageFormat::BlueGreenRed:
				return 3;
			case ImageFormat::BlueGreenRedAlpha:
				return 4;
			case ImageFormat::RedGreen:
				return 2;
			case ImageFormat::RedGreenInteger:
				return 2;
			case ImageFormat::DepthStencil:
				return 2;
			case ImageFormat::RedInteger:
				return 1;
			case ImageFormat::GreenInteger:
				return 1;
			case ImageFormat::BlueInteger:
				return 1;
			case ImageFormat::RedGreenBlueInteger:
				return 3;
			case ImageFormat::RedGreenBlueAlphaInteger:
				return 4;
			case ImageFormat::BlueGreenRedInteger:
				return 3;
			case ImageFormat::BlueGreenRedAlphaInteger:
				return 4;
			}
		}

		static int size_in_bytes(ImageDataType data_type)
		{
			switch (data_type)
			{
			case ImageDataType::Byte:
				return 1;
			case ImageDataType::UnsignedByte:
				return 1;
			case ImageDataType::Short:
				return 2;
			case ImageDataType::UnsignedShort:
				return 2;
			case ImageDataType::Int:
				return 4;
			case ImageDataType::UnsignedInt:
				return 4;
			case ImageDataType::Float:
				return 4;
			case ImageDataType::HalfFloat:
				return 2;
			case ImageDataType::UnsignedByte332:
				return 1;
			case ImageDataType::UnsignedShort4444:
				return 2;
			case ImageDataType::UnsignedShort5551:
				return 2;
			case ImageDataType::UnsignedInt8888:
				return 4;
			case ImageDataType::UnsignedInt1010102:
				return 4;
			case ImageDataType::UnsignedByte233Reversed:
				return 1;
			case ImageDataType::UnsignedShort565:
				return 2;
			case ImageDataType::UnsignedShort565Reversed:
				return 2;
			case ImageDataType::UnsignedShort4444Reversed:
				return 2;
			case ImageDataType::UnsignedShort1555Reversed:
				return 2;
			case ImageDataType::UnsignedInt8888Reversed:
				return 4;
			case ImageDataType::UnsignedInt2101010Reversed:
				return 4;
			case ImageDataType::UnsignedInt248:
				return 4;
			case ImageDataType::UnsignedInt10F11F11FReversed:
				return 4;
			case ImageDataType::UnsignedInt5999Reversed:
				return 4;
			case ImageDataType::Float32UnsignedInt248Reversed:
				return 4;
			}
		}
	};
}
