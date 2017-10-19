#include "include/JNIITerrain.h"

namespace argeo
{
	JNIITerrain::JNIITerrain(ITerrain* terrain)
		: NativeObjectWrapper(terrain)
	{ }

	JNIITerrain::JNIITerrain(
		JNIEnv* env,
		ITerrain* terrain)
		: NativeObjectWrapper(env, terrain)
	{
		initialize(env);
	}

	void JNIITerrain::initialize(JNIEnv *env)
	{
		setClass(env);

		cacheConstructor(env);

		addNativeMethod(
			"dispose",
			(void*)&JNIITerrain::dispose,
			kTypeVoid,
			NULL);

		addNativeMethod(
			"setWireframe",
			(void*)&JNIITerrain::set_wireframe,
			kTypeVoid,
			kTypeBool,
			NULL);

		addNativeMethod(
			"getShow",
			(void*)&JNIITerrain::get_show,
			kTypeBool,
			NULL);

		addNativeMethod(
			"setShow",
			(void*)&JNIITerrain::set_show,
			kTypeVoid,
			kTypeBool,
			NULL);

		addNativeMethod(
			"getLighting",
			(void*)&JNIITerrain::get_lighting,
			kTypeBool,
			NULL);

		addNativeMethod(
			"setLighting",
			(void*)&JNIITerrain::set_lighting,
			kTypeVoid,
			kTypeBool,
			NULL);

		addNativeMethod(
			"getWireframe",
			(void*)&JNIITerrain::get_wireframe,
			kTypeBool,
			NULL);
		addNativeMethod(
			"setHeightExaggeration",
			(void*)&JNIITerrain::set_height_exaggeration,
			kTypeVoid,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"getHeightExaggeration",
			(void*)&JNIITerrain::get_height_exaggeration,
			kTypeDouble,
			NULL);

		registerNativeMethods(env);
	}

	void JNIITerrain::mapFields()
	{ }

	void JNIITerrain::dispose(JNIEnv *env, jobject java_this)
	{
		JNIITerrain *object = gClasses.getNativeInstance<JNIITerrain>(env, java_this);
		if (object != NULL)
		{
			object->destroy(env, java_this);
		}
	}

	jobject JNIITerrain::create(
		JNIEnv* jni,
		ITerrain* terrain)
	{
		JNIITerrain* object = gClasses.newInstance<JNIITerrain>(jni, nullptr);
		object->setNativeObject(terrain);
		jobject java_this = object->toJavaObject(jni);
		object->persist(jni, java_this);
		return java_this;
	}

	void JNIITerrain::set_wireframe(
		JNIEnv* jni,
		jobject java_this,
		jboolean value)
	{
		JNIITerrain* object = gClasses.getNativeInstance<JNIITerrain>(jni, java_this);
		return object->getNativeObject()->set_wireframe(value);
	}

	jboolean JNIITerrain::get_wireframe(
		JNIEnv* jni,
		jobject java_this)
	{
		JNIITerrain* object = gClasses.getNativeInstance<JNIITerrain>(jni, java_this);
		return object->getNativeObject()->get_wireframe();
	}

	void JNIITerrain::set_height_exaggeration(
		JNIEnv* jni,
		jobject java_this,
		jdouble value)
	{
		JNIITerrain* object = gClasses.getNativeInstance<JNIITerrain>(jni, java_this);
		return object->getNativeObject()->set_height_exaggeration(value);
	}

	jdouble JNIITerrain::get_height_exaggeration(
		JNIEnv* jni,
		jobject java_this)
	{
		JNIITerrain* object = gClasses.getNativeInstance<JNIITerrain>(jni, java_this);
		return object->getNativeObject()->get_height_exaggeration();
	}

	void JNIITerrain::set_lighting(
		JNIEnv   *jni,
		jobject  java_this,
		jboolean value)
	{
		JNIITerrain* object = gClasses.getNativeInstance<JNIITerrain>(jni, java_this);
		object->getNativeObject()->set_lighting(value);
	}

	jboolean JNIITerrain::get_lighting(
		JNIEnv   *jni,
		jobject  java_this)
	{
		JNIITerrain* object = gClasses.getNativeInstance<JNIITerrain>(jni, java_this);
		return object->getNativeObject()->get_lighting();
	}

	void JNIITerrain::set_show(
		JNIEnv   *jni,
		jobject  java_this,
		jboolean value)
	{
		JNIITerrain* object = gClasses.getNativeInstance<JNIITerrain>(jni, java_this);
		object->getNativeObject()->set_show(value);
	}

	jboolean JNIITerrain::get_show(
		JNIEnv   *jni,
		jobject  java_this)
	{
		JNIITerrain* object = gClasses.getNativeInstance<JNIITerrain>(jni, java_this);
		return object->getNativeObject()->get_show();
	}
}