#include "TextureUnitGL.h"
#include "TextureGL.h"

#include "GL.h"

namespace argeo
{
	TextureUnitGL::TextureUnitGL(int index, ICleanableObserver* observer)
		: m_texture_unit_index(index)
		, m_texture_unit(GL_TEXTURE0 + index)
		, m_texture_sampler(nullptr)
		, m_texture(nullptr)
		, m_observer(observer)
		, m_dirty_flags(DirtyFlags::None)
	{ }

	TextureUnitGL::~TextureUnitGL()
	{ }

	Texture* TextureUnitGL::get_texture() const
	{
		return reinterpret_cast<Texture*>(m_texture);
	}

	void TextureUnitGL::set_texture(Texture* value)
	{
		if (m_texture != value)
		{
			if (m_dirty_flags == DirtyFlags::None)
			{
				m_observer->notify_dirty(this);
			}

			m_dirty_flags |= DirtyFlags::Texture;
			m_texture = value;
		}
	}

	TextureSampler* TextureUnitGL::get_texture_sampler() const
	{
		return m_texture_sampler;
	}

	void TextureUnitGL::set_texture_sampler(TextureSampler* value)
	{
		TextureSamplerGL* sampler = static_cast<TextureSamplerGL *>(value);

		if (m_texture_sampler != sampler)
		{
			if (m_dirty_flags == DirtyFlags::None)
			{
				m_observer->notify_dirty(this);
			}

			m_dirty_flags |= DirtyFlags::TextureSampler;
			m_texture_sampler = sampler;
		}
	}

	int TextureUnitGL::get_unit_index() const
	{
		return m_texture_unit_index;
	}

	void TextureUnitGL::clean()
	{
		if (m_dirty_flags != DirtyFlags::None)
		{
			validate();

			glActiveTexture(m_texture_unit);

			if ((m_dirty_flags & DirtyFlags::Texture) == DirtyFlags::Texture)
			{
				if (m_texture != nullptr)
				{
					TextureGL const * texture = dynamic_cast<TextureGL const *>(m_texture);
					texture->bind();
				}
				else
				{
					TextureGL::unbind(TextureTarget::Texture2D);
					TextureGL::unbind(TextureTarget::TextureExternal);
					TextureGL::unbind(TextureTarget::Texture2DArray);
				}
			}

			if ((m_dirty_flags & DirtyFlags::TextureSampler) == DirtyFlags::TextureSampler)
			{
				if (m_texture_sampler != nullptr)
				{
					m_texture_sampler->bind(m_texture_unit_index);
				}
				else
				{
					TextureSamplerGL::unbind(m_texture_unit_index);
				}
			}

			m_dirty_flags = DirtyFlags::None;
		}
	}

	void TextureUnitGL::clean_last_texture_unit()
	{
		// If the last texture unit has a texture attached, it
		// is cleaned even if it isn't dirty because the last
		// texture unit is used for texture uploads and downloads in
		// Texture2DGL, the texture unit could be dirty without
		// knowing it.
		if (m_texture != nullptr)
		{
			m_dirty_flags |= DirtyFlags::Texture;
		}

		clean();
	}

	void TextureUnitGL::validate()
	{
		if (m_texture != nullptr)
		{
			if (m_texture_sampler == nullptr)
			{
				throw std::invalid_argument("A texture sampler must be assigned to a texture unit with one or more bound textures.");
			}

			if (m_texture->get_target() == TextureTarget::TextureRectangle)
			{
				if (m_texture_sampler->get_minification_filter() != TextureMinificationFilter::Linear &&
					m_texture_sampler->get_minification_filter() != TextureMinificationFilter::Nearest)
				{
					throw std::invalid_argument("The texture sampler is incompatible with the rectangle texture bound to the same texture unit.  Rectangle textures only support linear and nearest minification filters.");
				}

				if (m_texture_sampler->get_wrap_s() == TextureWrap::Repeat ||
					m_texture_sampler->get_wrap_s() == TextureWrap::MirroredRepeat ||
					m_texture_sampler->get_wrap_t() == TextureWrap::Repeat ||
					m_texture_sampler->get_wrap_t() == TextureWrap::MirroredRepeat)
				{
					throw std::invalid_argument("The texture sampler is incompatible with the rectangle texture bound to the same texture unit.  Rectangle textures do not support repeat or mirrored repeat wrap modes.");
				}
			}
		}
	}
}