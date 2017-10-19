#pragma once

#include "Color.h"

namespace argeo
{
	class ColorHelper
	{
	public:
		static Color black_color() { return Color(0, 0, 0, 255); }
		static Color white_color() { return Color(255, 255, 255, 255); }
		static Color red_color()   { return Color(255, 0, 0, 255); }
		static Color green_color() { return Color(0, 255, 0, 255); }

		static vec4 color_to_vec4(const Color& color)
		{
			return vec4(
				(float)color.red / 255.0f,
				(float)color.green / 255.0f,
				(float)color.blue / 255.0f,
				(float)color.alpha / 255.0f);
		}

		static unsigned int color_to_hexa_rgba(const Color& color)
		{
			return ((color.red & 0xff) << 24)
				+ ((color.green & 0xff) << 16)
				+ ((color.blue & 0xff) << 8)
				+ ((color.alpha & 0xff));
		}

		static unsigned int color_to_hexa_argb(const Color& color)
		{
			return ((color.red & 0xff) << 16)
				+ ((color.green & 0xff) << 8)
				+ ((color.blue & 0xff))
				+ ((color.alpha & 0xff) << 24);
		}
	};
}