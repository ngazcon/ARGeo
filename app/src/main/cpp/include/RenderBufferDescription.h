#pragma once

#include <stdexcept>

#include "RenderBufferFormat.h"

namespace argeo
{
	class RenderBufferDescription
	{
	public:

		RenderBufferDescription(
			int width,
			int height,
			RenderBufferFormat format)
			: m_width(width)
			, m_height(height)
			, m_format(format)
		{ }

		int get_width() const
		{
			return m_width;
		}

		int get_size() const
		{
			return m_width * m_height;
		}

		int get_height() const
		{
			return m_height;
		}

		RenderBufferFormat get_render_buffer_format() const
		{
			return m_format;
		}

		bool get_color_renderable() const
		{
			return !get_depth_renderable() &&
				!get_depth_stencil_renderable();
		}

		bool get_stencil_renderable() const
		{
			return m_format == RenderBufferFormat::Stencil ||
				m_format == RenderBufferFormat::DepthStencil;
		}

		bool get_depth_renderable() const
		{
			return
				m_format == RenderBufferFormat::DepthComponent16 ||
				m_format == RenderBufferFormat::DepthStencil;
		}

		bool get_depth_stencil_renderable() const
		{
			return
				m_format == RenderBufferFormat::DepthStencil;
		}

		bool equals(RenderBufferDescription other) const
		{
			return
				(m_width == other.m_width) &&
				(m_height == other.m_height) &&
				(m_format == other.m_format);
		}


		static int size_in_bytes(RenderBufferFormat format)
		{
			switch (format)
			{
			case RenderBufferFormat::DepthComponent16:
				return 4;
			case RenderBufferFormat::DepthStencil:
				return 8;
			}

			throw std::invalid_argument("format");
		}

	private:
		int m_width;
		int m_height;
		RenderBufferFormat m_format;
	};

	inline bool operator==(const RenderBufferDescription& left, const RenderBufferDescription& right)
	{
		return left.equals(right);
	}

	inline bool operator!=(const RenderBufferDescription& left, const RenderBufferDescription& right)
	{
		return !left.equals(right);
	}
}
