#pragma once

#include "GL.h"

#define GL_GLEXT_PROTOTYPES

/* GL_OES_mapbuffer */
#ifndef GL_OES_mapbuffer
#define GL_READ_ONLY_OES										GL_MAP_READ_BIT
#define GL_WRITE_ONLY_OES                                       0x88B9
#define GL_BUFFER_ACCESS_OES                                    0x88BB
#define GL_BUFFER_MAPPED_OES                                    0x88BC
#define GL_BUFFER_MAP_POINTER_OES                               0x88BD
#endif

namespace argeo
{
	class ExtensionsES
	{
	public:
		static PFNGLREADBUFFERNVPROC glReadBufferNV;
		static PFNGLMAPBUFFEROESPROC glMapBufferOES;
		static PFNGLUNMAPBUFFEROESPROC glUnmapBufferOES;

	public:
		static void init();
	};
}
