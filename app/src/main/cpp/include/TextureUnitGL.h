#pragma once

#include "TextureUnit.h"
#include "ICleanable.h"
#include "ICleanableObserver.h"
#include "TextureSamplerGL.h"

namespace argeo
{
	class TextureUnitGL : public TextureUnit, public ICleanable
	{
	public:

		enum class DirtyFlags : int
		{
			None		   = 1 << 0,
			Texture		   = 1 << 1,
			TextureSampler = 1 << 2,
			All = Texture | TextureSampler
		};

		TextureUnitGL(int index, ICleanableObserver* observer);
		~TextureUnitGL();

		Texture* get_texture() const;
		void set_texture(Texture * value);

		TextureSampler* get_texture_sampler() const;
		void set_texture_sampler(TextureSampler * value);

		int get_unit_index() const;

		void clean();
		void clean_last_texture_unit();

	private:

		void validate();

		int m_texture_unit_index;
		unsigned int m_texture_unit;

		ICleanableObserver* m_observer;
		Texture * m_texture;
		TextureSamplerGL * m_texture_sampler;

		DirtyFlags m_dirty_flags;
	};

	inline TextureUnitGL::DirtyFlags operator&(TextureUnitGL::DirtyFlags lhs, TextureUnitGL::DirtyFlags rhs)
	{
		return (TextureUnitGL::DirtyFlags)(static_cast<int>(lhs)& static_cast<int>(rhs));
	}

	inline TextureUnitGL::DirtyFlags operator|(TextureUnitGL::DirtyFlags lhs, TextureUnitGL::DirtyFlags rhs)
	{
		return (TextureUnitGL::DirtyFlags)(static_cast<int>(lhs) | static_cast<int>(rhs));
	}

	inline TextureUnitGL::DirtyFlags& operator|=(TextureUnitGL::DirtyFlags& lhs, TextureUnitGL::DirtyFlags rhs)
	{
		lhs = (TextureUnitGL::DirtyFlags)(static_cast<int>(lhs) | static_cast<int>(rhs));
		return lhs;
	}
}

