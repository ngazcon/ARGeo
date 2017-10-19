#pragma once

#include "Texture.h"
#include "TextureSampler.h"

namespace argeo
{
	class TextureUnit
	{
	public:

		virtual int get_unit_index() const = 0;

		virtual Texture* get_texture() const = 0;
		virtual void set_texture(Texture* value) = 0;

		virtual TextureSampler* get_texture_sampler() const = 0;
		virtual void set_texture_sampler(TextureSampler* value) = 0;
	};
}
