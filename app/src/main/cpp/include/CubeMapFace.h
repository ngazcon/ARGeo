#pragma once

#include "CubeMapFaceTarget.h"
#include "Texture.h"

namespace argeo
{
	class CubeMapFace 
		: public Texture
	{
	public:
		virtual CubeMapFaceTarget get_target_face() const = 0;
		virtual TextureSampler* get_sampler() = 0;
		virtual TextureDescription get_description() const = 0;
		virtual TextureTarget get_target() const = 0;
	};
}
