#include "include/JNIEllipsoidTangentPlane.h"
#include "include/JNIGeodetic3D.h"
#include "include/JNIGeocentric3D.h"

namespace argeo
{
	JNIEllipsoidTangentPlane::JNIEllipsoidTangentPlane(EllipsoidTangentPlane* ellipsoid_tangent_plane)
		: NativeObjectWrapper()
	{ }

	JNIEllipsoidTangentPlane::JNIEllipsoidTangentPlane(
		JNIEnv *env,
		EllipsoidTangentPlane* ellipsoid_tangent_plane)
		: NativeObjectWrapper(env, ellipsoid_tangent_plane)
	{
		initialize(env);
	}

	void JNIEllipsoidTangentPlane::initialize(JNIEnv *env)
	{
		setClass(env);

		cacheConstructor(env);

		addNativeMethod(
			"dispose",
			(void*)&JNIEllipsoidTangentPlane::dispose,
			kTypeVoid,
			NULL);

		addNativeMethod(
			"initialize",
			(void*)&JNIEllipsoidTangentPlane::java_initialize,
			kTypeVoid,
			MAKE_CANONICAL_NAME(PACKAGE, Geodetic3D),
			NULL);

		addNativeMethod(
			"nativeGetOrigin",
			(void*)&JNIEllipsoidTangentPlane::get_origin,
			MAKE_CANONICAL_NAME(PACKAGE, Geocentric3D),
			NULL);

		addNativeMethod(
			"nativeGetNormal",
			(void*)&JNIEllipsoidTangentPlane::get_normal,
			MAKE_CANONICAL_NAME(PACKAGE, Geocentric3D),
			NULL);

		addNativeMethod(
			"nativeGetXAxis",
			(void*)&JNIEllipsoidTangentPlane::get_x_axis,
			MAKE_CANONICAL_NAME(PACKAGE, Geocentric3D),
			NULL);

		addNativeMethod(
			"nativeGetYAxis",
			(void*)&JNIEllipsoidTangentPlane::get_y_axis,
			MAKE_CANONICAL_NAME(PACKAGE, Geocentric3D),
			NULL);

		registerNativeMethods(env);
	}

	void JNIEllipsoidTangentPlane::mapFields()
	{ }

	void JNIEllipsoidTangentPlane::dispose(JNIEnv *env, jobject java_this)
	{
		JNIEllipsoidTangentPlane *object = gClasses.getNativeInstance<JNIEllipsoidTangentPlane>(env, java_this);
		if (object != NULL)
		{
			object->destroy(env, java_this);
		}
	}

	void JNIEllipsoidTangentPlane::java_initialize(
		JNIEnv* jni,
		jobject java_this,
		jobject java_geodetic)
	{
		JNIEllipsoidTangentPlane* object = gClasses.newInstance<JNIEllipsoidTangentPlane>(jni, java_this);
		JNIGeodetic3D* geodetic          = gClasses.getNativeInstance<JNIGeodetic3D>(jni, java_geodetic);
		object->setNativeObject(new EllipsoidTangentPlane(*geodetic->getNativeObject()));
		object->persist(jni, java_this);
	}

	jobject JNIEllipsoidTangentPlane::get_origin(
		JNIEnv* jni,
		jobject java_this)
	{
		JNIEllipsoidTangentPlane* object = gClasses.getNativeInstance<JNIEllipsoidTangentPlane>(jni, java_this);
		return JNIGeocentric3D::create(jni, object->getNativeObject()->get_origin());
	}

	jobject JNIEllipsoidTangentPlane::get_normal(
		JNIEnv* jni,
		jobject java_this)
	{
		JNIEllipsoidTangentPlane* object = gClasses.getNativeInstance<JNIEllipsoidTangentPlane>(jni, java_this);
		return JNIGeocentric3D::create(jni, object->getNativeObject()->get_normal());
	}

	jobject JNIEllipsoidTangentPlane::get_x_axis(
		JNIEnv* jni,
		jobject java_this)
	{
		JNIEllipsoidTangentPlane* object = gClasses.getNativeInstance<JNIEllipsoidTangentPlane>(jni, java_this);
		return JNIGeocentric3D::create(jni, object->getNativeObject()->get_x_axis());
	}


	jobject JNIEllipsoidTangentPlane::get_y_axis(
		JNIEnv* jni,
		jobject java_this)
	{
		JNIEllipsoidTangentPlane* object = gClasses.getNativeInstance<JNIEllipsoidTangentPlane>(jni, java_this);
		return JNIGeocentric3D::create(jni, object->getNativeObject()->get_y_axis());
	}
}