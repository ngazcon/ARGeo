#include "include/JNIGeodetic3D.h"

namespace argeo
{
	JNIGeodetic3D::JNIGeodetic3D(Geodetic3D* geodetic)
		: NativeObjectWrapper(geodetic)
	{ }

	JNIGeodetic3D::JNIGeodetic3D(
		JNIEnv *env,
		Geodetic3D* geodetic)
		: NativeObjectWrapper(env, geodetic)
	{
		initialize(env);
	}

	void JNIGeodetic3D::initialize(JNIEnv *env)
	{
		setClass(env);

		cacheConstructor(env);

		addNativeMethod(
			"dispose",
			(void*)&JNIGeodetic3D::dispose,
			kTypeVoid,
			NULL);

		addNativeMethod(
			"initialize",
			(void*)&JNIGeodetic3D::java_initialize,
			kTypeVoid,
			kTypeDouble,
			kTypeDouble,
			kTypeDouble,
			NULL);


		addNativeMethod(
			"getLatitude",
			(void*)&JNIGeodetic3D::get_latitude,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"getLongitude",
			(void*)&JNIGeodetic3D::get_longitude,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"getAltitude",
			(void*)&JNIGeodetic3D::get_altitude,
			kTypeDouble,
			NULL);

		registerNativeMethods(env);
	}

	void JNIGeodetic3D::mapFields()
	{ }

	void JNIGeodetic3D::dispose(JNIEnv *env, jobject java_this)
	{
		JNIGeodetic3D *object = gClasses.getNativeInstance<JNIGeodetic3D>(env, java_this);
		if (object != NULL)
		{
			object->destroy(env, java_this);
		}
	}

	void JNIGeodetic3D::java_initialize(
		JNIEnv* env,
		jobject java_this,
		jdouble latitude,
		jdouble longitude,
		jdouble altitude)
	{
		JNIGeodetic3D* object = gClasses.newInstance<JNIGeodetic3D>(env, java_this);
		object->setNativeObject(new Geodetic3D(
			ArgeoMath::to_radians(longitude),
			ArgeoMath::to_radians(latitude),
			altitude));
		object->persist(env, java_this);
	}

	jobject JNIGeodetic3D::create(
		JNIEnv* jni,
		const Geodetic3D& geodetic)
	{
		JNIGeodetic3D* object = gClasses.newInstance<JNIGeodetic3D>(jni, nullptr);
		object->setNativeObject(new Geodetic3D(geodetic));

		jobject java_this = object->toJavaObject(jni);
		object->persist(jni, java_this);

		return java_this;
	}


	jdouble JNIGeodetic3D::get_longitude(
		JNIEnv* env,
		jobject java_this)
	{
		JNIGeodetic3D* object = gClasses.getNativeInstance<JNIGeodetic3D>(env, java_this);
		return ArgeoMath::to_degrees(object->getNativeObject()->get_longitude());
	}


	jdouble JNIGeodetic3D::get_latitude(
		JNIEnv* env,
		jobject java_this)
	{
		JNIGeodetic3D* object = gClasses.getNativeInstance<JNIGeodetic3D>(env, java_this);
		return ArgeoMath::to_degrees(object->getNativeObject()->get_latitude());
	}

	jdouble JNIGeodetic3D::get_altitude(
		JNIEnv* env,
		jobject java_this)
	{
		JNIGeodetic3D* object = gClasses.getNativeInstance<JNIGeodetic3D>(env, java_this);
		return object->getNativeObject()->get_height();
	}
}