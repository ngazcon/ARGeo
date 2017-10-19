#include "include/JNIDevicePoseCameraController.h"
#include "include/JNIGeodetic3D.h"

namespace argeo
{
	JNIDevicePoseCameraController::JNIDevicePoseCameraController(DevicePoseCameraController* camera_controller)
		: NativeObjectWrapper()
	{ }

	JNIDevicePoseCameraController::JNIDevicePoseCameraController(
		JNIEnv *env,
		DevicePoseCameraController* camera_controller)
		: NativeObjectWrapper(env, camera_controller)
	{
		initialize(env);
	}

	void JNIDevicePoseCameraController::initialize(JNIEnv *env)
	{
		setClass(env);

		cacheConstructor(env);

		addNativeMethod(
			"dispose",
			(void*)&JNIDevicePoseCameraController::dispose,
			kTypeVoid,
			NULL);

		addNativeMethod(
			"enableLocationUpdates",
			(void*)&JNIDevicePoseCameraController::enable_location_updates,
			NULL);

		addNativeMethod(
			"disableLocationUpdates",
			(void*)&JNIDevicePoseCameraController::disable_location_updates,
			kTypeVoid,
			NULL);

		addNativeMethod(
			"enableOrientationUpdates",
			(void*)&JNIDevicePoseCameraController::enable_orientation_updates,
			NULL);

        addNativeMethod(
            "disableOrientationUpdates",
            (void*)&JNIDevicePoseCameraController::disable_orientation_updates,
            NULL);

		addNativeMethod(
			"getPosition",
			(void*)&JNIDevicePoseCameraController::get_position,
			MAKE_CANONICAL_NAME(PACKAGE, Geodetic3D),
			NULL);

		addNativeMethod(
			"getPitch",
			(void*)&JNIDevicePoseCameraController::get_pitch,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"getYaw",
			(void*)&JNIDevicePoseCameraController::get_yaw,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"getRoll",
			(void*)&JNIDevicePoseCameraController::get_roll,
			kTypeDouble,
			NULL);

		registerNativeMethods(env);
	}

	void JNIDevicePoseCameraController::mapFields()
	{ }

	void JNIDevicePoseCameraController::dispose(JNIEnv *env, jobject java_this)
	{
		JNIDevicePoseCameraController *object = gClasses.getNativeInstance<JNIDevicePoseCameraController>(env, java_this);
		if (object != NULL)
		{
			object->destroy(env, java_this);
		}
	}

	jobject JNIDevicePoseCameraController::create(
		JNIEnv* jni,
		DevicePoseCameraController* camera_controller)
	{
		JNIDevicePoseCameraController* object = gClasses.newInstance<JNIDevicePoseCameraController>(jni, nullptr);
		object->setNativeObject(camera_controller);
		jobject java_this = object->toJavaObject(jni);
		object->persist(jni, java_this);
		return java_this;
	}

	jobject JNIDevicePoseCameraController::get_position(
		JNIEnv *jni,
		jobject java_this)
	{
		JNIDevicePoseCameraController* object = gClasses.getNativeInstance<JNIDevicePoseCameraController>(jni, java_this);
		return JNIGeodetic3D::create(jni, object->getNativeObject()->get_position());
	}

	void JNIDevicePoseCameraController::enable_location_updates(
			JNIEnv *jni,
			jobject java_this)
	{
		JNIDevicePoseCameraController* object = gClasses.getNativeInstance<JNIDevicePoseCameraController>(jni, java_this);
		object->getNativeObject()->enable_location_updates();
	}

    void JNIDevicePoseCameraController::disable_location_updates(
            JNIEnv *jni,
            jobject java_this)
    {
        JNIDevicePoseCameraController* object = gClasses.getNativeInstance<JNIDevicePoseCameraController>(jni, java_this);
        object->getNativeObject()->disable_location_updates();
    }

	void JNIDevicePoseCameraController::enable_orientation_updates(
            JNIEnv *jni,
            jobject java_this)
	{
		JNIDevicePoseCameraController* object = gClasses.getNativeInstance<JNIDevicePoseCameraController>(jni, java_this);
		object->getNativeObject()->enable_orientation_updates();
	}

    void JNIDevicePoseCameraController::disable_orientation_updates(
            JNIEnv *jni,
            jobject java_this)
    {
        JNIDevicePoseCameraController* object = gClasses.getNativeInstance<JNIDevicePoseCameraController>(jni, java_this);
        object->getNativeObject()->disable_orientation_updates();
    }

	jdouble JNIDevicePoseCameraController::get_yaw(
		JNIEnv   *jni,
		jobject  java_this)
	{
		JNIDevicePoseCameraController* object = gClasses.getNativeInstance<JNIDevicePoseCameraController>(jni, java_this);
		return object->getNativeObject()->get_yaw();
	}

	jdouble JNIDevicePoseCameraController::get_pitch(
		JNIEnv   *jni,
		jobject  java_this)
	{
		JNIDevicePoseCameraController* object = gClasses.getNativeInstance<JNIDevicePoseCameraController>(jni, java_this);
		return object->getNativeObject()->get_pitch();
	}

	jdouble JNIDevicePoseCameraController::get_roll(
		JNIEnv   *jni,
		jobject  java_this)
	{
		JNIDevicePoseCameraController* object = gClasses.getNativeInstance<JNIDevicePoseCameraController>(jni, java_this);
		return object->getNativeObject()->get_roll();
	}
}