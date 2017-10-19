#include "ColorAttachmentsGL.h"
#include "Device.h"

#include "GL.h"

namespace argeo
{
	ColorAttachmentsGL::ColorAttachmentsGL()
		: m_maximum_attachments(Device::get_maximum_number_of_color_attachments())
		, m_dirty(false)
		, m_count(0)
	{
		m_attachments.resize(m_maximum_attachments);
	}


	ColorAttachmentsGL::~ColorAttachmentsGL()
	{ }

	int ColorAttachmentsGL::get_count() const
	{
		return m_count;
	}

	void ColorAttachmentsGL::set_attachment(int index, const Texture* value)
	{
		if (value != nullptr && 
			!value->get_description().get_color_renderable())
		{
			throw std::invalid_argument(
				"Texture must be color renderable but the Description.ColorRenderable property is false.");
		}

		if (m_attachments[index].texture != value)
		{
			if ((m_attachments[index].texture != nullptr) && (value == nullptr))
			{
				--m_count;
			}
			else if ((m_attachments[index].texture == nullptr) && (value != nullptr))
			{
				++m_count;
			}

			m_attachments[index].texture = value;
			m_attachments[index].dirty = true;
			m_dirty = true;
		}
	}

	const Texture * ColorAttachmentsGL::get_attachment(int index) const
	{
		return m_attachments[index].texture;
	}

	std::vector<ColorAttachmentGL>& ColorAttachmentsGL::get_attachments()
	{
		return m_attachments;
	}

	int ColorAttachmentsGL::get_maximum_attachments() const
	{
		return m_maximum_attachments;
	}

	bool ColorAttachmentsGL::get_dirty() const
	{
		return m_dirty;
	}

	void ColorAttachmentsGL::set_dirty(bool value)
	{
		m_dirty = value;
	}

}