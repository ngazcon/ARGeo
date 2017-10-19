#pragma once

#include "TouchEvent.h"
#include "Viewer.h"
#include "NativeObjectWrapper.h"
#include "ArgeoFragment.h"
#include "JniHelpers.h"
#include "Bootstrap.h"

namespace argeo
{
	namespace jni
	{
		class JNIArgeoFragment
			: public NativeObjectWrapper<ArgeoFragment>
		{
		public:
			JNIArgeoFragment(ArgeoFragment* fragment = nullptr);
			JNIArgeoFragment(
				JNIEnv* jni,
				ArgeoFragment* fragment = nullptr);

			const char* getCanonicalName() const
			{
				return MAKE_CANONICAL_NAME(PACKAGE, ArgeoFragment);
			}

			void initialize(JNIEnv *env);
			void mapFields();

			static void java_initialize(
				JNIEnv* env,
				jobject java_this);

			static void dispose(
				JNIEnv *env,
				jobject java_this);

			static jboolean on_drag(
				JNIEnv* jni,
				jobject java_this,
				jfloat x,
				jfloat y,
				jint   action,
				jlong  event_time,
				jint   drag_state);

			static jboolean on_rotate(
				JNIEnv* jni,
				jobject java_this,
				jfloat x,
				jfloat y,
				jint   action,
				jlong  event_time,
				jfloat radians);

			static jboolean on_scale(
				JNIEnv* jni,
				jobject java_this,
				jfloat x,
				jfloat y,
				jint   action,
				jlong  event_time,
				jfloat factor);

			static void on_surface_created(
				JNIEnv* jni,
				jobject object,
				jobject surface);

			static void on_surface_destroyed(
				JNIEnv* jni,
				jobject java_this,
				jobject surface);

			static jobject get_viewer(
				JNIEnv* jni,
				jobject java_this);
		};
	}
}
