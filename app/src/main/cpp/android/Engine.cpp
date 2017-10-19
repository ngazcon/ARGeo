#include "include/Engine.h"
#include "include/JNIHelper.h"
#include "include/LocationManager.h"

namespace argeo
{
	void Engine::init(
		JNIEnv* jni,
		jobject context)
	{
		JavaVM* jvm;
		jni->GetJavaVM(&jvm);

		JNIHelper::init(
			jni,
			context);

		// Trys to find the GPSListener class. As I said, this should not be here
		// but inside the QT application however I cannot make it work otherwise as
		// findclass cannot find it.

		jclass location_listener_class = JNIHelper::get_instance().retrieve_class(
			jni,
			"vyglab/argeo/jni/ArgeoLocationListener");

		LocationManager::get_instance().init(
			context,
			location_listener_class,
			jvm,
			jni);
	}	
}
