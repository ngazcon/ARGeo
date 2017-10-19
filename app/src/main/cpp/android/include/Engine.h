#pragma once

#include <jni.h>

namespace argeo
{
	class Engine
	{
	public:
		static void init(
			JNIEnv* jni,
			jobject context);
	};
}
