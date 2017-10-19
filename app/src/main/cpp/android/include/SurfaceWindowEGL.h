#pragma once

#include "SurfaceWindow.h"
#include "SurfaceEGL.h"
#include "Context.h"

#include <string>

namespace argeo
{
	class SurfaceWindowEGL : public SurfaceWindow, public SurfaceEGL
	{
	public:
		SurfaceWindowEGL(
			Context* shared_context,
			ANativeWindow* window);

		~SurfaceWindowEGL();

		void update();
		void stop();

		Context* get_context();

		unsigned int get_width() const;
		unsigned int get_height() const;

		EGLSurface get_surface() const;
		EGLDisplay get_display() const;
		EGLConfig  get_config() const;

	private:

		unsigned int m_width;
		unsigned int m_height;

		EGLConfig  m_config;
		EGLSurface m_surface;
		EGLDisplay m_display;

		std::unique_ptr<Context>  m_context;

		Context* m_shared_context;
	};
}
