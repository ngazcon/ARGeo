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
	};
}
