#include "CubeMapFaceGL.h"
#include "CubeMapGL.h"
#include "GL.h"

namespace argeo
{
	CubeMapFaceGL::CubeMapFaceGL(
		CubeMapGL* cube_map,
		const CubeMapFaceTarget& target_face)
		: m_cube_map(cube_map)
		, m_target_face(target_face)
	{ }

	CubeMapFaceGL::~CubeMapFaceGL()
	{ }

	TextureSampler* CubeMapFaceGL::get_sampler()
	{
		return m_cube_map->get_sampler();
	}

	TextureDescription CubeMapFaceGL::get_description() const
	{
		return m_cube_map->get_description();
	}

	CubeMapFaceTarget CubeMapFaceGL::get_target_face() const
	{
		return m_target_face;
	}

	TextureTarget CubeMapFaceGL::get_target() const
	{
		return m_cube_map->get_target();
	}

	void CubeMapFaceGL::bind() const
	{
		glBindTexture(
			TypeConverterGL::ToEnum(m_cube_map->get_target()),
			m_cube_map->get_handle());
	}

	unsigned int CubeMapFaceGL::get_handle() const
	{
		return m_cube_map->get_handle();
	}
}