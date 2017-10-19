#include "include/JNIArgeoActivity.h"
#include "include/JNIHelper.h"
#include "include/JNIViewer.h"

#include <android/native_window_jni.h>

namespace argeo
{
	JNIArgeoActivity::JNIArgeoActivity(ArgeoActivity* activity)
		: NativeObjectWrapper(activity)
	{ }

	JNIArgeoActivity::JNIArgeoActivity(
		JNIEnv *env,
		ArgeoActivity* activity)
		: NativeObjectWrapper(env, activity)
	{
		initialize(env);
	}

	void JNIArgeoActivity::initialize(JNIEnv *env)
	{
		setClass(env);

		cacheConstructor(env);

		addNativeMethod(
			"dispose", 
			(void*)&JNIArgeoActivity::dispose,
			kTypeVoid,
			NULL);
		
		addNativeMethod(
			"initialize", 
			(void*)&JNIArgeoActivity::java_initialize,
			kTypeVoid, 
			NULL);

		addNativeMethod(
			"onDrag",
			(void*)&JNIArgeoActivity::on_drag, 
			kTypeBool,
			kTypeFloat,
			kTypeFloat,
			kTypeInt,
			kTypeLong,
			kTypeInt,
			NULL);

		addNativeMethod(
			"onScale",
			(void*)&JNIArgeoActivity::on_scale,
			kTypeBool,
			kTypeFloat,
			kTypeFloat,
			kTypeInt,
			kTypeLong,
			kTypeFloat,
			NULL);

		addNativeMethod(
			"onRotate",
			(void*)&JNIArgeoActivity::on_rotate,
			kTypeBool,
			kTypeFloat,
			kTypeFloat,
			kTypeInt,
			kTypeLong,
			kTypeFloat,
			NULL);

		addNativeMethod(
			"onSurfaceCreated",
			(void*)&JNIArgeoActivity::on_surface_created,
			kTypeVoid,
			"Landroid/view/Surface;",
			NULL);

		addNativeMethod(
			"onSurfaceDestroyed", 
			(void*)&JNIArgeoActivity::on_surface_destroyed, 
			kTypeVoid, 
			"Landroid/view/Surface;",
			NULL);

		addNativeMethod(
			"nativeGetViewer",
			(void*)&JNIArgeoActivity::get_viewer,
			MAKE_CANONICAL_NAME(PACKAGE, Viewer),
			NULL);

		registerNativeMethods(env);
	}

	void JNIArgeoActivity::mapFields()
	{ }

	void JNIArgeoActivity::dispose(JNIEnv *env, jobject java_this)
	{
		JNIArgeoActivity *object = gClasses.getNativeInstance<JNIArgeoActivity>(env, java_this);
		if (object != NULL)
		{
			object->destroy(env, java_this);
		}
	}

	void JNIArgeoActivity::java_initialize(JNIEnv* jni, jobject java_this)
	{
		JNIArgeoActivity* object = gClasses.newInstance<JNIArgeoActivity>(jni, java_this);
		object->setNativeObject(new ArgeoActivity());
		object->persist(jni, java_this);
	}

	jboolean JNIArgeoActivity::on_rotate(
		JNIEnv* env,
		jobject java_this,
		jfloat x,
		jfloat y,
		jint   action,
		jlong  event_time,
		jfloat radians)
	{
		JNIArgeoActivity* object = gClasses.getNativeInstance<JNIArgeoActivity>(env, java_this);
		return object->getNativeObject()->on_rotate(
			TouchEvent(
				Point(x, y),
				static_cast<Action>(action),
				event_time),
			radians);
	}

	jboolean JNIArgeoActivity::on_scale(
		JNIEnv* env,
		jobject java_this,
		jfloat x,
		jfloat y,
		jint   action,
		jlong  event_time,
		jfloat factor)
	{
		JNIArgeoActivity* object = gClasses.getNativeInstance<JNIArgeoActivity>(env, java_this);
		return object->getNativeObject()->on_scale(
			TouchEvent(
				Point(x, y),
				static_cast<Action>(action),
				event_time),
			factor);
	}

	jboolean JNIArgeoActivity::on_drag(
		JNIEnv* env,
		jobject java_this,
		jfloat x,
		jfloat y,
		jint   action,
		jlong  event_time,
		jint   drag_state)
	{
		JNIArgeoActivity* object = gClasses.getNativeInstance<JNIArgeoActivity>(env, java_this);
		return object->getNativeObject()->on_drag(
			TouchEvent(
				Point(x, y),
				static_cast<Action>(action),
				event_time),
			static_cast<DragState>(drag_state));
	}

	void JNIArgeoActivity::on_surface_created(
		JNIEnv* env,
		jobject java_this,
		jobject surface)
	{
		JNIArgeoActivity* object = gClasses.getNativeInstance<JNIArgeoActivity>(env, java_this);
		object->getNativeObject()->on_surface_created(
			ANativeWindow_fromSurface(env, surface));
	}

	void JNIArgeoActivity::on_surface_destroyed(
		JNIEnv* env,
		jobject java_this,
		jobject surface)
	{
		JNIArgeoActivity* object = gClasses.getNativeInstance<JNIArgeoActivity>(env, java_this);
		object->getNativeObject()->on_surface_destroyed();
	}

	jobject JNIArgeoActivity::get_viewer(
		JNIEnv* jni,
		jobject java_this)
	{
		JNIArgeoActivity* object = gClasses.getNativeInstance<JNIArgeoActivity>(jni, java_this);
		return JNIViewer::create(jni, object->getNativeObject()->get_viewer());
	}
}