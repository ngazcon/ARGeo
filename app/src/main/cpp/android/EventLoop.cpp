#include "include/EventLoop.h"

#include "include/Application.h"
#include "InputManager.h"
#include "include/SensorManager.h"
#include "include/LocationManager.h"
#include "include/AndroidNativeAppGlue.h"

#include <android/input.h>

namespace argeo
{
	EventLoop::EventLoop(
		ActivityHandler* activity_handler)
		: m_enabled(false)
		, m_quit(false)
		, m_activity_handler(activity_handler)
	{
		SensorManager::get_instance();
		LocationManager::get_instance();
		
		Application::get_android_app()->userData = this;
		Application::get_android_app()->onAppCmd = callback_app_event;
		Application::get_android_app()->onInputEvent = callback_input_event;
	}


	void EventLoop::run()
	{
		while (true)
		{
			int32_t result;
			int32_t events;
			android_poll_source* source;

			// Read all pending events.
			// If not animating, we will block forever waiting for events.
			// If animating, we loop until all events are read, then continue
			// to draw the next frame of animation.
			while ((result = ALooper_pollAll(
				m_enabled ? 0 : -1,
				nullptr,
				&events,
				reinterpret_cast<void**>(&source))) >= 0)
			{
				// Process this event.
				if (source != nullptr)
				{
					source->process(Application::get_android_app(), source);
				}

				// Check if we are exiting.
				if (Application::get_android_app()->destroyRequested != 0)
				{
					return;
				}
			}

			// Steps the application
			if (m_enabled && !m_quit)
			{
				if (m_activity_handler->on_step() != STATUS_OK)
				{
					m_quit = true;
					ANativeActivity_finish(Application::get_android_app()->activity);
				}
			}
		}
	}

	void EventLoop::activate()
	{
		// Enables activity only if a window is available.
		if (!m_enabled && Application::get_android_app()->window != nullptr)
		{
			m_quit	  = false;
			m_enabled = true;
			if (m_activity_handler->on_activate() != STATUS_OK)
			{
				m_quit = true;
				deactivate();
				ANativeActivity_finish(Application::get_android_app()->activity);
			}
		}
	}

	void EventLoop::deactivate()
	{
		if (m_enabled)
		{
			m_activity_handler->on_deactivate();
			m_enabled = false;
		}
	}


	// -----------------------------------------------------------
	//						Callbacks
	// -----------------------------------------------------------

	int32_t EventLoop::callback_input_event(
		android_app* app,
		AInputEvent* event)
	{
		EventLoop* event_loop = static_cast<EventLoop*>(app->userData);
		return event_loop->process_input_event(event);
	}

	void EventLoop::callback_app_event(
		android_app* app,
		int32_t command)
	{
		EventLoop* event_loop = static_cast<EventLoop*>(app->userData);
		event_loop->process_app_event(command);
	}

	

	int32_t EventLoop::process_input_event(AInputEvent* event)
	{
		if (!m_enabled)
		{
			return 0;
		}

		int32_t event_type = AInputEvent_getType(event);
		switch (event_type)
		{
		case AINPUT_EVENT_TYPE_MOTION:
			switch (AInputEvent_getSource(event))
			{
			case AINPUT_SOURCE_TOUCHSCREEN:
				InputManager::process_touch_event(TouchEvent(
					Point(AMotionEvent_getX(event, 0), AMotionEvent_getY(event, 0)),
					AMotionEvent_getAction(event) == AMOTION_EVENT_ACTION_DOWN ? Action::Down : Action::Up,
					AMotionEvent_getEventTime(event),
					AMotionEvent_getButtonState(event) == AMOTION_EVENT_BUTTON_PRIMARY ? ActionButton::Right : ActionButton::Left,
					event));
			}
			break;

		case AINPUT_EVENT_TYPE_KEY:
			return InputManager::process_key_event(KeyboardKeyEventArgs(
				KeyboardKey::E,
				AKeyEvent_getAction(event) == AKEY_EVENT_ACTION_DOWN ? KeyboardAction::Down : KeyboardAction::Up));

		}

		return 0;
	}

	void EventLoop::process_app_event(int32_t command)
	{
		switch (command)
		{
		case APP_CMD_CONFIG_CHANGED:
			m_activity_handler->on_configuration_changed();
			break;
		case APP_CMD_INIT_WINDOW:
			m_activity_handler->on_create_window();
			break;
		case APP_CMD_TERM_WINDOW:
			m_activity_handler->on_destroy_window();
			deactivate();
			break;
		case APP_CMD_RESUME:
			m_activity_handler->on_resume();
			break;
		case APP_CMD_START:
			m_activity_handler->on_start();
			break;
		case APP_CMD_PAUSE:
			m_activity_handler->on_pause();
			deactivate();
			break;
		case APP_CMD_STOP:
			m_activity_handler->on_stop();
			break;
		case APP_CMD_GAINED_FOCUS:
			activate();
			m_activity_handler->on_gain_focus();
			break;
		case APP_CMD_LOST_FOCUS:
			m_activity_handler->on_lost_focus();
			deactivate();
			break;
		case APP_CMD_LOW_MEMORY:
			m_activity_handler->on_low_memory();
			break;
		case APP_CMD_DESTROY:
			m_activity_handler->on_destroy();
			break;
		case APP_CMD_SAVE_STATE:
			m_activity_handler->on_save_instace_state(
				&Application::get_android_app()->savedState,
				&Application::get_android_app()->savedStateSize);
			break;
		}
	}
}
