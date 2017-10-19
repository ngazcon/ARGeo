#pragma once

#include "ColorHelper.h"
#include "SourceBlendingFactor.h"
#include "DestinationBlendingFactor.h"
#include "BlendEquation.h"

namespace argeo
{
	class Blending
	{
	public:

		Blending(
			bool enabled									   = false,
			SourceBlendingFactor source_rgb_factor			   = SourceBlendingFactor::SourceAlpha,
			SourceBlendingFactor source_alpha_factor		   = SourceBlendingFactor::SourceAlpha,
			DestinationBlendingFactor destination_rgb_factor   = DestinationBlendingFactor::OneMinusSourceAlpha,
			DestinationBlendingFactor destination_alpha_factor = DestinationBlendingFactor::OneMinusSourceAlpha,
			BlendEquation rgb_equation						   = BlendEquation::Add,
			BlendEquation alpha_equation					   = BlendEquation::Add,
			Color color										   = ColorHelper::white_color())
			: enabled(enabled)
			, source_rgb_factor(source_rgb_factor)
			, source_alpha_factor(source_alpha_factor)
			, destination_rgb_factor(destination_rgb_factor)
			, destination_alpha_factor(destination_alpha_factor)
			, rgb_equation(rgb_equation)
			, alpha_equation(alpha_equation)
			, color(color)
		{ }


		bool enabled;
		SourceBlendingFactor source_rgb_factor;
		SourceBlendingFactor source_alpha_factor;
		DestinationBlendingFactor destination_rgb_factor;
		DestinationBlendingFactor destination_alpha_factor;
		BlendEquation rgb_equation;
		BlendEquation alpha_equation;
		Color color;

		bool equals(Blending other) const
		{
			return
				(enabled == other.enabled) &&
				(source_rgb_factor == other.source_rgb_factor) &&
				(source_alpha_factor == other.source_alpha_factor) &&
				(destination_rgb_factor == other.destination_rgb_factor) &&
				(destination_alpha_factor == other.destination_alpha_factor) &&
				(rgb_equation == other.rgb_equation) &&
				(alpha_equation == other.alpha_equation) &&
				(color == other.color);
		}
	};


	inline bool operator==(const Blending& left, const Blending& right)
	{
		return left.equals(right);
	}

	inline bool operator!=(const Blending& left, const Blending& right)
	{
		return !left.equals(right);
	}
}