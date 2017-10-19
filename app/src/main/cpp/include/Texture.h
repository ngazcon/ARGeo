#pragma once

#include "TextureDescription.h"
#include "TextureTarget.h"
#include "TextureSampler.h"

namespace argeo
{
	class Texture
	{
	public:
		virtual TextureSampler* get_sampler() = 0;
		virtual TextureDescription get_description() const = 0;
		virtual TextureTarget get_target() const = 0;
	};
}
