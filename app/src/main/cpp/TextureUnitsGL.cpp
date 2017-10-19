#include "TextureUnitsGL.h"
#include "TypeConverterGL.h"
#include "TextureGL.h"
#include "GL.h"

namespace argeo
{
	TextureUnitsGL::TextureUnitsGL()
	{
		// Device::number_of_texture_units() is not initialized yet.
		glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &m_maximum_texture_units);

		m_texture_units.resize(m_maximum_texture_units);

		for (int i = 0; i < m_maximum_texture_units; ++i)
		{
			m_texture_units[i] = std::unique_ptr<TextureUnit>(new TextureUnitGL(i, this));
		}

		m_last_texture_unit = static_cast<TextureUnitGL*>(m_texture_units[m_maximum_texture_units - 1].get());
	}


	TextureUnitsGL::~TextureUnitsGL()
	{ }

	void TextureUnitsGL::unbind(unsigned int max_texture_unit_index)
	{
		for (int i = 0; i < max_texture_unit_index; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			TextureGL::unbind(TextureTarget::Texture2D);
			TextureGL::unbind(TextureTarget::TextureExternal);
			TextureGL::unbind(TextureTarget::Texture2DArray);
			TextureSamplerGL::unbind(i);
		}
	}

	TextureUnit* TextureUnitsGL::get_texture_unit(int index)
	{
		return m_texture_units[index].get();
	}

	void TextureUnitsGL::set_texture_unit(int index, std::unique_ptr<TextureUnit> value)
	{
		m_texture_units[index] = std::move(value);
	}

	int TextureUnitsGL::get_count() const
	{
		return m_maximum_texture_units;
	}

	void TextureUnitsGL::clean()
	{
		for (int i = 0; i < m_dirty_texture_units.size(); ++i)
		{
			m_dirty_texture_units[i]->clean();
		}

		m_dirty_texture_units.clear();
		m_last_texture_unit->clean_last_texture_unit();
	}

	void TextureUnitsGL::notify_dirty(ICleanable* value)
	{
		m_dirty_texture_units.push_back(value);
	}
}