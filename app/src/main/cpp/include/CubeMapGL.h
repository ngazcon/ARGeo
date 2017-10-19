#pragma once

#include "CubeMap.h"
#include "CubeMapFaceGL.h"
#include "TextureGL.h"
#include "TextureDescription.h"

#include <memory>

namespace argeo
{
	class CubeMapGL : 
		public CubeMap,
		public TextureGL
	{
	public:
		CubeMapGL(
			const TextureDescription& description,
			const bool& generate_mipmaps,
			TextureSampler* sampler);

		~CubeMapGL();

		CubeMapFace* get_positive_x();
		CubeMapFace* get_negative_x();
		CubeMapFace* get_positive_y();
		CubeMapFace* get_negative_y();
		CubeMapFace* get_positive_z();
		CubeMapFace* get_negative_z();

		void bind() const;
		unsigned int get_handle() const;

		void set_sampler(TextureSampler* value);
		TextureSampler* get_sampler();
		TextureDescription get_description() const;
		TextureTarget get_target() const;

	private:
		void generate_mipmap();

	private:
		std::unique_ptr<CubeMapFaceGL> m_negative_x;
		std::unique_ptr<CubeMapFaceGL> m_positive_x;
		std::unique_ptr<CubeMapFaceGL> m_negative_y;
		std::unique_ptr<CubeMapFaceGL> m_positive_y;
		std::unique_ptr<CubeMapFaceGL> m_negative_z;
		std::unique_ptr<CubeMapFaceGL> m_positive_z;

		unsigned int m_handler;
		TextureTarget m_texture_target;
		TextureDescription m_description;
		TextureSampler* m_sampler;
	};
}
