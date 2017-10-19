#pragma once

#include "Texture.h"
#include "TextureFormat.h"
#include "CubeMapFace.h"

namespace argeo
{
	class CubeMap : public Texture
	{
	public:

		virtual void set_sampler(TextureSampler* value) = 0;
		virtual TextureSampler* get_sampler() = 0;

		virtual TextureTarget get_target() const = 0;
		virtual TextureDescription get_description() const = 0;

		virtual CubeMapFace* get_positive_x() = 0;
		virtual CubeMapFace* get_negative_x() = 0;
		virtual CubeMapFace* get_positive_y() = 0;
		virtual CubeMapFace* get_negative_y() = 0;
		virtual CubeMapFace* get_positive_z() = 0;
		virtual CubeMapFace* get_negative_z() = 0;
	};
}
