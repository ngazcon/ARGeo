#pragma once

#include "Context.h"
#include "PassState.h"
#include "Rectangle.h"

namespace argeo
{
	class PickFramebuffer
	{
	public:
		PickFramebuffer(Context* context);
		~PickFramebuffer();

		PassState* begin(Rectangle screen_space_rectangle);
		IPickable* end(Rectangle screen_space_rectangle);

	private:
		Context* m_context;

		std::unique_ptr<PassState>    m_pass_state;
		std::unique_ptr<FrameBuffer>  m_framebuffer;
		std::unique_ptr<Texture2D>    m_depth_stencil_texture;
		std::unique_ptr<Texture2D>    m_color_attachment;
		std::unique_ptr<Rectangle>    m_viewport;

		unsigned int m_width;
		unsigned int m_height;
	};
}
