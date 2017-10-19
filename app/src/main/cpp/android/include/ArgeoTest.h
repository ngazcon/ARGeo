#pragma once

#include "ArgeoNativeActivity.h"
#include "GestureListener.h"

namespace argeo
{
	namespace jni
	{
		class ArgeoTest :
			public ArgeoNativeActivity,
			public GestureListener
		{
		public:
			ArgeoTest();
			~ArgeoTest();

			bool on_tap_event(const TouchEvent& args);

			void on_start();
			void on_stop();

		protected:
			virtual void load_resources();
			virtual void unload_resources();
		};
	}
}
