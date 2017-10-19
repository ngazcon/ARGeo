#include "include/JNIScreenSpaceCameraController.h"
#include "include/JNIGeodetic3D.h"

namespace argeo
{
	JNIScreenSpaceCameraController::JNIScreenSpaceCameraController(ScreenSpaceCameraController* camera_controller)
		: NativeObjectWrapper()
	{ }

	JNIScreenSpaceCameraController::JNIScreenSpaceCameraController(
		JNIEnv *env,
		ScreenSpaceCameraController* camera_controller)
		: NativeObjectWrapper(env, camera_controller)
	{
		initialize(env);
	}

	void JNIScreenSpaceCameraController::initialize(JNIEnv *env)
	{
		setClass(env);

		cacheConstructor(env);

		addNativeMethod(
			"dispose",
			(void*)&JNIScreenSpaceCameraController::dispose,
			kTypeVoid,
			NULL);

		addNativeMethod(
			"setTranslationEnabled",
			(void*)&JNIScreenSpaceCameraController::set_translation_enabled,
			kTypeVoid,
			kTypeBool,
			NULL);

		addNativeMethod(
			"setRotationEnabled",
			(void*)&JNIScreenSpaceCameraController::set_rotation_enabled,
			kTypeVoid,
			kTypeBool,
			NULL);

		addNativeMethod(
			"getPositionOffset",
			(void*)&JNIScreenSpaceCameraController::get_position_offset,
			MAKE_CANONICAL_NAME(PACKAGE, Geodetic3D),
			NULL);

		addNativeMethod(
			"getPitchOffset",
			(void*)&JNIScreenSpaceCameraController::get_pitch_offset,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"getYawOffset",
			(void*)&JNIScreenSpaceCameraController::get_yaw_offset,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"getRollOffset",
			(void*)&JNIScreenSpaceCameraController::get_roll_offset,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"getTranslationSpeed",
			(void*)&JNIScreenSpaceCameraController::get_translation_speed,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"setTranslationSpeed",
			(void*)&JNIScreenSpaceCameraController::set_translation_speed,
			kTypeVoid,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"getRotationSpeed",
			(void*)&JNIScreenSpaceCameraController::get_rotation_speed,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"setRotationSpeed",
			(void*)&JNIScreenSpaceCameraController::set_rotation_speed,
			kTypeVoid,
			kTypeDouble,
			NULL);

		registerNativeMethods(env);
	}

	void JNIScreenSpaceCameraController::mapFields()
	{ }

	void JNIScreenSpaceCameraController::dispose(JNIEnv *env, jobject java_this)
	{
		JNIScreenSpaceCameraController *object = gClasses.getNativeInstance<JNIScreenSpaceCameraController>(env, java_this);
		if (object != NULL)
		{
			object->destroy(env, java_this);
		}
	}

	jobject JNIScreenSpaceCameraController::create(
		JNIEnv* jni,
		ScreenSpaceCameraController* camera_controller)
	{
		JNIScreenSpaceCameraController* object = gClasses.newInstance<JNIScreenSpaceCameraController>(jni, nullptr);
		object->setNativeObject(camera_controller);
		jobject java_this = object->toJavaObject(jni);
		object->persist(jni, java_this);
		return java_this;
	}

	jobject JNIScreenSpaceCameraController::get_position_offset(
		JNIEnv *jni,
		jobject java_this)
	{
		JNIScreenSpaceCameraController* object = gClasses.getNativeInstance<JNIScreenSpaceCameraController>(jni, java_this);
		return JNIGeodetic3D::create(jni, object->getNativeObject()->get_position_offset());
	}

	void JNIScreenSpaceCameraController::set_translation_enabled(
		JNIEnv   *jni,
		jobject  java_this,
		jboolean enabled)
	{
		JNIScreenSpaceCameraController* object = gClasses.getNativeInstance<JNIScreenSpaceCameraController>(jni, java_this);
		object->getNativeObject()->set_translation_enabled(enabled);
	}

	jboolean JNIScreenSpaceCameraController::get_translation_enabled(
		JNIEnv   *jni,
		jobject  java_this)
	{
		JNIScreenSpaceCameraController* object = gClasses.getNativeInstance<JNIScreenSpaceCameraController>(jni, java_this);
		return true;
	}

	void JNIScreenSpaceCameraController::set_rotation_enabled(
		JNIEnv   *jni,
		jobject  java_this,
		jboolean enabled)
	{
		JNIScreenSpaceCameraController* object = gClasses.getNativeInstance<JNIScreenSpaceCameraController>(jni, java_this);
		object->getNativeObject()->set_rotation_enabled(enabled);
	}

	jboolean JNIScreenSpaceCameraController::get_rotation_enabled(
		JNIEnv   *jni,
		jobject  java_this)
	{
		JNIScreenSpaceCameraController* object = gClasses.getNativeInstance<JNIScreenSpaceCameraController>(jni, java_this);
		return true;
	}

	jdouble JNIScreenSpaceCameraController::get_yaw_offset(
		JNIEnv   *jni,
		jobject  java_this)
	{
		JNIScreenSpaceCameraController* object = gClasses.getNativeInstance<JNIScreenSpaceCameraController>(jni, java_this);
		return object->getNativeObject()->get_yaw_offset();
	}

	jdouble JNIScreenSpaceCameraController::get_pitch_offset(
		JNIEnv   *jni,
		jobject  java_this)
	{
		JNIScreenSpaceCameraController* object = gClasses.getNativeInstance<JNIScreenSpaceCameraController>(jni, java_this);
		return object->getNativeObject()->get_pitch_offset();
	}

	jdouble JNIScreenSpaceCameraController::get_roll_offset(
		JNIEnv   *jni,
		jobject  java_this)
	{
		JNIScreenSpaceCameraController* object = gClasses.getNativeInstance<JNIScreenSpaceCameraController>(jni, java_this);
		return object->getNativeObject()->get_roll_offset();
	}

	jdouble JNIScreenSpaceCameraController::get_translation_speed(
		JNIEnv   *jni,
		jobject  java_this)
	{
		JNIScreenSpaceCameraController* object = gClasses.getNativeInstance<JNIScreenSpaceCameraController>(jni, java_this);
		return object->getNativeObject()->get_translation_speed();
	}

	void JNIScreenSpaceCameraController::set_translation_speed(
		JNIEnv   *jni,
		jobject  java_this,
		jdouble  speed)
	{
		JNIScreenSpaceCameraController* object = gClasses.getNativeInstance<JNIScreenSpaceCameraController>(jni, java_this);
		object->getNativeObject()->set_translation_speed(speed);
	}

	jdouble JNIScreenSpaceCameraController::get_rotation_speed(
		JNIEnv   *jni,
		jobject  java_this)
	{
		JNIScreenSpaceCameraController* object = gClasses.getNativeInstance<JNIScreenSpaceCameraController>(jni, java_this);
		return object->getNativeObject()->get_rotation_speed();
	}

	void JNIScreenSpaceCameraController::set_rotation_speed(
		JNIEnv   *jni,
		jobject  java_this,
		jdouble  speed)
	{
		JNIScreenSpaceCameraController* object = gClasses.getNativeInstance<JNIScreenSpaceCameraController>(jni, java_this);
		object->getNativeObject()->set_rotation_speed(speed);
	}
}