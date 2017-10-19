#pragma once

#include "GL.h"

namespace argeo
{
	class SurfaceEGL
	{
	public:

		virtual EGLSurface get_surface() const = 0;
		virtual EGLDisplay get_display() const = 0;
		virtual EGLConfig  get_config() const = 0;
	};
}
