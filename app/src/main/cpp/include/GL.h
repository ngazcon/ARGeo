//
// stdafx.h
// Header for standard system include files.
//
// Used by the build system to generate the precompiled header.


#ifdef __ANDROID__
#include <EGL/egl.h>
#include <EGL/eglext.h>


#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3platform.h>
#else
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif