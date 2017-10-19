#pragma once

#include "Texture.h"

namespace argeo
{
	class ColorAttachments
	{
	public:

		// Set the texture attached for color attachment at index position.
		virtual void set_attachment(int index, const Texture* value) = 0;

		// Get the texture attached to the color attachment at index position.
		virtual Texture const* get_attachment(int index) const = 0;

		// Get the total count of attachments.
		virtual int get_count() const = 0;
	};
}
