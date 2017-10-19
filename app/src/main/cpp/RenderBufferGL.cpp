#include "RenderBufferGL.h"

#include "GL.h"
#include "TypeConverterGL.h"

#include <stdexcept>

namespace argeo
{
	RenderBufferGL::RenderBufferGL(
		RenderBufferDescription description)
		: m_description(description)
	{
		if (m_description.get_width() <= 0)
		{
			throw std::invalid_argument("Width must be greater than zero.");
		}

		if (m_description.get_height() <= 0)
		{
			throw std::invalid_argument("Height must be greater than zero.");
		}
		// Create the texture in device memory.
		glGenRenderbuffers(1, &m_handle);

		bind();

		glRenderbufferStorage(
			GL_RENDERBUFFER,
			TypeConverterGL::ToEnum(m_description.get_render_buffer_format()),
			m_description.get_width(), 
			m_description.get_height());

		RenderBufferGL::unbind();
	}


	RenderBufferGL::~RenderBufferGL()
	{
		glDeleteRenderbuffers(1, &m_handle);
	}

	RenderBufferDescription RenderBufferGL::get_description() const
	{
		return m_description;
	}

	unsigned int RenderBufferGL::get_handle() const
	{
		return m_handle;
	}

	void RenderBufferGL::bind()
	{
		glBindRenderbuffer(
			GL_RENDERBUFFER,
			m_handle);
	}

	void RenderBufferGL::unbind()
	{
		glBindRenderbuffer(
			GL_RENDERBUFFER,
			0);
	}
}