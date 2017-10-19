#include "include/Engine.h"
#include "include/JNIEngine.h"

namespace argeo
{
	///
	/// This is called when an activity is created.
	///
	void JNIEngine::init(
		JNIEnv* jni,
		jobject object,
		jobject context)
	{
		Engine::init(
			jni,
			context);
	}

	void JNIEngine::register_natives(JNIEnv* jni)
	{
		//Declaring the methods. This will be used by registerNatives
		JNINativeMethod methods[] =
		{
			{ "init",  "(Landroid/content/Context;)V", (void*)&JNIEngine::init },
		};

		jclass clazz = jni->FindClass(getCanonicalName());

		// Register the native methods.
		int methods_size = sizeof(methods) / sizeof(methods[0]);
		if (jni->RegisterNatives(clazz, methods, methods_size) < 0)
		{
			throw std::runtime_error("Error running RegisterNatives.");
		}
	}
}