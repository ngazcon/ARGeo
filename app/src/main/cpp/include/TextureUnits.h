#pragma once

#include "TextureUnit.h"

#include <vector>
#include <memory>

namespace argeo
{
	class TextureUnits
	{
	public:

		// Get the texture unit in the given index.
		virtual TextureUnit* get_texture_unit(int index) = 0;

		// Set the texture unit in the given index.
		virtual void set_texture_unit(int index, std::unique_ptr<TextureUnit> value) = 0;

		// Get the actual number of texture units.
		virtual int get_count() const = 0;
	};
}
