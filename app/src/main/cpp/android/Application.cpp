#include "include/AndroidNativeAppGlue.h"
#include "include/Application.h"

namespace argeo
{
	android_app* Application::s_android_app = nullptr;

	void Application::set_android_app(android_app* android_app)
	{
		s_android_app = android_app;
	}

	android_app* Application::get_android_app()
	{
		return s_android_app;
	}
}