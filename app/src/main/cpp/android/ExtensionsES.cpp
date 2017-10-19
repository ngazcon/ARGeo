#include "include/ExtensionsES.h"
#include "GL.h"

#include <string>

namespace argeo
{
	PFNGLMAPBUFFEROESPROC   ExtensionsES::glMapBufferOES = nullptr;
	PFNGLUNMAPBUFFEROESPROC ExtensionsES::glUnmapBufferOES = nullptr;
	PFNGLREADBUFFERNVPROC   ExtensionsES::glReadBufferNV = nullptr;

	void ExtensionsES::init()
	{
		const char* extensions = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));

		if (strstr(extensions, "GL_OES_mapbuffer"))
		{
			glMapBufferOES   = (PFNGLMAPBUFFEROESPROC)eglGetProcAddress("glMapBufferOES");
			glUnmapBufferOES = (PFNGLUNMAPBUFFEROESPROC)eglGetProcAddress("glUnmapBufferOES");
		}

		if (strstr(extensions, "NV_read_buffer"))
		{
			glReadBufferNV = (PFNGLREADBUFFERNVPROC)eglGetProcAddress("NV_read_buffer");
		}
	}
}