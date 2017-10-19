#pragma once

#include "RenderBuffer.h"

namespace argeo
{
	class RenderBufferGL : public RenderBuffer
	{
	public:
		RenderBufferGL(
			RenderBufferDescription description);

		~RenderBufferGL();

		RenderBufferDescription get_description() const;
		unsigned int get_handle() const;

		void bind();
		static void unbind();

	private:
		RenderBufferDescription m_description;
		unsigned int m_handle;
	};
}
