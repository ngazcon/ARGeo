#pragma once

#include "TextureSampler.h"

namespace argeo
{
	class TextureSamplerGL : public TextureSampler
	{
	public:

		TextureSamplerGL(
			TextureMinificationFilter minification_filter,
			TextureMagnificationFilter magnification_filter,
			TextureWrap wrap_s,
			TextureWrap wrap_t,
			float maximum_anistropy);

		~TextureSamplerGL();

		void bind(int texture_unit_index) const;
		static void unbind(int texture_unit_index);

	private:

		unsigned int m_handle;

	};
}
