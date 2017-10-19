#pragma once

namespace argeo
{
	struct Size
	{
		///
		/// Represents a size in 2D.
		/// The size has width and height.
		///

		///
		/// Construct a size with the given width and height.
		/// @constructor
		/// @param {unsigned int} width The width.
		/// @param {unsigned int} height The height.
		///
		Size(
			const unsigned int& width, 
			const unsigned int& height)
			: width(width)
			, height(height)
		{ }

		unsigned int width;
		unsigned int height;
	};
}
