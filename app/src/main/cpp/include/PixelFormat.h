#pragma once

namespace argeo
{
	///
	/// The internal pixel format of a bitmap.
	///
	enum class PixelFormat : unsigned int
	{
		Alpha					= 0,
		Canonical				= 1,
		DontCare				= 2,
		Extended				= 3,
		Format16bppArgb1555		= 4,
		Format16bppGrayScale	= 5,
		Format16bppRgb555		= 6,
		Format16bppRgb565		= 7,
		Format1bppIndexed		= 8,
		Format24bppRgb			= 9,
		Format32bppArgb			= 10,
		Format32bppPArgb		= 11,
		Format32bppRgb			= 12,
		Format48bppRgb			= 13,
		Format4bppIndexed		= 14,
		Format64bppArgb			= 15,
		Format64bppPArgb		= 16,
		Format8bppIndexed		= 17,
		Gdi						= 18,
		Indexed					= 19,
		PAlpha					= 20,
		Undefined				= 21,
	};
}
