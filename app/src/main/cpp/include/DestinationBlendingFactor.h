#pragma once

namespace argeo
{
	enum class DestinationBlendingFactor : unsigned int
	{
		Zero					 = 0,
		One						 = 1,
		SourceColor				 = 2,
		OneMinusSourceColor		 = 3,
		SourceAlpha				 = 4,
		OneMinusSourceAlpha		 = 5,
		DestinationAlpha		 = 6,
		OneMinusDestinationAlpha = 7,
		DestinationColor		 = 8,
		OneMinusDestinationColor = 9,
		ConstantColor			 = 10,
		OneMinusConstantColor    = 11,
		ConstantAlpha			 = 12,
		OneMinusConstantAlpha	 = 13,
	};
}
