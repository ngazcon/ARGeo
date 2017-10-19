#include "include/JNIArgeoFragment.h"
#include "include/JNIViewer.h"

#include <android/native_window_jni.h>

namespace argeo
{
	JNIArgeoFragment::JNIArgeoFragment(ArgeoFragment* fragment)
		: NativeObjectWrapper(fragment)
	{ }

	JNIArgeoFragment::JNIArgeoFragment(
		JNIEnv *env,
		ArgeoFragment* fragment)
		: NativeObjectWrapper(env, fragment)
	{
		initialize(env);
	}

	void JNIArgeoFragment::initialize(JNIEnv *env)
	{
		setClass(env);

		cacheConstructor(env);

		addNativeMethod(
			"dispose",
			(void*)&JNIArgeoFragment::dispose,
			kTypeVoid,
			NULL);

		addNativeMethod(
			"initialize",
			(void*)&JNIArgeoFragment::java_initialize,
			kTypeVoid,
			NULL);

		addNativeMethod(
			"onDrag",
			(void*)&JNIArgeoFragment::on_drag,
			kTypeBool,
			kTypeFloat,
			kTypeFloat,
			kTypeInt,
			kTypeLong,
			kTypeInt,
			NULL);

		addNativeMethod(
			"onScale",
			(void*)&JNIArgeoFragment::on_scale,
			kTypeBool,
			kTypeFloat,
			kTypeFloat,
			kTypeInt,
			kTypeLong,
			kTypeFloat,
			NULL);

		addNativeMethod(
			"onRotate",
			(void*)&JNIArgeoFragment::on_rotate,
			kTypeBool,
			kTypeFloat,
			kTypeFloat,
			kTypeInt,
			kTypeLong,
			kTypeFloat,
			NULL);

		addNativeMethod(
			"onSurfaceCreated",
			(void*)&JNIArgeoFragment::on_surface_created,
			kTypeVoid,
			"Landroid/view/Surface;",
			NULL);

		addNativeMethod(
			"onSurfaceDestroyed",
			(void*)&JNIArgeoFragment::on_surface_destroyed,
			kTypeVoid,
			"Landroid/view/Surface;",
			NULL);

		addNativeMethod(
			"nativeGetViewer",
			(void*)&JNIArgeoFragment::get_viewer,
			MAKE_CANONICAL_NAME(PACKAGE, Viewer),
			NULL);

		registerNativeMethods(env);
	}

	void JNIArgeoFragment::mapFields()
	{ }

	void JNIArgeoFragment::dispose(JNIEnv *env, jobject java_this)
	{
		JNIArgeoFragment *object = gClasses.getNativeInstance<JNIArgeoFragment>(env, java_this);
		if (object != NULL)
		{
			object->destroy(env, java_this);
		}
	}

	void JNIArgeoFragment::java_initialize(JNIEnv* jni, jobject java_this)
	{
		JNIArgeoFragment* object = gClasses.newInstance<JNIArgeoFragment>(jni, java_this);
		object->setNativeObject(new ArgeoFragment());
		object->persist(jni, java_this);
	}

	jboolean JNIArgeoFragment::on_rotate(
		JNIEnv* env,
		jobject java_this,
		jfloat x,
		jfloat y,
		jint   action,
		jlong  event_time,
		jfloat radians)
	{
		JNIArgeoFragment* object = gClasses.getNativeInstance<JNIArgeoFragment>(env, java_this);
		return object->getNativeObject()->on_rotate(
			TouchEvent(
				Point(x, y),
				static_cast<Action>(action),
				event_time),
			radians);
	}

	jboolean JNIArgeoFragment::on_scale(
		JNIEnv* env,
		jobject java_this,
		jfloat x,
		jfloat y,
		jint   action,
		jlong  event_time,
		jfloat factor)
	{
		JNIArgeoFragment* object = gClasses.getNativeInstance<JNIArgeoFragment>(env, java_this);
		return object->getNativeObject()->on_scale(
			TouchEvent(
				Point(x, y),
				static_cast<Action>(action),
				event_time),
			factor);
	}

	jboolean JNIArgeoFragment::on_drag(
		JNIEnv* env,
		jobject java_this,
		jfloat x,
		jfloat y,
		jint   action,
		jlong  event_time,
		jint   drag_state)
	{
		JNIArgeoFragment* object = gClasses.getNativeInstance<JNIArgeoFragment>(env, java_this);
		return object->getNativeObject()->on_drag(
			TouchEvent(
				Point(x, y),
				static_cast<Action>(action),
				event_time),
			static_cast<DragState>(drag_state));
	}

	void JNIArgeoFragment::on_surface_created(
		JNIEnv* env,
		jobject java_this,
		jobject surface)
	{
		JNIArgeoFragment* object = gClasses.getNativeInstance<JNIArgeoFragment>(env, java_this);
		object->getNativeObject()->on_surface_created(
			ANativeWindow_fromSurface(env, surface));
	}

	void JNIArgeoFragment::on_surface_destroyed(
		JNIEnv* env,
		jobject java_this,
		jobject surface)
	{
		JNIArgeoFragment* object = gClasses.getNativeInstance<JNIArgeoFragment>(env, java_this);
		object->getNativeObject()->on_surface_destroyed();
	}

	jobject JNIArgeoFragment::get_viewer(
		JNIEnv* jni,
		jobject java_this)
	{
		JNIArgeoFragment* object = gClasses.getNativeInstance<JNIArgeoFragment>(jni, java_this);
		return JNIViewer::create(jni, object->getNativeObject()->get_viewer());
	}
}