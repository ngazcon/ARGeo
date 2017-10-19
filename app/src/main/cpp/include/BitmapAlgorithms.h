#pragma once

#include "Bitmap.h"
#include "Rectangle.h"

namespace argeo
{
	enum class ImageRowOrder : unsigned int
	{
		BottomToTop = 0,
		TopToBottom = 1,
	};

	class BitmapAlgorithms
	{

		///
		/// A class with helper functions for a {@ref Bitmap}
		///
	public:

		///
		/// Get the row order of a bitmap.
		/// @param {Bitmap} bitmap The bitmap.
		/// @return {ImageRowOrder} The row order.
		///
		static ImageRowOrder row_order(Bitmap* bitmap)
		{
			return bitmap->get_stride() > 0 ?
				ImageRowOrder::TopToBottom :
				ImageRowOrder::BottomToTop;
		}

		///
		/// Get size per pixel of a pixel format.
		/// @param {PixelFormat} bitmap The bitmap.
		/// @return {unsigned int} The size of each pixel in bytes.
		///
		static unsigned int pixel_format_to_bpp(PixelFormat pixel_format)
		{
			switch (pixel_format)
			{
			case PixelFormat::Format16bppRgb555:
			case PixelFormat::Format16bppRgb565:
			case PixelFormat::Format16bppArgb1555:
			case PixelFormat::Format16bppGrayScale:
				return 16;

			case PixelFormat::Format24bppRgb:
				return 24;

			case PixelFormat::Format32bppArgb:
			case PixelFormat::Format32bppPArgb:
			case PixelFormat::Format32bppRgb:
				return 32;

			case PixelFormat::Format48bppRgb:
				return 48;

			case PixelFormat::Format64bppArgb:
			case PixelFormat::Format64bppPArgb:
				return 64;
			}
		}

		///
		/// Get size per pixel of each pixel.
		/// @param {Bitmap} bitmap The bitmap.
		/// @return {unsigned int} The size of each pixel in bytes.
		///
		static unsigned int size_of_pixels_in_bytes(Bitmap* bitmap)
		{
			return pixel_format_to_bpp(bitmap->get_pixel_format());
		}

		///
		/// Get size per pixel of each pixel.
		/// @param {Bitmap} bitmap The bitmap.
		/// @return {unsigned int} The size of each pixel in bytes.
		///
		static void rgba_to_argb(
			unsigned char* in,
			unsigned char* out,
			int width,
			int height)
		{
			unsigned int pixel;
			int i, j, offset;

			for (i = 0; i < height; ++i)
			{
				offset = i * width * 4;
				for (j = 0; j < width * 4; j += 4)
				{
					pixel = in[i * width + j / 4];

					// argb (out)
					// rgba (in)

					unsigned char red     = (pixel >> 24) & 0xff;
					unsigned char green   = (pixel >> 16) & 0xff;
					unsigned char blue    = (pixel >> 8) & 0xff;
					unsigned char alpha   = (pixel) & 0xff;

					out[offset + j]     = red;
					out[offset + j + 1] = green;
					out[offset + j + 2] = blue;
					out[offset + j + 3] = alpha;
				}
			}
		}

		///
		/// Get size per pixel of each pixel.
		/// @param {Bitmap} bitmap The bitmap.
		/// @return {unsigned int} The size of each pixel in bytes.
		///
		static void argb_to_rgba(
			unsigned char* in,
			unsigned char* out,
			int width,
			int height)
		{
			unsigned int pixel;
			int i, j, offset;

			for (i = 0; i < height; ++i)
			{
				offset = i * width * 4;
				for (j = 0; j < width * 4; j += 4)
				{
					pixel = in[i * width + j / 4];

					//Get and set the pixel channel values from/to int  //TODO OPTIMIZE!
					unsigned char alpha = (pixel >> 24) & 0xff;
					unsigned char red = (pixel >> 16) & 0xff;
					unsigned char green = (pixel >> 8) & 0xff;
					unsigned char blue = (pixel) & 0xff;

					out[offset + j] = red;
					out[offset + j + 1] = green;
					out[offset + j + 2] = blue;
					out[offset + j + 3] = alpha;
				}
			}
		}


