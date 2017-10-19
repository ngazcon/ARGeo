#include "BillboardGraphics.h"

namespace argeo
{
	BillboardGraphics::BillboardGraphics(
		const std::string& image_path,
		const std::string& image_id,
		const unsigned int& width,
		const unsigned int& height,
	    const HeightClamping& height_clamping)
		: m_image_id(image_id)
		, m_show(true)
		, m_width(width)
		, m_height(height)
		, m_image_path(image_path)
		, m_image(nullptr)
		, m_height_clamping(height_clamping)
	{ }

	BillboardGraphics::BillboardGraphics(
		Bitmap* image,
		const std::string& image_id,
		const unsigned int& width,
		const unsigned int& height,
		const HeightClamping& height_clamping)
		: m_image_id(image_id)
		, m_image(image)
		, m_show(true)
		, m_width(width)
		, m_height(height)
		, m_image_path(std::string())
		, m_height_clamping(height_clamping)
	{ }


	BillboardGraphics::~BillboardGraphics()
	{ }

	std::string BillboardGraphics::get_image_id() const
	{
		return m_image_id;
	}

    HeightClamping BillboardGraphics::get_height_clamping() const
    {
        return m_height_clamping;
    }

    void BillboardGraphics::set_height_clamping(const HeightClamping &value)
    {
        if (m_height_clamping != value)
        {
            raise_on_property_changed(
                    "height_clamping",
                    &m_height_clamping,
                    &value);
            m_height_clamping = value;
        }
    }

	std::string BillboardGraphics::get_image_path()
	{
		return m_image_path;
	}

	void BillboardGraphics::set_image_path(const std::string& value)
	{
		if (m_image_path != value)
		{
			raise_on_property_changed(
				"image_path",
				&m_image_path,
				&value);
			m_image_path = value;
		}
	}

	Bitmap* BillboardGraphics::get_image()
	{
		return m_image;
	}

	void BillboardGraphics::set_image(Bitmap* value)
	{
		if (m_image != value)
		{
			raise_on_property_changed(
				"image",
				&m_image,
				&value);
			m_image = value;
		}
	}

	void BillboardGraphics::set_pixel_offset(const vec2i& value)
	{
		if (m_pixel_offset != value)
		{
			raise_on_property_changed(
				"pixel_offset",
				&m_pixel_offset,
				&value);
			m_pixel_offset = value;
		}
	}

	vec2i BillboardGraphics::get_pixel_offset() const
	{
		return m_pixel_offset;
	}


	bool BillboardGraphics::get_show() const
	{
		return m_show;
	}

	void BillboardGraphics::set_show(const bool& value)
	{
		if (m_show != value)
		{
			raise_on_property_changed(
				"show",
				&m_show,
				&value);
			m_show = value;
		}
	}

	void BillboardGraphics::set_height(const unsigned int& value)
	{
		if (m_height != value)
		{
			raise_on_property_changed(
				"height",
				&m_height,
				&value);
			m_height = value;
		}
	}

	unsigned int BillboardGraphics::get_height() const
	{
		return m_height;
	}

	void BillboardGraphics::set_width(const unsigned int& value)
	{
		if (m_width != value)
		{
			raise_on_property_changed(
				"width",
				&m_width,
				&value);
			m_width = value;
		}
	}

	unsigned int BillboardGraphics::get_width() const
	{
		return m_width;
	}

}