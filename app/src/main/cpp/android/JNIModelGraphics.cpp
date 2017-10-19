#include "EllipsoidTransformations.h"
#include "ModelGraphics.h"
#include "include/JavaString.h"

#include "include/JNIModelGraphics.h"

namespace argeo
{
	JNIModelGraphics::JNIModelGraphics()
		: JNIIGraphics()
	{ }

	JNIModelGraphics::JNIModelGraphics(
		JNIEnv *env,
		ModelGraphics* graphics)
		: JNIIGraphics(env, graphics)
	{
		initialize(env);
	}

	void JNIModelGraphics::initialize(JNIEnv* env)
	{
		setClass(env);

		cacheConstructor(env);

		addNativeMethod(
			"dispose",
			(void*)&JNIModelGraphics::dispose,
			kTypeVoid,
			NULL);

		addNativeMethod(
			"initialize",
			(void*)&JNIModelGraphics::java_initialize,
			kTypeVoid,
			kTypeString,
			NULL);

		addNativeMethod(
			"initialize",
			(void*)&JNIModelGraphics::java_initialize2,
			kTypeVoid,
			kTypeString,
			kTypeInt,
			kTypeInt,
			NULL);

		addNativeMethod(
			"setHeightClamping",
			(void*)&JNIModelGraphics::set_height_clamping,
			kTypeVoid,
			kTypeInt,
			NULL);

		addNativeMethod(
			"setOrientationClamping",
			(void*)&JNIModelGraphics::set_orientation_clamping,
			kTypeVoid,
			kTypeInt,
			NULL);

		registerNativeMethods(env);
	}

	void JNIModelGraphics::mapFields()
	{ 
		JNIIGraphics::mapFields();
	}

	void JNIModelGraphics::java_initialize(
		JNIEnv* jni,
		jobject java_this,
		jstring path)
	{
		JavaString native_path(jni, path);
		JNIModelGraphics* object = gClasses.newInstance<JNIModelGraphics>(jni, java_this);
		object->setNativeObject(new ModelGraphics(native_path.get()));
		object->persist(jni, java_this);
	}

	void JNIModelGraphics::java_initialize2(
		JNIEnv* jni,
		jobject java_this,
		jstring path,
		jint    height_clamping,
		jint    orientation_clamping)
	{
		JavaString native_path(jni, path);
		JNIModelGraphics* object = gClasses.newInstance<JNIModelGraphics>(jni, java_this);
		object->setNativeObject(new ModelGraphics(
			native_path.get(),
			static_cast<HeightClamping>(height_clamping),
			static_cast<OrientationClamping>(orientation_clamping)));
		object->persist(jni, java_this);
	}

	void JNIModelGraphics::set_height_clamping(
		JNIEnv* jni,
		jobject java_this,
		jint    clamping)
	{
		JNIModelGraphics* object = gClasses.getNativeInstance<JNIModelGraphics>(jni, java_this);
		dynamic_cast<ModelGraphics*>(object->getNativeObject())->set_height_clamping(
			static_cast<HeightClamping>(clamping));
	}

	void JNIModelGraphics::set_orientation_clamping(
		JNIEnv* jni,
		jobject java_this,
		jint    clamping)
	{
		JNIModelGraphics* object = gClasses.getNativeInstance<JNIModelGraphics>(jni, java_this);
		dynamic_cast<ModelGraphics*>(object->getNativeObject())->set_orientation_clamping(
			static_cast<OrientationClamping>(clamping));
	}
}
