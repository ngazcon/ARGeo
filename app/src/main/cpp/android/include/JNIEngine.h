#pragma once

#include "Bootstrap.h"
#include "JniHelpers.h"
#include <jni.h>

namespace argeo
{
	class JNIEngine
	{
	public:
		static const char* getCanonicalName()
		{
			return MAKE_CANONICAL_NAME(PACKAGE, Engine);
		}

		static void init(
			JNIEnv* jni,
			jobject object,
			jobject context);

		static void register_natives(JNIEnv* jni);

	private:
		static const char* class_path_name;
	};
}
