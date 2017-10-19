#include "include/JNIIGraphics.h"
#include "include/JNIBillboardGraphics.h"
#include "include/JNIModelGraphics.h"
#include "include/JNIPolylineGraphics.h"

namespace argeo
{
	JNIIGraphics::JNIIGraphics(IGraphics* graphics)
		: NativeObjectWrapper(graphics)
	{ }

	JNIIGraphics::JNIIGraphics(
		JNIEnv* env,
		IGraphics* graphics)
		: NativeObjectWrapper(env, graphics)
	{ }

	JNIIGraphics::JNIIGraphics(
		JNIEnv* env)
		: NativeObjectWrapper(env)
	{ 
		initialize(env);
	}

	void JNIIGraphics::initialize(JNIEnv *env)
	{ 
		setClass(env);
	}

	void JNIIGraphics::mapFields()
	{ }

	void JNIIGraphics::dispose(JNIEnv *env, jobject java_this)
	{
		JNIIGraphics *object = gClasses.getNativeInstance<JNIIGraphics>(env, java_this);
		if (object != NULL)
		{
			object->destroy(env, java_this);
		}
	}

	jobject JNIIGraphics::create(
		JNIEnv* jni,
		IGraphics* graphics)
	{
		NativeObject* object = nullptr;

		if (dynamic_cast<BillboardGraphics*>(graphics) != nullptr)
		{
			object = gClasses.newInstance<JNIBillboardGraphics>(jni, nullptr);
			static_cast<JNIBillboardGraphics*>(object)->setNativeObject(graphics);
		}
		else if (dynamic_cast<ModelGraphics*>(graphics) != nullptr)
		{
			object = gClasses.newInstance<JNIModelGraphics>(jni, nullptr);
			static_cast<JNIModelGraphics*>(object)->setNativeObject(graphics);
		}
		else if (dynamic_cast<JNIPolylineGraphics*>(graphics) != nullptr)
		{
			object = gClasses.newInstance<JNIPolylineGraphics>(jni, nullptr);
			static_cast<JNIPolylineGraphics*>(object)->setNativeObject(graphics);
		}
		
		jobject java_this = object->toJavaObject(jni);
		object->persist(jni, java_this);
		return java_this;
	}
}