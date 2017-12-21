#pragma once

#include "JniHelpers.h"
#include "Bootstrap.h"
#include "NativeObjectWrapper.h"
#include "DevicePoseCameraController.h"

namespace argeo
{
	class JNIDevicePoseCameraController
		: public NativeObjectWrapper<DevicePoseCameraController>
	{
	public:
		JNIDevicePoseCameraController(DevicePoseCameraController* camera_controller = nullptr);
		JNIDevicePoseCameraController(
			JNIEnv* jni,
			DevicePoseCameraController* camera_controller = nullptr);

		const char* getCanonicalName() const
		{
			return MAKE_CANONICAL_NAME(PACKAGE, DevicePoseCameraController);
		}

		void initialize(JNIEnv *env);
        void mapFields();

		static void dispose(
			JNIEnv *env,
			jobject java_this);

		static jobject create(
			JNIEnv* jni,
			DevicePoseCameraController* camera_controller);

		static jobject get_position(
			JNIEnv *env,
			jobject java_this);

		static void enable_orientation_updates(
			JNIEnv   *jni,
			jobject  java_this);

		static void disable_orientation_updates(
				JNIEnv   *jni,
				jobject  java_this);

		static void enable_location_updates(
			JNIEnv   *jni,
			jobject  java_this);

        static void disable_location_updates(
                JNIEnv   *jni,
                jobject  java_this);

		static jdouble get_pitch(
			JNIEnv   *jni,
			jobject  java_this);

		static jdouble get_yaw(
			JNIEnv   *jni,
			jobject  java_this);

		static jdouble get_roll(
			JNIEnv   *jni,
			jobject  java_this);


        static jint get_gps_state(
                JNIEnv   *jni,
                jobject  java_this);

		static jdouble get_sensor_low_pass_filter_alpha(
				JNIEnv   *jni,
				jobject  java_this);

		static void set_sensor_low_pass_filter_alpha(
				JNIEnv   *jni,
				jobject  java_this,
				jdouble  alpha);

        static jint get_network_standing_frec(
                JNIEnv   *jni,
                jobject  java_this);

        static void set_network_standing_frec(
                JNIEnv   *jni,
                jobject  java_this,
                jint     network_standing_frec);

        static jint get_network_moving_frec(
                JNIEnv   *jni,
                jobject  java_this);

        static void set_network_moving_frec(
                JNIEnv   *jni,
                jobject  java_this,
                jint    network_moving_frec);

        static jint get_gps_moving_frec(
                JNIEnv   *jni,
                jobject  java_this);

        static void set_gps_moving_frec(
                JNIEnv   *jni,
                jobject  java_this,
                jint     gps_moving_frec);

        static jint get_gps_standing_frec(
                JNIEnv   *jni,
                jobject  java_this);

        static void set_gps_standing_frec(
                JNIEnv   *jni,
                jobject  java_this,
                jint     gps_standing_frec);

        static jint get_initial_samples(
                JNIEnv   *jni,
                jobject  java_this);

        static void set_initial_samples(
                JNIEnv   *jni,
                jobject  java_this,
                jint     initial_samples);

        static jint  get_accuracy_delta(
                JNIEnv   *jni,
                jobject  java_this);

        static void set_accuracy_delta(
                JNIEnv   *jni,
                jobject  java_this,
                jint     accuracy_delta);

        static void force_location_updates(
                JNIEnv   *jni,
                jobject  java_this);
	};
}
