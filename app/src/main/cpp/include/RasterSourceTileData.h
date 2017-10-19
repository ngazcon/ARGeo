#pragma once

#include "Texture2D.h"

#include <memory>

namespace argeo
{
	struct RasterSourceTileData
	{
		RasterSourceTileData(
			float maximum = 0.0f,
			float minimum = 0.0f,
			std::unique_ptr<Texture2D> texture = nullptr)
			: maximum(maximum)
			, minimum(minimum)
			, texture(std::move(texture))
		{ }

		float maximum;
		float minimum;
		std::unique_ptr<Texture2D> texture;
	};
}
