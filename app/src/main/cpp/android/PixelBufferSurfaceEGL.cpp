#include "include/PixelBufferSurfaceEGL.h"
#include "include/ContextEGL.h"

#include <stdexcept>
#include <chrono>

namespace argeo
{
	PixelBufferSurfaceEGL::PixelBufferSurfaceEGL(
		int width,
		int height,
		Context* shared_context)
		: m_width(width)
		, m_height(height)
		, m_shared_context(shared_context)
		, m_surface(EGL_NO_SURFACE)
	{
		// First we initialize the display
		m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

		// Get the current display
		if (m_display == EGL_NO_DISPLAY)
		{
			throw std::logic_error("No display available.");
		}

		/*
		* Here specify the attributes of the desired configuration.
		* Below, we select an EGLConfig with at least 8 bits per color
		* component compatible with on-screen windows
		*/
		const EGLint attribs[] =
		{
			EGL_SURFACE_TYPE,    EGL_PBUFFER_BIT,
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
			EGL_RED_SIZE,             8,
			EGL_GREEN_SIZE,           8,
			EGL_BLUE_SIZE,            8,
			EGL_ALPHA_SIZE,           8,
			EGL_DEPTH_SIZE,			  24,
			EGL_SAMPLES,			  4,
			EGL_SAMPLE_BUFFERS,		  1,
			EGL_NONE
		};

		/* Here, the application chooses the configuration it desires. In this
		* sample, we have a very simplified selection process, where we pick
		* the first EGLConfig that matches our criteria */
		EGLint num_configs;
		if (!eglChooseConfig(m_display, attribs, &m_config, 1, &num_configs) || num_configs <= 0)
		{
			throw std::logic_error("Cannot get the config for the given attributes.");
		}

		/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
		* guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
		* As soon as we picked a EGLConfig, we can safely reconfigure the
		* ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
		EGLint format;
		if (!eglGetConfigAttrib(m_display, m_config, EGL_NATIVE_VISUAL_ID, &format))
		{
			throw std::logic_error("Cannot get the format for this config.");
		}

		/* Pixel buffer attributes. */
		EGLint p_buffer_attributes[] =
		{
			EGL_WIDTH,  m_width,
			EGL_HEIGHT, m_height,
			EGL_NONE
		};

		m_surface = eglCreatePbufferSurface(m_display, m_config, p_buffer_attributes);

		if (m_surface == EGL_NO_SURFACE)
		{
			throw std::logic_error("Cannot create the surface.");
		}

		// Get the current width and height of the display.
		eglQuerySurface(m_display, m_surface, EGL_WIDTH, &m_width);
		eglQuerySurface(m_display, m_surface, EGL_HEIGHT, &m_height);

		m_context = std::unique_ptr<ContextEGL>(
			new ContextEGL(this, m_shared_context, m_width, m_height));		
	}

	PixelBufferSurfaceEGL::~PixelBufferSurfaceEGL()
	{
		eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

		if (m_surface != EGL_NO_SURFACE)
		{
			eglDestroySurface(m_display, m_surface);
		}
	}

	Context* PixelBufferSurfaceEGL::get_context()
	{
		return m_context.get();
	}

	void PixelBufferSurfaceEGL::set_width(const unsigned int& value)
	{
		throw std::logic_error("Set the width for this window is not supported.");
	}

	void PixelBufferSurfaceEGL::set_height(const unsigned int& value)
	{
		throw std::logic_error("Set the height for this window is not supported.");
	}

	unsigned int PixelBufferSurfaceEGL::get_width() const
	{
		return m_width;
	}

	unsigned int PixelBufferSurfaceEGL::get_height() const
	{
		return m_height;
	}

	EGLSurface PixelBufferSurfaceEGL::get_surface() const
	{
		return m_surface;
	}

	EGLDisplay PixelBufferSurfaceEGL::get_display() const
	{
		return m_display;
	}

	EGLConfig  PixelBufferSurfaceEGL::get_config() const
	{
		return m_config;
	}

}
