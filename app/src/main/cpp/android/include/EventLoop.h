#pragma once

#include "ActivityHandler.h"
#include "AndroidNativeAppGlue.h"

#include <vector>

namespace argeo
{
	class EventLoop
	{
	public:

		EventLoop(ActivityHandler* activity_handler);
		
		void run();

	private:

		void activate();
		void deactivate();

		void process_app_event(int32_t command);
		int32_t process_input_event(AInputEvent* event);

		static void callback_app_event(
			android_app* app,
			int32_t command);

		static int32_t callback_input_event(
			android_app* app,
			AInputEvent* event);

	private:

		bool m_enabled;
		bool m_quit;

		ActivityHandler* m_activity_handler;
	};
}

