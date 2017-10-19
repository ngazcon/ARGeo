#pragma once

namespace argeo
{
	class Application
	{
	public:
		static android_app* get_android_app();
		static void set_android_app(android_app* android_app);
	private:
		static android_app* s_android_app;
	};
}
