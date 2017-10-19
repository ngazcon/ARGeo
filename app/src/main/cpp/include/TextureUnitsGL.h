#pragma once

#include "TextureUnits.h"
#include "TextureUnitGL.h"
#include "ICleanableObserver.h"

#include <vector>
#include <memory>

namespace argeo
{
	class TextureUnitsGL : public TextureUnits, public ICleanableObserver
	{
	public:
		TextureUnitsGL();
		~TextureUnitsGL();

		TextureUnit* get_texture_unit(int index);
		void set_texture_unit(int index, std::unique_ptr<TextureUnit> value);

		int get_count() const;

		static void unbind(unsigned int max_texture_unit_index);

		void clean();
		void notify_dirty(ICleanable* value);

	private:
		
		std::vector<std::unique_ptr<TextureUnit>> m_texture_units;

		// Maximum number of posible texture units.
		int m_maximum_texture_units;

		std::vector<ICleanable*> m_dirty_texture_units;

		// The last texture unit of the device.
		TextureUnitGL* m_last_texture_unit;

	};
}
