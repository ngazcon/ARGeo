#include "include/JNIGeocentric3D.h"

namespace argeo
{
	JNIGeocentric3D::JNIGeocentric3D(Geocentric3D* geocentric)
		: NativeObjectWrapper(geocentric)
	{ }

	JNIGeocentric3D::JNIGeocentric3D(
		JNIEnv *env,
		Geocentric3D* geocentric)
		: NativeObjectWrapper(env, geocentric)
	{
		initialize(env);
	}

	void JNIGeocentric3D::initialize(JNIEnv *env)
	{
		setClass(env);

		cacheConstructor(env);

		addNativeMethod(
			"dispose",
			(void*)&JNIGeocentric3D::dispose,
			kTypeVoid,
			NULL);

		addNativeMethod(
			"initialize",
			(void*)&JNIGeocentric3D::java_initialize,
			kTypeVoid,
			kTypeDouble,
			kTypeDouble,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"getX",
			(void*)&JNIGeocentric3D::get_x,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"getY",
			(void*)&JNIGeocentric3D::get_y,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"getZ",
			(void*)&JNIGeocentric3D::get_z,
			kTypeDouble,
			NULL);

		registerNativeMethods(env);
	}

	void JNIGeocentric3D::mapFields()
	{ }

	void JNIGeocentric3D::dispose(JNIEnv *env, jobject java_this)
	{
		JNIGeocentric3D *object = gClasses.getNativeInstance<JNIGeocentric3D>(env, java_this);
		if (object != NULL)
		{
			object->destroy(env, java_this);
		}
	}

	jobject JNIGeocentric3D::create(
		JNIEnv* jni,
		const Geocentric3D& geocentric)
	{
		JNIGeocentric3D* object = gClasses.newInstance<JNIGeocentric3D>(jni, nullptr);
		object->setNativeObject(new Geocentric3D(geocentric));
		jobject java_this = object->toJavaObject(jni);
		object->persist(jni, java_this);
		return java_this;
	}

	void JNIGeocentric3D::java_initialize(
		JNIEnv* env,
		jobject java_this,
		jdouble x,
		jdouble y,
		jdouble z)
	{
		JNIGeocentric3D* object = gClasses.newInstance<JNIGeocentric3D>(env, java_this);
		object->setNativeObject(new Geocentric3D(x, y, z));
		object->persist(env, java_this);
	}

	jdouble JNIGeocentric3D::get_x(
		JNIEnv* env,
		jobject java_this)
	{
		JNIGeocentric3D* object = gClasses.getNativeInstance<JNIGeocentric3D>(env, java_this);
		return object->getNativeObject()->get_x();
	}


	jdouble JNIGeocentric3D::get_y(
		JNIEnv* env,
		jobject java_this)
	{
		JNIGeocentric3D* object = gClasses.getNativeInstance<JNIGeocentric3D>(env, java_this);
		return object->getNativeObject()->get_y();
	}

	jdouble JNIGeocentric3D::get_z(
		JNIEnv* env,
		jobject java_this)
	{
		JNIGeocentric3D* object = gClasses.getNativeInstance<JNIGeocentric3D>(env, java_this);
		return object->getNativeObject()->get_z();
	}
}