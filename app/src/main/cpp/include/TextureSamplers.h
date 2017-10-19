#pragma once

#include "TextureSampler.h"

#include <memory>

namespace argeo
{
	class TextureSamplers
	{
	public:

		TextureSamplers();

		~TextureSamplers();
		
		TextureSampler* get_nearest_clamp() const;
		TextureSampler* get_linear_clamp() const;
		TextureSampler* get_nearest_mipmap_clamp() const;
		TextureSampler* get_linear_mipmap_clamp() const;
		TextureSampler* get_nearest_repeat() const;
		TextureSampler* get_linear_repeat() const;
		TextureSampler* get_nearest_mipmap_repeat() const;
		TextureSampler* get_linear_mipmap_repeat() const;

	private:

		std::unique_ptr<TextureSampler> m_nearest_clamp;
		std::unique_ptr<TextureSampler> m_linear_clamp;
		std::unique_ptr<TextureSampler> m_nearest_mipmap_clamp;
		std::unique_ptr<TextureSampler> m_linear_mipmap_clamp;
		std::unique_ptr<TextureSampler> m_nearest_repeat;
		std::unique_ptr<TextureSampler> m_linear_repeat;
		std::unique_ptr<TextureSampler> m_nearest_mipmap_repeat;
		std::unique_ptr<TextureSampler> m_linear_mipmap_repeat;
	};
}