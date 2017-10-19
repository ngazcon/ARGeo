#pragma once

#include "ImageFormat.h"
#include "ImageDataType.h"
#include "Color.h"

namespace argeo
{
	struct BlittableRGBA
	{
		BlittableRGBA()
			: red(0),
			  green(0),
			  blue(0),
			  alpha(0)
		{ }

		BlittableRGBA(Color color)
			: red(color.red),
			  green(color.green),
			  blue(color.blue),
			  alpha(color.alpha)
		{ }

		BlittableRGBA(
			unsigned char r,
			unsigned char g, 
			unsigned char b, 
			unsigned char a)
			: red(r),
			  green(g),
			  blue(b),
			  alpha(a)
		{ }

		static ImageFormat format()
		{
			return ImageFormat::RedGreenBlueAlpha;
		}

		static ImageDataType get_type()
		{
			return ImageDataType::UnsignedByte;
		}

		bool operator==(BlittableRGBA right)
		{
			return equals(right);
		}

		bool operator!=(BlittableRGBA right)
		{
			return !equals(right);
		}

		bool equals(BlittableRGBA other)
		{
			return
				(red == other.red) &&
				(green == other.green) &&
				(blue == other.blue) &&
				(alpha == other.alpha);
		}

		union
		{
			struct
			{
				unsigned char red;
				unsigned char green;
				unsigned char blue;
				unsigned char alpha;
			};
			unsigned char data[4];
		};

		

	};
}
