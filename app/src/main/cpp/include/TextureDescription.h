#pragma once

#include <stdexcept>
#include "TextureFormat.h"

namespace argeo
{
	struct TextureDescription
	{
	public:

		TextureDescription(
			const unsigned int& width,
			const unsigned int& height,
			const TextureFormat& format,
			const bool& generate_mipmaps = false)
			: m_width(width)
			, m_height(height)
			, m_format(format)
			, m_layers(1)
			, m_generate_mipmaps(generate_mipmaps)
		{ }

		TextureDescription(
			const unsigned int& width,
			const unsigned int& height,
			const TextureFormat& format,
			const unsigned int& layers,
			const bool& generate_mipmaps = false)
			: m_width(width)
			, m_height(height)
			, m_format(format)
			, m_layers(layers)
			, m_generate_mipmaps(generate_mipmaps)
		{ }

		unsigned int get_layers() const
		{
			return m_layers;
		}

		unsigned int get_width() const
		{
			return m_width;
		}

		unsigned int get_size() const
		{
			return m_width * m_height;
		}

		unsigned int get_height() const
		{
			return m_height;
		}

		TextureFormat get_texture_format() const
		{
			return m_format;
		}

		bool get_generate_mipmaps() const
		{
			return m_generate_mipmaps;
		}

		bool get_color_renderable() const
		{
			return !get_depth_renderable() && 
				   !get_depth_stencil_renderable();
		}

		bool get_depth_renderable() const
		{
			return
				m_format == TextureFormat::Depth16 ||
				m_format == TextureFormat::Depth24 ||
				m_format == TextureFormat::Depth32f ||
				m_format == TextureFormat::Depth24Stencil8 ||
				m_format == TextureFormat::Depth32fStencil8;
		}

		bool get_stencil_renderable() const
		{
			return
				m_format == TextureFormat::Depth24Stencil8 ||
				m_format == TextureFormat::Depth32fStencil8;
		}

		bool get_depth_stencil_renderable() const
		{
			return
				m_format == TextureFormat::Depth24Stencil8 ||
				m_format == TextureFormat::Depth32fStencil8;
		}

		bool equals(TextureDescription other) const
		{
			return
				(m_width == other.m_width) &&
				(m_height == other.m_height) &&
				(m_format == other.m_format) &&
				(m_generate_mipmaps == other.m_generate_mipmaps);
		}


		static int size_in_bytes(TextureFormat format)
		{
			switch (format)
			{
			case TextureFormat::RedGreenBlue8:
				return 3;
			case TextureFormat::RedGreenBlue16:
				return 6;
			case TextureFormat::RedGreenBlueAlpha8:
				return 4;
			case TextureFormat::RedGreenBlue10A2:
				return 4;
			case TextureFormat::RedGreenBlueAlpha16:
				return 8;
			case TextureFormat::Depth16:
				return 2;
			case TextureFormat::Depth24:
				return 3;
			case TextureFormat::Red8:
				return 1;
			case TextureFormat::Red16:
				return 2;
			case TextureFormat::RedGreen8:
				return 2;
			case TextureFormat::RedGreen16:
				return 4;
			case TextureFormat::Red16f:
				return 2;
			case TextureFormat::Red32f:
				return 4;
			case TextureFormat::RedGreen16f:
				return 4;
			case TextureFormat::RedGreen32f:
				return 8;
			case TextureFormat::Red8i:
				return 1;
			case TextureFormat::Red8ui:
				return 1;
			case TextureFormat::Red16i:
				return 2;
			case TextureFormat::Red16ui:
				return 2;
			case TextureFormat::Red32i:
				return 4;
			case TextureFormat::Red32ui:
				return 4;
			case TextureFormat::RedGreen8i:
				return 2;
			case TextureFormat::RedGreen8ui:
				return 2;
			case TextureFormat::RedGreen16i:
				return 4;
			case TextureFormat::RedGreen16ui:
				return 4;
			case TextureFormat::RedGreen32i:
				return 8;
			case TextureFormat::RedGreen32ui:
				return 8;
			case TextureFormat::RedGreenBlueAlpha32f:
				return 16;
			case TextureFormat::RedGreenBlue32f:
				return 12;
			case TextureFormat::RedGreenBlueAlpha16f:
				return 8;
			case TextureFormat::RedGreenBlue16f:
				return 6;
			case TextureFormat::Depth24Stencil8:
				return 4;
			case TextureFormat::Red11fGreen11fBlue10f:
				return 4;
			case TextureFormat::RedGreenBlue9E5:
				return 4;
			case TextureFormat::SRedGreenBlue8:
				return 3;
			case TextureFormat::SRedGreenBlue8Alpha8:
				return 4;
			case TextureFormat::Depth32f:
				return 4;
			case TextureFormat::Depth32fStencil8:
				return 5;
			case TextureFormat::RedGreenBlueAlpha32ui:
				return 16;
			case TextureFormat::RedGreenBlue32ui:
				return 12;
			case TextureFormat::RedGreenBlueAlpha16ui:
				return 8;
			case TextureFormat::RedGreenBlue16ui:
				return 6;
			case TextureFormat::RedGreenBlueAlpha8ui:
				return 4;
			case TextureFormat::RedGreenBlue8ui:
				return 3;
			case TextureFormat::RedGreenBlueAlpha32i:
				return 16;
			case TextureFormat::RedGreenBlue32i:
				return 12;
			case TextureFormat::RedGreenBlueAlpha16i:
				return 8;
			case TextureFormat::RedGreenBlue16i:
				return 6;
			case TextureFormat::RedGreenBlueAlpha8i:
				return 4;
			case TextureFormat::RedGreenBlue8i:
				return 3;
			}

			throw std::invalid_argument("format");
		}

	private:
		unsigned int m_width;
		unsigned int m_height;
		unsigned int m_layers;
		TextureFormat m_format;
		bool m_generate_mipmaps;
	};

	inline bool operator==(const TextureDescription& left, const TextureDescription& right)
	{
		return left.equals(right);
	}

	inline bool operator!=(const TextureDescription& left, const TextureDescription& right)
	{
		return !left.equals(right);
	}
}
