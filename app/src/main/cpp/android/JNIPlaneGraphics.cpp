#include "include/NativeObject.h"
#include "EllipsoidTransformations.h"

#include "include/JNIPlaneGraphics.h"
#include "include/JNIEllipsoidTangentPlane.h"

namespace argeo
{
	JNIPlaneGraphics::JNIPlaneGraphics()
		: JNIIGraphics()
	{ }

	JNIPlaneGraphics::JNIPlaneGraphics(
		JNIEnv *env,
		PlaneGraphics* graphics)
		: JNIIGraphics(env, graphics)
	{
		initialize(env);
	}

	void JNIPlaneGraphics::java_initialize(
		JNIEnv* jni,
		jobject java_this,
		jobject java_ellipsoid_tangent_plane,
		jboolean flag)
	{
		JNIPlaneGraphics* object = gClasses.newInstance<JNIPlaneGraphics>(jni, java_this);
		JNIEllipsoidTangentPlane* tangent_plane = gClasses.getNativeInstance<JNIEllipsoidTangentPlane>(jni, java_ellipsoid_tangent_plane);
		object->setNativeObject(new PlaneGraphics(*tangent_plane->getNativeObject(), flag));
		object->persist(jni, java_this);
	}

	void JNIPlaneGraphics::initialize(JNIEnv* env)
	{
		setClass(env);

		cacheConstructor(env);

		addNativeMethod(
			"dispose",
			(void*)&JNIPlaneGraphics::dispose,
			kTypeVoid,
			NULL);

		addNativeMethod(
			"initialize",
			(void*)&JNIPlaneGraphics::java_initialize,
			kTypeVoid,
			MAKE_CANONICAL_NAME(PACKAGE, EllipsoidTangentPlane),
			kTypeBool,
			NULL);
		
		addNativeMethod(
			"setWidth",
			(void*)&JNIPlaneGraphics::set_width,
			kTypeVoid,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"getWidth",
			(void*)&JNIPlaneGraphics::get_width,
			kTypeDouble,
			NULL);

		registerNativeMethods(env);
	}

	void JNIPlaneGraphics::mapFields()
	{
		JNIIGraphics::mapFields();
	}

	void JNIPlaneGraphics::set_width(
		JNIEnv* jni,
		jobject java_this,
		jdouble width)
	{
		JNIPlaneGraphics* object = gClasses.getNativeInstance<JNIPlaneGraphics>(jni, java_this);
		static_cast<PlaneGraphics*>(object->getNativeObject())->set_width(width);
	}

	jdouble JNIPlaneGraphics::get_width(
		JNIEnv* jni,
		jobject java_this)
	{
		JNIPlaneGraphics* object = gClasses.getNativeInstance<JNIPlaneGraphics>(jni, java_this);
		return static_cast<PlaneGraphics*>(object->getNativeObject())->get_width();
	}
}
