#pragma once

#include "JniHelpers.h"
#include "Bootstrap.h"
#include "NativeObjectWrapper.h"
#include "ScreenSpaceCameraController.h"

namespace argeo
{
	class JNIScreenSpaceCameraController
		: public NativeObjectWrapper<ScreenSpaceCameraController>
	{
	public:
		JNIScreenSpaceCameraController(ScreenSpaceCameraController* camera_controller = nullptr);
		JNIScreenSpaceCameraController(
			JNIEnv* jni,
			ScreenSpaceCameraController* camera_controller = nullptr);

		const char* getCanonicalName() const
		{
			return MAKE_CANONICAL_NAME(PACKAGE, ScreenSpaceCameraController);
		}

		void initialize(JNIEnv *env);
		void mapFields();

		static void dispose(
			JNIEnv *env,
			jobject java_this);

		static jobject create(
			JNIEnv* jni,
			ScreenSpaceCameraController* camera_controller);

		static jobject get_position_offset(
			JNIEnv *env,
			jobject java_this);

		static void set_translation_enabled(
			JNIEnv   *jni,
			jobject  java_this,
			jboolean enabled);

		static jboolean get_translation_enabled(
			JNIEnv   *jni,
			jobject  java_this);

		static void set_rotation_enabled(
			JNIEnv   *jni,
			jobject  java_this,
			jboolean enabled);

		static jboolean get_rotation_enabled(
			JNIEnv   *jni,
			jobject  java_this);

		static jdouble get_pitch_offset(
			JNIEnv   *jni,
			jobject  java_this);

		static jdouble get_yaw_offset(
			JNIEnv   *jni,
			jobject  java_this);

		static jdouble get_roll_offset(
			JNIEnv   *jni,
			jobject  java_this);

		static jdouble get_translation_speed(
			JNIEnv   *jni,
			jobject  java_this);

		static void set_translation_speed(
			JNIEnv   *jni,
			jobject  java_this,
			jdouble  speed);

		static jdouble get_rotation_speed(
			JNIEnv   *jni,
			jobject  java_this);

		static void set_rotation_speed(
			JNIEnv   *jni,
			jobject  java_this,
			jdouble  speed);
	};
}
