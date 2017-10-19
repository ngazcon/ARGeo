#include "TextureSamplerGL.h"
#include "TypeConverterGL.h"

#include "GL.h"

namespace argeo
{
	TextureSamplerGL::TextureSamplerGL(
		TextureMinificationFilter minification_filter,
		TextureMagnificationFilter magnification_filter,
		TextureWrap wrap_s,
		TextureWrap wrap_t,
		float maximum_anistropy)
		: TextureSampler(
			minification_filter
			, magnification_filter
			, wrap_s
			, wrap_t
			, maximum_anistropy)
	{
		glGenSamplers(1, &m_handle);

		glSamplerParameteri(
			m_handle,
			GL_TEXTURE_MIN_FILTER,
			TypeConverterGL::ToEnum(minification_filter));

		glSamplerParameteri(
			m_handle,
			GL_TEXTURE_MAG_FILTER,
			TypeConverterGL::ToEnum(magnification_filter));

		glSamplerParameteri(
			m_handle,
			GL_TEXTURE_WRAP_S,
			TypeConverterGL::ToEnum(wrap_s));

		glSamplerParameteri(
			m_handle,
			GL_TEXTURE_WRAP_T,
			TypeConverterGL::ToEnum(wrap_t));
	}


	TextureSamplerGL::~TextureSamplerGL()
	{
		glDeleteSamplers(1, &m_handle);
	}

	void TextureSamplerGL::bind(int texture_unit_index) const
	{
		glBindSampler(texture_unit_index, m_handle);
	}

	void TextureSamplerGL::unbind(int texture_unit_index)
	{
		glBindSampler(texture_unit_index, 0);
	}
}