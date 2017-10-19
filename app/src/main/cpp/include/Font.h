#pragma once

#include "Color.h"
#include "FontStyle.h"

#include <string>

namespace argeo
{
	class Font
	{
	public:
		Font(
			std::string name,
			int size);

		~Font();

		Color get_color() const;

		std::string get_name() const;

		float get_size() const;

		FontStyle get_style() const;

	private:
		Color m_color;
		FontStyle m_style;
		std::string m_name;
		float m_size;

	};
}