        static void rgba_int32_to_rgba_ubyte(
                unsigned int* in,
                unsigned char* out,
                int width,
                int height,
                bool flip = false)
        {
            unsigned int pixel;
            int i, j, offset;
            if (flip)
            {
                for (i = 0; i < height; ++i)
                {
                    offset = i * width * 4;
                    unsigned int* row = in + (height - i - 1) * width;
                    for (j = 0; j < width; ++j)
                    {
                        pixel = row[j];
                        out[offset + (j * 4) + 0] = (pixel >> 0)  & 0xff;
                        out[offset + (j * 4) + 1] = (pixel >> 8)  & 0xff;
                        out[offset + (j * 4) + 2] = (pixel >> 16) & 0xff;
                        out[offset + (j * 4) + 3] = (pixel >> 24) & 0xff;
                    }
                }
            }
            else
            {
                for (i = 0; i < height; ++i)
                {
                    offset = i * width * 4;
                    unsigned int* row = in + i * width;
                    for (j = 0; j < width; ++j)
                    {
                        pixel = row[j];
                        out[offset + (j * 4) + 0] = (pixel >> 0)  & 0xff;
                        out[offset + (j * 4) + 1] = (pixel >> 8)  & 0xff;
                        out[offset + (j * 4) + 2] = (pixel >> 16) & 0xff;
                        out[offset + (j * 4) + 3] = (pixel >> 24) & 0xff;
                    }
                }
            }
        }

		static void argb_int32_to_rgba_ubyte(
			unsigned int* in,
			unsigned char* out,
			int width,
			int height,
			bool flip = false)
		{
			unsigned int pixel;
			int i, j, offset;
			if (flip)
			{
				for (i = 0; i < height; ++i)
				{
					offset = i * width * 4;
					unsigned int* row = in + (height - i - 1) * width;
					for (j = 0; j < width; ++j)
					{
						pixel = row[j];
						out[offset + (j * 4) + 0] = (pixel >> 16) & 0xff;
						out[offset + (j * 4) + 1] = (pixel >> 8) & 0xff;
						out[offset + (j * 4) + 2] = (pixel) & 0xff;
						out[offset + (j * 4) + 3] = (pixel >> 24) & 0xff;
					}
				}
			}
			else
			{
				for (i = 0; i < height; ++i)
				{
					offset = i * width * 4;
					unsigned int* row = in + i * width;
					for (j = 0; j < width; ++j)
					{
						pixel = row[j];
						out[offset + (j * 4) + 0] = (pixel >> 16) & 0xff;
						out[offset + (j * 4) + 1] = (pixel >> 8) & 0xff;
						out[offset + (j * 4) + 2] = (pixel) & 0xff;
						out[offset + (j * 4) + 3] = (pixel >> 24) & 0xff;
					}
				}
			}
		}

		static void rgb_int32_to_rgb_ubyte(
			unsigned int* in,
			unsigned char* out,
			int width,
			int height,
			bool flip = false)
		{
			unsigned int pixel;
			int i, j, offset;

			if (flip)
			{
				for (i = 0; i < height; ++i)
				{
					offset = i * width * 3;
					unsigned int* row = static_cast<unsigned int*>(
						in + (height - i - 1) * width);
					for (j = 0; j < width; ++j)
					{
						pixel = row[j];
						out[offset + (j * 3) + 0] = (pixel >> 16) & 0xff;
						out[offset + (j * 3) + 1] = (pixel >> 8) & 0xff;
						out[offset + (j * 3) + 2] = (pixel) & 0xff;
					}
				}
			}
			else
			{
				for (i = 0; i < height; ++i)
				{
					offset = i * width * 3;
					unsigned int* row = static_cast<unsigned int*>(
						in + i * width);
					for (j = 0; j < width; ++j)
					{
						pixel = row[j];
						out[offset + (j * 3) + 0] = (pixel >> 16) & 0xff;
						out[offset + (j * 3) + 1] = (pixel >> 8) & 0xff;
						out[offset + (j * 3) + 2] = (pixel) & 0xff;
					}
				}
			}
		}

