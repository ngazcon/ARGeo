#pragma once

#include "TextureMagnificationFilter.h"
#include "TextureMinificationFilter.h"
#include "TextureWrap.h"

namespace argeo
{
	class TextureSampler
	{
	protected:

		TextureSampler(
			TextureMinificationFilter minification_filter,
			TextureMagnificationFilter magnification_filter,
			TextureWrap wrap_s,
			TextureWrap wrap_t,
			float maximum_anistropy)
			: m_minification_filter(minification_filter)
			, m_magnification_filter(magnification_filter)
			, m_wrap_s(wrap_s)
			, m_wrap_t(wrap_t)
			, m_maximum_anistropy(maximum_anistropy)
		{ }

	public:

		TextureMinificationFilter get_minification_filter() const
		{
			return m_minification_filter;
		}

		TextureMagnificationFilter get_magnification_filter() const
		{
			return m_magnification_filter;
		}

		TextureWrap get_wrap_s() const
		{
			return m_wrap_s;
		}

		TextureWrap get_wrap_t() const
		{
			return m_wrap_t;
		}

		float get_maximum_anistropy() const
		{
			return m_maximum_anistropy;
		}

	private:

		const TextureMinificationFilter  m_minification_filter;
		const TextureMagnificationFilter m_magnification_filter;
		const TextureWrap m_wrap_s;
		const TextureWrap m_wrap_t;
		const float m_maximum_anistropy;

	};
}
