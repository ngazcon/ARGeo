#pragma once

#include "CubeMapFace.h"
#include "TextureGL.h"

namespace argeo
{
	class CubeMapGL;

	class CubeMapFaceGL :
		public CubeMapFace,
		public TextureGL
	{
	public:
		CubeMapFaceGL(
			CubeMapGL* cube_map,
			const CubeMapFaceTarget& target_face);

		~CubeMapFaceGL();

		void bind() const;
		unsigned int get_handle() const;

		TextureDescription get_description() const;
		TextureSampler* get_sampler();
		TextureTarget get_target() const;
		CubeMapFaceTarget get_target_face() const;

	private:
		CubeMapGL* m_cube_map;
		CubeMapFaceTarget m_target_face;
	};
}
