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


		addNativeMethod(
				"setSensorLowPassFilterAlpha",
				(void*)&JNIDevicePoseCameraController::set_sensor_low_pass_filter_alpha,
                kTypeVoid,
				kTypeDouble,
				NULL);

		addNativeMethod(
				"getSensorLowPassFilterAlpha",
				(void*)&JNIDevicePoseCameraController::get_sensor_low_pass_filter_alpha,
                kTypeDouble,
				NULL);

        addNativeMethod(
                "setGPSStandingFrec",
                (void*)&JNIDevicePoseCameraController::set_gps_standing_frec,
                kTypeVoid,
                kTypeInt,
                NULL);

        addNativeMethod(
                "getGPSStandingFrec",
                (void*)&JNIDevicePoseCameraController::get_gps_standing_frec,
                kTypeInt,
                NULL);

        addNativeMethod(
                "setGPSMovingFrec",
                (void*)&JNIDevicePoseCameraController::set_gps_moving_frec,
                kTypeVoid,
                kTypeInt,
                NULL);

        addNativeMethod(
                "getGPSMovingFrec",
                (void*)&JNIDevicePoseCameraController::get_gps_moving_frec,
                kTypeInt,
                NULL);

        addNativeMethod(
                "setNetworkStandingFrec",
                (void*)&JNIDevicePoseCameraController::set_network_standing_frec,
                kTypeVoid,
                kTypeInt,
                NULL);

        addNativeMethod(
                "getNetworkStandingFrec",
                (void*)&JNIDevicePoseCameraController::get_network_standing_frec,
                kTypeInt,
                NULL);

        addNativeMethod(
                "setNetworkMovingFrec",
                (void*)&JNIDevicePoseCameraController::set_network_moving_frec,
                kTypeVoid,
                kTypeInt,
                NULL);

        addNativeMethod(
                "getNetworkMovingFrec",
                (void*)&JNIDevicePoseCameraController::get_network_moving_frec,
                kTypeInt,
                NULL);

        addNativeMethod(
                "setInitialSamples",
                (void*)&JNIDevicePoseCameraController::set_initial_samples,
                kTypeVoid,
                kTypeInt,
                NULL);

        addNativeMethod(
                "getInitialSamples",
                (void*)&JNIDevicePoseCameraController::get_initial_samples,
                kTypeInt,
                NULL);

        addNativeMethod(
                "setAccuracyDelta",
                (void*)&JNIDevicePoseCameraController::set_accuracy_delta,
                kTypeVoid,
                kTypeInt,
                NULL);

        addNativeMethod(
                "getAccuracyDelta",
                (void*)&JNIDevicePoseCameraController::get_accuracy_delta,
                kTypeInt,
                NULL);

        addNativeMethod(
                "forceLocationUpdates",
                (void*)&JNIDevicePoseCameraController::force_location_updates,
                kTypeVoid,
                NULL);

        addNativeMethod(
                "getGPSState",
                (void*)&JNIDevicePoseCameraController::get_gps_state,
                kTypeInt,
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

    jdouble JNIDevicePoseCameraController::get_sensor_low_pass_filter_alpha(
            JNIEnv *jni,
            jobject java_this)
    {
        JNIDevicePoseCameraController* object = gClasses.getNativeInstance<JNIDevicePoseCameraController>(jni, java_this);
        return object->getNativeObject()->get_sensor_low_pass_filter_alpha();
    }

    void JNIDevicePoseCameraController::set_sensor_low_pass_filter_alpha(
            JNIEnv *jni,
            jobject java_this,
            jdouble alpha)
    {
        JNIDevicePoseCameraController* object = gClasses.getNativeInstance<JNIDevicePoseCameraController>(jni, java_this);
        object->getNativeObject()->set_sensor_low_pass_filter_alpha(alpha);
    }

    void JNIDevicePoseCameraController::set_accuracy_delta(
            JNIEnv *jni,
            jobject java_this,
            jint accuracy_delta)
    {
        JNIDevicePoseCameraController* object = gClasses.getNativeInstance<JNIDevicePoseCameraController>(jni, java_this);
        object->getNativeObject()->set_accuracy_delta(accuracy_delta);
    }

    jint JNIDevicePoseCameraController::get_accuracy_delta(
            JNIEnv *jni,
            jobject java_this)
    {
        JNIDevicePoseCameraController* object = gClasses.getNativeInstance<JNIDevicePoseCameraController>(jni, java_this);
        return object->getNativeObject()->get_accuracy_delta();
    }

    void JNIDevicePoseCameraController::set_initial_samples(
            JNIEnv *jni,
            jobject java_this,
            jint initial_samples)
    {
        JNIDevicePoseCameraController* object = gClasses.getNativeInstance<JNIDevicePoseCameraController>(jni, java_this);
        object->getNativeObject()->set_initial_samples(initial_samples);
    }

    jint JNIDevicePoseCameraController::get_initial_samples(
            JNIEnv *jni,
            jobject java_this)
    {
        JNIDevicePoseCameraController* object = gClasses.getNativeInstance<JNIDevicePoseCameraController>(jni, java_this);
        return object->getNativeObject()->get_initial_samples();
    }

    void JNIDevicePoseCameraController::set_gps_moving_frec(
            JNIEnv *jni,
            jobject java_this,
            jint gps_moving_frec)
    {
        JNIDevicePoseCameraController* object = gClasses.getNativeInstance<JNIDevicePoseCameraController>(jni, java_this);
        object->getNativeObject()->set_gps_moving_frec(gps_moving_frec);
    }

    jint JNIDevicePoseCameraController::get_gps_moving_frec(
            JNIEnv *jni,
            jobject java_this)
    {
        JNIDevicePoseCameraController* object = gClasses.getNativeInstance<JNIDevicePoseCameraController>(jni, java_this);
        return object->getNativeObject()->get_gps_moving_frec();
    }

    void JNIDevicePoseCameraController::set_gps_standing_frec(
            JNIEnv *jni,
            jobject java_this,
            jint gps_standing_frec)
    {
        JNIDevicePoseCameraController* object = gClasses.getNativeInstance<JNIDevicePoseCameraController>(jni, java_this);
        object->getNativeObject()->set_gps_standing_frec(gps_standing_frec);
    }

    jint JNIDevicePoseCameraController::get_gps_standing_frec(
            JNIEnv *jni,
            jobject java_this)
    {
        JNIDevicePoseCameraController* object = gClasses.getNativeInstance<JNIDevicePoseCameraController>(jni, java_this);
        return object->getNativeObject()->get_gps_standing_frec();
    }

    void JNIDevicePoseCameraController::set_network_moving_frec(
            JNIEnv *jni,
            jobject java_this,
            jint network_moving_frec)
    {
        JNIDevicePoseCameraController* object = gClasses.getNativeInstance<JNIDevicePoseCameraController>(jni, java_this);
        object->getNativeObject()->set_network_moving_frec(network_moving_frec);
    }

    jint JNIDevicePoseCameraController::get_network_moving_frec(
            JNIEnv *jni,
            jobject java_this)
    {
        JNIDevicePoseCameraController* object = gClasses.getNativeInstance<JNIDevicePoseCameraController>(jni, java_this);
        return object->getNativeObject()->get_network_moving_frec();
    }

    void JNIDevicePoseCameraController::set_network_standing_frec(
            JNIEnv *jni,
            jobject java_this,
            jint network_standing_frec)
    {
        JNIDevicePoseCameraController* object = gClasses.getNativeInstance<JNIDevicePoseCameraController>(jni, java_this);
        object->getNativeObject()->set_network_standing_frec(network_standing_frec);
    }

    jint JNIDevicePoseCameraController::get_network_standing_frec(
            JNIEnv *jni,
            jobject java_this)
    {
        JNIDevicePoseCameraController* object = gClasses.getNativeInstance<JNIDevicePoseCameraController>(jni, java_this);
        return object->getNativeObject()->get_network_standing_frec();
    }

    void JNIDevicePoseCameraController::force_location_updates(
            JNIEnv *jni,
            jobject java_this)
    {
        JNIDevicePoseCameraController* object = gClasses.getNativeInstance<JNIDevicePoseCameraController>(jni, java_this);
        object->getNativeObject()->force_location_updates();
    }

    jint JNIDevicePoseCameraController::get_gps_state(
            JNIEnv *jni,
            jobject java_this)
    {
        JNIDevicePoseCameraController* object = gClasses.getNativeInstance<JNIDevicePoseCameraController>(jni, java_this);
        return static_cast<jint>(object->getNativeObject()->get_gps_state());
    }
}