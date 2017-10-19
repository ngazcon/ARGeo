#include "TextureSamplers.h"
#include "Device.h"

namespace argeo
{
	TextureSamplers::TextureSamplers()
	{
		m_nearest_clamp = Device::create_texture2D_sampler(
			TextureMinificationFilter::Nearest,
			TextureMagnificationFilter::Nearest,
			TextureWrap::Clamp,
			TextureWrap::Clamp);

		m_linear_clamp = Device::create_texture2D_sampler(
			TextureMinificationFilter::Linear,
			TextureMagnificationFilter::Linear,
			TextureWrap::Clamp,
			TextureWrap::Clamp);

		m_nearest_mipmap_clamp = Device::create_texture2D_sampler(
			TextureMinificationFilter::NearestMipmapNearest,
			TextureMagnificationFilter::Nearest,
			TextureWrap::Clamp,
			TextureWrap::Clamp);

		m_linear_mipmap_clamp = Device::create_texture2D_sampler(
			TextureMinificationFilter::LinearMipmapLinear,
			TextureMagnificationFilter::Linear,
			TextureWrap::Clamp,
			TextureWrap::Clamp);

		m_nearest_repeat = Device::create_texture2D_sampler(
			TextureMinificationFilter::Nearest,
			TextureMagnificationFilter::Nearest,
			TextureWrap::Repeat,
			TextureWrap::Repeat);

		m_linear_repeat = Device::create_texture2D_sampler(
			TextureMinificationFilter::Linear,
			TextureMagnificationFilter::Linear,
			TextureWrap::Repeat,
			TextureWrap::Repeat);

		m_nearest_mipmap_repeat = Device::create_texture2D_sampler(
			TextureMinificationFilter::NearestMipmapNearest,
			TextureMagnificationFilter::Nearest,
			TextureWrap::Repeat,
			TextureWrap::Repeat);

		m_linear_mipmap_repeat = Device::create_texture2D_sampler(
			TextureMinificationFilter::LinearMipmapLinear,
			TextureMagnificationFilter::Linear,
			TextureWrap::Repeat,
			TextureWrap::Repeat);
	}


	TextureSamplers::~TextureSamplers()
	{ }

	TextureSampler* TextureSamplers::get_nearest_clamp() const
	{
		return m_nearest_clamp.get();
	}

	TextureSampler* TextureSamplers::get_linear_clamp() const
	{
		return m_linear_clamp.get();
	}

	TextureSampler* TextureSamplers::get_nearest_mipmap_clamp() const
	{
		return m_nearest_mipmap_clamp.get();
	}

	TextureSampler* TextureSamplers::get_linear_mipmap_clamp() const
	{
		return m_linear_mipmap_clamp.get();
	}

	TextureSampler* TextureSamplers::get_nearest_repeat() const
	{
		return m_nearest_repeat.get();
	}

	TextureSampler* TextureSamplers::get_linear_repeat() const
	{
		return m_linear_repeat.get();
	}

	TextureSampler* TextureSamplers::get_nearest_mipmap_repeat() const
	{
		return m_nearest_mipmap_repeat.get();
	}

	TextureSampler* TextureSamplers::get_linear_mipmap_repeat() const
	{
		return m_linear_mipmap_repeat.get();
	}
}