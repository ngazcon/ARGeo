#pragma once 

#include "EventLoop.h"
#include "ActivityHandler.h"
#include "Viewer.h"
#include "VideoCapture.h"
#include "PreRenderListener.h"
#include "AndroidNativeAppGlue.h"

#include <android/input.h>
#include <memory>

namespace argeo
{
	namespace jni
	{
		class ArgeoNativeActivity
			: public ActivityHandler
			, public PreRenderListener
		{
		public:
			ArgeoNativeActivity();
			~ArgeoNativeActivity();

			void run();
			void pre_render(
				Scene* scene,
				EngineClockTick time);

			Viewer* get_viewer();

		protected:
			virtual void load_resources();
			virtual void unload_resources();


		private:
			status on_activate();
			void   on_deactivate();

			status on_step();
			void on_create_window();

		private:
			bool m_resources_created;

			EventLoop m_event_loop;

			std::unique_ptr<Viewer> m_viewer;
			std::unique_ptr<VideoCapture> m_direct_video;
		};
	}
}