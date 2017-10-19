#pragma once

#include "ColorAttachments.h"
#include "Texture2D.h"

namespace argeo
{
	struct ColorAttachmentGL
	{
		ColorAttachmentGL()
			: texture(nullptr)
			, dirty(false)
		{ }

		const Texture * texture;
		bool dirty;
	};

	class ColorAttachmentsGL : public ColorAttachments
	{
	public:
		ColorAttachmentsGL();
		~ColorAttachmentsGL();

		void set_attachment(int index, const Texture* value);
		const Texture * get_attachment(int index) const;

		std::vector<ColorAttachmentGL>& get_attachments();

		int get_maximum_attachments() const;
		
		int get_count() const;

		bool get_dirty() const;
		void set_dirty(bool value);

	private:
		std::vector<ColorAttachmentGL> m_attachments;
		int m_maximum_attachments;
		int m_count;
		bool m_dirty;
	};
}