        static void rgba_ubyte_argb_int32(
            unsigned char* in,
            unsigned int* out,
            const unsigned int& width,
            const unsigned int& height,
            const bool& flip = false)
        {
            int i, j, offset;

            if (!flip)
            {
                for (i = 0; i < height; ++i)
                {
                    offset = i * width * 4;
                    unsigned int* row = static_cast<unsigned int*>(out + i * width);
                    for (j = 0; j < width; ++j)
                    {
                        row[j] =
                            (in[offset + (j * 4) + 3] << 24) |
                            (in[offset + (j * 4) + 0] << 16) |
                            (in[offset + (j * 4) + 1] << 8) |
                            (in[offset + (j * 4) + 2]);
                    }
                }
            }
            else
            {
                for (i = 0; i < height; ++i)
                {
                    offset = i * width * 4;
                    unsigned int* row = static_cast<unsigned int*>(out + (height - i - 1) * width);
                    for (j = 0; j < width; ++j)
                    {
                        row[j] =
                            (in[offset + (j * 4) + 3] << 24) |
                            (in[offset + (j * 4) + 0] << 16) |
                            (in[offset + (j * 4) + 1] << 8) |
                            (in[offset + (j * 4) + 2]);
                    }
                }
            }
        }

        static void rgba_ubyte_rgba_int32(
                unsigned char* in,
                unsigned int* out,
                const unsigned int& width,
                const unsigned int& height,
                const bool& flip = false)
        {
            int i, j, offset;

            if (!flip)
            {
                for (i = 0; i < height; ++i)
                {
                    offset = i * width * 4;
                    unsigned int* row = static_cast<unsigned int*>(out + i * width);
                    for (j = 0; j < width; ++j)
                    {
                        row[j] =
                                (in[offset + (j * 4) + 0] << 0) |
                                (in[offset + (j * 4) + 1] << 8) |
                                (in[offset + (j * 4) + 2] << 16) |
                                (in[offset + (j * 4) + 3] << 24);
                    }
                }
            }
            else
            {
                for (i = 0; i < height; ++i)
                {
                    offset = i * width * 4;
                    unsigned int* row = static_cast<unsigned int*>(out + (height - i - 1) * width);
                    for (j = 0; j < width; ++j)
                    {
                        row[j] =
                                (in[offset + (j * 4) + 0] << 0) |
                                (in[offset + (j * 4) + 1] << 8) |
                                (in[offset + (j * 4) + 2] << 16) |
                                (in[offset + (j * 4) + 3] << 24);
                    }
                }
            }
        }

		static void bgra_ubyte_argb_int32(
				unsigned char* in,
				unsigned int* out,
				const unsigned int& width,
				const unsigned int& height,
				const bool& flip = false)
		{
			int i, j, offset;

			if (!flip)
			{
				for (i = 0; i < height; ++i)
				{
					offset = i * width * 4;
					unsigned int* row = static_cast<unsigned int*>(out + i * width);
					for (j = 0; j < width; ++j)
					{
						row[j] =
                                (in[offset + (j * 4) + 3] << 24) |
                                (in[offset + (j * 4) + 2] << 16) |
                                (in[offset + (j * 4) + 1] << 8) |
                                (in[offset + (j * 4) + 0]);
					}
				}
			}
			else
			{
				for (i = 0; i < height; ++i)
				{
					offset = i * width * 4;
					unsigned int* row = static_cast<unsigned int*>(out + (height - i - 1) * width);
					for (j = 0; j < width; ++j)
					{
						row[j] =
								(in[offset + (j * 4) + 3] << 24) |
								(in[offset + (j * 4) + 2] << 16) |
								(in[offset + (j * 4) + 1] << 8) |
								(in[offset + (j * 4) + 0]);
					}
				}
			}
		}

			static void rgb_ubyte_to_rgb_int32(
				unsigned int* in,
				unsigned char* out,
				int width,
				int height)
			{
				int i, j, offset;
				for (i = 0; i < height; ++i)
				{
					offset = i * width * 4;
					for (j = 0; j < width; ++j)
					{
						in[offset + j] =
							(out[offset + j + 0] << 16) |
							(out[offset + j + 1] << 8) |
							(out[offset + j + 2]);
					}
				}
			}
	};
}
