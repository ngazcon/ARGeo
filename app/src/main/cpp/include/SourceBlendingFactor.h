#pragma once

namespace argeo
{
	enum class SourceBlendingFactor : unsigned int
	{
		Zero						= 0,
		One						    = 1,
		SourceAlpha				    = 2,
		OneMinusSourceAlpha		    = 3,
		DestinationAlpha			= 4,
		OneMinusDestinationAlpha	= 5,
		DestinationColor			= 6,
		OneMinusDestinationColor	= 7,
		SourceAlphaSaturate		    = 8,
		ConstantColor				= 9,
		OneMinusConstantColor		= 10,
		ConstantAlpha				= 11,
		OneMinusConstantAlpha		= 12,
	};					
}