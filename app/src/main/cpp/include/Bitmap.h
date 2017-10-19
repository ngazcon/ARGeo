#pragma once

#include "PixelFormat.h"
#include "Font.h"

#include <string>
#include <memory>
#include <iostream>

namespace argeo
{
	class Bitmap
	{

		///
		/// Represent a image stored as a pixel map, that is an array of pixels.
		///

	public:

		///
		/// Construct a Bitmap from a file handler.
		/// @constructor
		/// @param {FILE*} file The file handler.
		///
		Bitmap(FILE* file);

		///
		/// Construct a empty Bitmap with a given width and height and using a custom pixel format.
		/// @constructor
		/// @param {unsigned int} width The width.
		/// @param {unsigned int} height The height.
		/// @param {unsigned int} pixel_format The internal pixel format.
		///
		Bitmap(
			const unsigned int& width,
			const unsigned int& height,
			const PixelFormat& pixel_format);


		Bitmap(void* handle);

		///
		/// @destructor
		///
		~Bitmap();


		void recycle();

		///
		/// Get the pixels of this bitmap.
		/// @return {unsigned char*} The pixels of this bitmap.
		///
		void get_pixels(unsigned char* pixels) const;

		///
		/// Set the pixels of this bitmap.
		/// @param {unsigned char*} Set pixels of this bitmap.
		///
		void set_pixels(unsigned char* pixels);

		///
		/// Set the pixels of this bitmap.
		/// @param {unsigned int*} Set pixels of this bitmap.
		///
		void set_pixels(unsigned int* pixels);

		///
		/// Get the size in bytes of this bitmap.
		/// @return {unsigned int} The size in bytes of this bitmap.
		///
		std::size_t get_size_in_bytes() const;

		///
		/// Get the size in bytes of this bitmap.
		/// @return {unsigned int} The size in bytes of this bitmap.
		///
		unsigned int get_stride() const;

		///
		/// Get the width of this bitmap.
		/// @return {unsigned int} The width of this bitmap.
		///
		unsigned int get_width() const;

		///
		/// Get the height of this bitmap.
		/// @return {unsigned int} The height of this bitmap.
		///
		unsigned int get_height() const;

		///
		/// Get the pixel format bitmap.
		/// @return {PixelFormat} The pixel format of this bitmap.
		///
		PixelFormat get_pixel_format() const;


		///
		/// Create a bitmap from a given string using a custom font.
		/// @param {std::string} text The text to render.
		/// @param {Font} font The font to use.
		/// @return {std::unique_ptr<Bitmap>} The bitmap.
		///
		static std::unique_ptr<Bitmap> create_bitmap_from_text(
			std::string text,
			Font font);

		///
		/// Create a bitmap from a given asset file.
		/// @param {std::string} asset_path The path to the asset.
		/// @return {std::unique_ptr<Bitmap>} The bitmap.
		///
		static std::unique_ptr<Bitmap> create_bitmap_from_asset(
			std::string asset_path);

		void* get_handle() const;

	private:
		unsigned int m_stride;
		unsigned int m_width;
		unsigned int m_height;
		PixelFormat m_pixel_format;
		void* m_handle;
	};

}
