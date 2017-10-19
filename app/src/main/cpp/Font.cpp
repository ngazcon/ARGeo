#include "Font.h"
#include "ColorHelper.h"

namespace argeo
{
	Font::Font(
		std::string name,
		int size)
		: m_name(name)
		, m_size(size)
		, m_color(ColorHelper::white_color())
		, m_style(FontStyle::Normal)
	{ }


	Font::~Font()
	{ }

	Color Font::get_color() const
	{
		return m_color;
	}

	FontStyle Font::get_style() const
	{
		return m_style;
	}

	std::string Font::get_name() const
	{
		return m_name;
	}

	float Font::get_size() const
	{
		return m_size;
	}

}


