#include "include/NativeObject.h"
#include "BillboardGraphics.h"

#include "include/JNIBillboardGraphics.h"

namespace argeo
{
	JNIBillboardGraphics::JNIBillboardGraphics()
		: JNIIGraphics()
	{ }

	JNIBillboardGraphics::JNIBillboardGraphics(
		JNIEnv *env,
		BillboardGraphics* graphics)
		: JNIIGraphics(env, graphics)
	{ 
		initialize(env);
	}

	void JNIBillboardGraphics::java_initialize3(
		JNIEnv* jni,
		jobject java_this,
		jstring image_path)
	{
		JavaString native_image_path(jni, image_path);
		JNIBillboardGraphics* object = gClasses.newInstance<JNIBillboardGraphics>(jni, java_this);
		object->setNativeObject(new BillboardGraphics(native_image_path.get()));
		object->persist(jni, java_this);
	}

	void JNIBillboardGraphics::java_initialize2(
		JNIEnv* jni,
		jobject java_this,
		jstring image_path,
		jstring image_id)
	{
		JavaString native_image_path(jni, image_path);
		JavaString native_image_id(jni, image_id);
		JNIBillboardGraphics* object = gClasses.newInstance<JNIBillboardGraphics>(jni, java_this);
		object->setNativeObject(new BillboardGraphics(
			native_image_path.get(),
			native_image_id.get()));
		object->persist(jni, java_this);
	}

	void JNIBillboardGraphics::java_initialize(
		JNIEnv* jni,
		jobject java_this,
		jstring image_path,
		jstring image_id,
		jint    width,
		jint    height,
		jint height_clamping)
	{
		JavaString native_image_path(jni, image_path);
		JavaString native_image_id(jni, image_id);

		JNIBillboardGraphics* object = gClasses.newInstance<JNIBillboardGraphics>(jni, java_this);
		object->setNativeObject(new BillboardGraphics(
			native_image_path.get(),
			native_image_id.get(),
			static_cast<unsigned int>(width),
			static_cast<unsigned int>(height),
            static_cast<HeightClamping>(height_clamping)));

		object->persist(jni, java_this);
	}

	void JNIBillboardGraphics::set_height(
		JNIEnv* jni,
		jobject java_this,
		jint height)
	{
		JNIBillboardGraphics* object = gClasses.getNativeInstance<JNIBillboardGraphics>(jni, java_this);
		static_cast<BillboardGraphics*>(object->getNativeObject())->set_height(height);
	}

	void JNIBillboardGraphics::set_width(
		JNIEnv* jni,
		jobject java_this,
		jint width)
	{
		JNIBillboardGraphics* object = gClasses.getNativeInstance<JNIBillboardGraphics>(jni, java_this);
		static_cast<BillboardGraphics*>(object->getNativeObject())->set_width(width);
	}

	void JNIBillboardGraphics::set_image_path(
		JNIEnv* jni,
		jobject java_this,
		jstring path)
	{
		JavaString path_string(jni, path);
		JNIBillboardGraphics* object = gClasses.getNativeInstance<JNIBillboardGraphics>(jni, java_this);
		static_cast<BillboardGraphics*>(object->getNativeObject())->set_image_path(path_string.get());
	}

	void JNIBillboardGraphics::set_show(
		JNIEnv*  jni,
		jobject  java_this,
		jboolean show)
	{
		JNIBillboardGraphics* object = gClasses.getNativeInstance<JNIBillboardGraphics>(jni, java_this);
		static_cast<BillboardGraphics*>(object->getNativeObject())->set_show(show);
	}

	jboolean JNIBillboardGraphics::get_show(
		JNIEnv* jni,
		jobject java_this)
	{
		JNIBillboardGraphics* object = gClasses.getNativeInstance<JNIBillboardGraphics>(jni, java_this);
		return static_cast<BillboardGraphics*>(object->getNativeObject())->get_show();
	}

	jstring JNIBillboardGraphics::get_image_path(
		JNIEnv* jni,
		jobject java_this)
	{
		JNIBillboardGraphics* object = gClasses.getNativeInstance<JNIBillboardGraphics>(jni, java_this);
		return JavaString(static_cast<BillboardGraphics*>(object->getNativeObject())->get_image_path()).toJavaString(jni).leak();
	}

	jstring JNIBillboardGraphics::get_image_id(
		JNIEnv* jni,
		jobject java_this)
	{
		JNIBillboardGraphics* object = gClasses.getNativeInstance<JNIBillboardGraphics>(jni, java_this);
		return JavaString(static_cast<BillboardGraphics*>(object->getNativeObject())->get_image_id()).toJavaString(jni).leak();
	}

	jint JNIBillboardGraphics::get_height(
		JNIEnv* jni,
		jobject java_this)
	{
		JNIBillboardGraphics* object = gClasses.getNativeInstance<JNIBillboardGraphics>(jni, java_this);
		return static_cast<BillboardGraphics*>(object->getNativeObject())->get_height();
	}

	jint JNIBillboardGraphics::get_width(
		JNIEnv* jni,
		jobject java_this)
	{
		JNIBillboardGraphics* object = gClasses.getNativeInstance<JNIBillboardGraphics>(jni, java_this);
		return static_cast<BillboardGraphics*>(object->getNativeObject())->get_width();
	}

	void JNIBillboardGraphics::initialize(JNIEnv* env)
	{
		setClass(env);

		cacheConstructor(env);

		addNativeMethod(
			"dispose",
			(void*)&JNIBillboardGraphics::dispose,
			kTypeVoid,
			NULL);

		addNativeMethod(
			"initialize",
			(void*)&JNIBillboardGraphics::java_initialize,
			kTypeVoid,
			kTypeString,
			kTypeString,
			kTypeInt,
			kTypeInt,
            kTypeInt,
			NULL);

		addNativeMethod(
			"initialize",
			(void*)&JNIBillboardGraphics::java_initialize2,
			kTypeVoid,
			kTypeString,
			kTypeString,
			NULL);

		addNativeMethod(
			"initialize",
			(void*)&JNIBillboardGraphics::java_initialize3,
			kTypeVoid,
			kTypeString,
			NULL);

		addNativeMethod(
			"setShow",
			(void*)&JNIBillboardGraphics::set_show,
			kTypeVoid,
			kTypeBool,
			NULL);

		addNativeMethod(
			"setImagePath",
			(void*)&JNIBillboardGraphics::set_image_path,
			kTypeVoid,
			kTypeString,
			NULL);

		addNativeMethod(
			"setWidth",
			(void*)&JNIBillboardGraphics::set_width,
			kTypeVoid,
			kTypeInt,
			NULL);

		addNativeMethod(
			"setHeight",
			(void*)&JNIBillboardGraphics::set_height,
			kTypeVoid,
			kTypeInt,
			NULL);

		addNativeMethod(
			"getWidth",
			(void*)&JNIBillboardGraphics::get_width,
			kTypeInt,
			NULL);

		addNativeMethod(
			"getHeight",
			(void*)&JNIBillboardGraphics::get_height,
			kTypeInt,
			NULL);

		addNativeMethod(
			"getImageId",
			(void*)&JNIBillboardGraphics::get_image_id,
			kTypeString,
			NULL);

		addNativeMethod(
			"getImagePath",
			(void*)&JNIBillboardGraphics::get_image_path,
			kTypeString,
			NULL);

		addNativeMethod(
			"getShow",
			(void*)&JNIBillboardGraphics::get_show,
			kTypeBool,
			NULL);

		registerNativeMethods(env);
	}

	void JNIBillboardGraphics::mapFields()
	{ 
		JNIIGraphics::mapFields();
	}
}
