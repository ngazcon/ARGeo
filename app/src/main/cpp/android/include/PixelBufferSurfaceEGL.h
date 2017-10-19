#pragma once

#include "PixelBufferSurface.h"
#include "SurfaceEGL.h"

namespace argeo
{
	class PixelBufferSurfaceEGL : public PixelBufferSurface, public SurfaceEGL
	{
	public:
		PixelBufferSurfaceEGL(
			int width, 
			int height,
			Context* shared_context);

		~PixelBufferSurfaceEGL();

		Context* get_context();

		unsigned int  get_width() const;
		void set_width(const unsigned int& value);

		unsigned int  get_height() const;
		void set_height(const unsigned int& value);

		EGLSurface get_surface() const;
		EGLDisplay get_display() const;
		EGLConfig  get_config() const;

	private:

		int m_width;
		int m_height;

		EGLConfig  m_config;
		EGLSurface m_surface;
		EGLDisplay m_display;

		std::unique_ptr<Context>  m_context;

		Context* m_shared_context;
	};
}
