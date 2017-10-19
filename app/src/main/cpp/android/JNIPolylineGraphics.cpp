#include "include/NativeObject.h"
#include "EllipsoidTransformations.h"

#include "include/JNIGeocentric3D.h"
#include "include/JNIPolylineGraphics.h"

namespace argeo
{
	JNIPolylineGraphics::JNIPolylineGraphics()
		: JNIIGraphics()
	{ }

	JNIPolylineGraphics::JNIPolylineGraphics(
		JNIEnv *env,
		PolylineGraphics* graphics)
		: JNIIGraphics(env, graphics)
	{
		initialize(env);
	}

	void JNIPolylineGraphics::create3(
		JNIEnv* jni,
		jobject java_this,
		jobjectArray java_geocentric_array)
	{
		JNIPolylineGraphics* object = gClasses.newInstance<JNIPolylineGraphics>(jni, java_this);
		
		jsize size = jni->GetArrayLength(java_geocentric_array);
		std::vector<vec3d> points(size);
		for (int i = 0; i < size; i++)
		{
			jobject java_geocentric = jni->GetObjectArrayElement(
				java_geocentric_array, 
				i);

			JNIGeocentric3D* geocentric =  gClasses.getNativeInstance<JNIGeocentric3D>(
				jni, 
				java_geocentric);
			
			points[i] = geocentric->getNativeObject()->to_vec3d();
		}

		object->setNativeObject(new PolylineGraphics(points));

		object->persist(jni, java_this);
	}

	void JNIPolylineGraphics::create2(
		JNIEnv* jni,
		jobject java_this,
		jobjectArray java_geocentric_array,
		jboolean     loop)
	{
		JNIPolylineGraphics* object = gClasses.newInstance<JNIPolylineGraphics>(jni, java_this);

		jsize size = jni->GetArrayLength(java_geocentric_array);
		std::vector<vec3d> points(size);
		for (int i = 0; i < size; i++)
		{
			jobject java_geocentric = jni->GetObjectArrayElement(
				java_geocentric_array,
				i);

			JNIGeocentric3D* geocentric = gClasses.getNativeInstance<JNIGeocentric3D>(
				jni,
				java_geocentric);

			points[i] = geocentric->getNativeObject()->to_vec3d();
		}

		object->setNativeObject(new PolylineGraphics(points, loop));

		object->persist(jni, java_this);
	}

	void JNIPolylineGraphics::create(
		JNIEnv* jni,
		jobject java_this,
		jobjectArray java_geocentric_array,
		jboolean     loop,
		jdouble		 width)
	{
		JNIPolylineGraphics* object = gClasses.newInstance<JNIPolylineGraphics>(jni, java_this);

		jsize size = jni->GetArrayLength(java_geocentric_array);
		std::vector<vec3d> points(size);
		for (int i = 0; i < size; i++)
		{
			jobject java_geocentric = jni->GetObjectArrayElement(
				java_geocentric_array,
				i);

			JNIGeocentric3D* geocentric = gClasses.getNativeInstance<JNIGeocentric3D>(
				jni,
				java_geocentric);

			points[i] = geocentric->getNativeObject()->to_vec3d();
		}

		object->setNativeObject(new PolylineGraphics(points, loop, width));

		object->persist(jni, java_this);
	}


	void JNIPolylineGraphics::add_point(
		JNIEnv* jni,
		jobject java_this,
		jobject java_geocentric)
	{
		JNIPolylineGraphics* object = gClasses.getNativeInstance<JNIPolylineGraphics>(jni, java_this);
		JNIGeocentric3D* geocentric = gClasses.getNativeInstance<JNIGeocentric3D>(jni, java_geocentric);

		static_cast<PolylineGraphics*>(object->getNativeObject())->add_point(
			geocentric->getNativeObject()->to_vec3d());
	}

	void JNIPolylineGraphics::initialize(JNIEnv* env)
	{
		setClass(env);

		cacheConstructor(env);

		addNativeMethod(
			"dispose",
			(void*)&JNIPolylineGraphics::dispose,
			kTypeVoid,
			NULL);

		addNativeMethod(
			"create",
			(void*)&JNIPolylineGraphics::create,
			kTypeVoid,
			kTypeObjectArray(MAKE_CANONICAL_NAME(PACKAGE, Geocentric3D)),
			kTypeBool,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"create",
			(void*)&JNIPolylineGraphics::create2,
			kTypeVoid,
			kTypeObjectArray(MAKE_CANONICAL_NAME(PACKAGE, Geocentric3D)),
			kTypeBool,
			NULL);

		addNativeMethod(
			"create",
			(void*)&JNIPolylineGraphics::create3,
			kTypeVoid,
			kTypeObjectArray(MAKE_CANONICAL_NAME(PACKAGE, Geocentric3D)),
			NULL);

		addNativeMethod(
			"addPoint",
			(void*)&JNIPolylineGraphics::add_point,
			kTypeVoid,
			MAKE_CANONICAL_NAME(PACKAGE, Geocentric3D),
			NULL);

		registerNativeMethods(env);
	}

	void JNIPolylineGraphics::mapFields()
	{
		JNIIGraphics::mapFields();
	}
}
