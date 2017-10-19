#include "CubeMapGL.h"
#include "Device.h"
#include "GL.h"

namespace argeo
{
	CubeMapGL::CubeMapGL(
		const TextureDescription& description,
		const bool& generate_mipmaps,
		TextureSampler* sampler)
		: m_description(description)
		, m_sampler(sampler)
	{
		if (description.get_width() != description.get_height())
		{
			throw std::invalid_argument("Width must equal height.");
		}

		if (description.get_depth_renderable())
		{
			throw std::invalid_argument("format cannot be DEPTH_COMPONENT or DEPTH_STENCIL.");
		}

		std::size_t size = description.get_width();
		m_texture_target = TextureTarget::TextureCubeMap;
		// Create the texture in device memory.
		glGenTextures(1, &m_handler);

		TextureFormat format = description.get_texture_format();
		unsigned int texture_format  = TypeConverterGL::ToEnum(format);
		unsigned int pixel_format    = TypeConverterGL::TextureToPixelFormat(format);
		unsigned int pixel_type      = TypeConverterGL::TextureToPixelType(format);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(TypeConverterGL::ToEnum(m_texture_target), m_handler);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, texture_format, size, size, 0, pixel_format, pixel_type, nullptr);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, texture_format, size, size, 0, pixel_format, pixel_type, nullptr);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, texture_format, size, size, 0, pixel_format, pixel_type, nullptr);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, texture_format, size, size, 0, pixel_format, pixel_type, nullptr);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, texture_format, size, size, 0, pixel_format, pixel_type, nullptr);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, texture_format, size, size, 0, pixel_format, pixel_type, nullptr);
		glBindTexture(TypeConverterGL::ToEnum(m_texture_target), 0);

		m_positive_x = std::unique_ptr<CubeMapFaceGL>(new CubeMapFaceGL(this, CubeMapFaceTarget::PositiveX));
		m_negative_x = std::unique_ptr<CubeMapFaceGL>(new CubeMapFaceGL(this, CubeMapFaceTarget::NegativeX));
		m_positive_y = std::unique_ptr<CubeMapFaceGL>(new CubeMapFaceGL(this, CubeMapFaceTarget::PositiveY));
		m_negative_y = std::unique_ptr<CubeMapFaceGL>(new CubeMapFaceGL(this, CubeMapFaceTarget::NegativeY));
		m_positive_z = std::unique_ptr<CubeMapFaceGL>(new CubeMapFaceGL(this, CubeMapFaceTarget::PositiveZ));
		m_negative_z = std::unique_ptr<CubeMapFaceGL>(new CubeMapFaceGL(this, CubeMapFaceTarget::NegativeZ));

		// Default sampler, compatiable when attaching a non-mimapped 
		// texture to a frame buffer object.
		set_sampler(sampler != nullptr ?
			sampler :
			Device::get_texture_samplers()
			->get_linear_clamp());
	}


	CubeMapGL::~CubeMapGL()
	{
		glDeleteTextures(1, &m_handler);
	}

	void CubeMapGL::generate_mipmap()
	{
		// TODO: Add mipmap hint
		//glHint(GL_GENERATE_MIPMAP_HINT, );
		const unsigned int target = TypeConverterGL::ToEnum(m_texture_target);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(TypeConverterGL::ToEnum(m_texture_target),m_handler);
		glGenerateMipmap(target);
		glBindTexture(target, 0);
	}

	void CubeMapGL::bind() const
	{
		glBindTexture(
			TypeConverterGL::ToEnum(m_texture_target),
			m_handler);
	}

	unsigned int CubeMapGL::get_handle() const
	{
		return m_handler;
	}

	CubeMapFace* CubeMapGL::get_positive_x()
	{
		return m_positive_x.get();
	}

	CubeMapFace* CubeMapGL::get_negative_x()
	{
		return m_negative_x.get();
	}

	CubeMapFace* CubeMapGL::get_positive_y()
	{
		return m_positive_y.get();
	}

	CubeMapFace* CubeMapGL::get_negative_y()
	{
		return m_negative_y.get();
	}

	CubeMapFace* CubeMapGL::get_positive_z()
	{
		return m_positive_z.get();
	}

	CubeMapFace* CubeMapGL::get_negative_z()
	{
		return m_negative_z.get();
	}

	void CubeMapGL::set_sampler(TextureSampler* value)
	{
		TextureMinificationFilter  minification_filter  = value->get_minification_filter();
		TextureMagnificationFilter magnification_filter = value->get_magnification_filter();

		bool mipmap =
			(minification_filter == TextureMinificationFilter::NearestMipmapNearest) ||
			(minification_filter == TextureMinificationFilter::NearestMipmapLinear) ||
			(minification_filter == TextureMinificationFilter::LinearMipmapNearest) ||
			(minification_filter == TextureMinificationFilter::LinearMipmapLinear);

		// float textures only support nearest filtering, so override the sampler's settings
		if (TypeConverterGL::TextureToPixelType(m_description.get_texture_format()) == GL_FLOAT) 
		{
			minification_filter  = mipmap ? TextureMinificationFilter::NearestMipmapNearest : TextureMinificationFilter::Nearest;
			magnification_filter = TextureMagnificationFilter::Nearest;
		}

		const unsigned int target = TypeConverterGL::ToEnum(m_texture_target);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(target, m_handler);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, TypeConverterGL::ToEnum(minification_filter));
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, TypeConverterGL::ToEnum(magnification_filter));
		glTexParameteri(target, GL_TEXTURE_WRAP_S, TypeConverterGL::ToEnum(value->get_wrap_s()));
		glTexParameteri(target, GL_TEXTURE_WRAP_T, TypeConverterGL::ToEnum(value->get_wrap_t()));
		
		//if (defined(this._textureFilterAnisotropic)) 
		//{
		//	gl.texParameteri(target, this._textureFilterAnisotropic.TEXTURE_MAX_ANISOTROPY_EXT, sampler.maximumAnisotropy);
		//}

		glBindTexture(target, 0);

		m_sampler = value;
	}

	TextureSampler* CubeMapGL::get_sampler()
	{
		return m_sampler;
	}

	TextureDescription CubeMapGL::get_description() const
	{
		return m_description;
	}

	TextureTarget CubeMapGL::get_target() const
	{
		return m_texture_target;
	}
}