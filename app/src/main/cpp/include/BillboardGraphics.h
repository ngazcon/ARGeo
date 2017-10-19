#pragma once

#include "Bitmap.h"
#include "Vector.h"
#include "Billboard.h"
#include "IGraphics.h"

#include <string>

namespace argeo
{
	class BillboardGraphics 
		: public IGraphics
	{
	public:
		BillboardGraphics(
			const std::string& image_path,
			const std::string& image_id = std::string(),
			const unsigned int& width = Billboard::IMAGE_WIDTH,
			const unsigned int& height = Billboard::IMAGE_HEIGHT,
            const HeightClamping& height_clamping = HeightClamping::Ground);

		BillboardGraphics(
			Bitmap* image,
			const std::string& image_id = std::string(),
			const unsigned int& width   = Billboard::IMAGE_WIDTH,
			const unsigned int& height  = Billboard::IMAGE_HEIGHT,
            const HeightClamping& height_clamping = HeightClamping::Ground);

		~BillboardGraphics();

		std::string get_image_id() const;

		void  set_pixel_offset(const vec2i& value);
		vec2i get_pixel_offset() const;

		Bitmap* get_image();
		void set_image(Bitmap* value);

        void set_height_clamping(const HeightClamping& value);
        HeightClamping get_height_clamping() const;

		std::string get_image_path();
		void set_image_path(const std::string& value);

		bool get_show() const;
		void set_show(const bool& value);

		void set_width(const unsigned int& value);
		unsigned int get_width() const;

		void set_height(const unsigned int& value);
		unsigned int get_height() const;

	private:
		bool m_show;

		Bitmap*     m_image;
		std::string m_image_path;
		std::string m_image_id;

		unsigned int m_width;
		unsigned int m_height;
        HeightClamping m_height_clamping;

		vec2i m_pixel_offset;
	};
}
