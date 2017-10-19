#include "PickFramebuffer.h"

#include "IPickable.h"
#include "Device.h"


namespace argeo
{
	PickFramebuffer::PickFramebuffer(Context* context)
		: m_context(context)
		, m_pass_state(new PassState(context))
		, m_width(0)
		, m_height(0)
		, m_viewport(new Rectangle())
	{ 
		m_pass_state->set_viewport(m_viewport.get());
		m_pass_state->set_blending(new Blending(false));
		m_pass_state->set_scissor_test(new ScissorTest(true));
	}


	PickFramebuffer::~PickFramebuffer()
	{
		delete m_pass_state->get_blending();
		delete m_pass_state->get_scissor_test();
		delete m_pass_state->get_viewport();
	}

	PassState* PickFramebuffer::begin(Rectangle screen_space_rectangle)
	{
		unsigned int width   = m_context->get_drawing_surface_width();
		unsigned int height  = m_context->get_drawing_surface_height();

		m_pass_state->get_scissor_test()->rectangle = screen_space_rectangle;

		// Initially create or recreate renderbuffers and framebuffer used for picking
		if (m_framebuffer == nullptr || m_width != width || m_height != height) 
		{
			m_width  = width;
			m_height = height;

			m_framebuffer = m_context->create_frame_buffer();

			m_depth_stencil_texture = Device::create_texture2D(TextureDescription(
				width,
				height, 
				TextureFormat::Depth24Stencil8));

			m_color_attachment = Device::create_texture2D(TextureDescription(
				width, 
				height, 
				TextureFormat::RedGreenBlueAlpha8));

			m_framebuffer->get_color_attachments()->set_attachment(0, m_color_attachment.get());
			m_framebuffer->set_depth_stencil_attachment_texture(m_depth_stencil_texture.get());
			m_pass_state->set_framebuffer(m_framebuffer.get());
		}

		m_pass_state->get_viewport()->set_width(width);
		m_pass_state->get_viewport()->set_height(height);

		return m_pass_state.get();
	}

	IPickable* PickFramebuffer::end(Rectangle screen_space_rectangle)
	{
		unsigned int width  = screen_space_rectangle.get_width();
		unsigned int height = screen_space_rectangle.get_height();

		std::vector<vec4ub> pixels = m_context->read_pixels(
			screen_space_rectangle.get_left(),
			screen_space_rectangle.get_bottom(),
			width,
			height,
			m_framebuffer.get());

		int max		    = fmax(width, height);
		int length	    = max * max;
		int half_width  = floor(width * 0.5);
		int half_height = floor(height * 0.5);

		int x  = 0;
		int y  = 0;
		int dx = 0;
		int dy = -1;

		// Spiral around the center pixel, this is a workaround until
		// we can access the depth buffer on all browsers.

		// The region does not have to square and the dimensions do not have to be odd, but
		// loop iterations would be wasted. Prefer square regions where the size is odd.
		for (int i = 0; i < length; ++i) 
		{
			if (-half_width <= x && x <= half_width && -half_height <= y && y <= half_height)
			{
				unsigned int index = ((half_height - y) * width + x + half_width);

				Color color(
					pixels[index].x,
					pixels[index].y,
					pixels[index].z,
					pixels[index].w);

				IPickable* object = m_context->get_object_by_pick_color(color);

				if (object != nullptr) 
				{
					return object;
				}
			}

			// if (top right || bottom left corners) || (top left corner) || (bottom right corner + (1, 0))
			// change spiral direction
			if (x == y || (x < 0 && -x == y) || (x > 0 && x == 1 - y)) 
			{
				int temp = dx;
				dx		 = -dy;
				dy		 = temp;
			}

			x += dx;
			y += dy;
		}

		return nullptr;
	}
}