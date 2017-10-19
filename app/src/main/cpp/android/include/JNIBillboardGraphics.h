#pragma once

#include "JNIIGraphics.h"
#include "BillboardGraphics.h"

namespace argeo
{
	class JNIBillboardGraphics
		: public JNIIGraphics
	{
	public:
		JNIBillboardGraphics();
		JNIBillboardGraphics(
			JNIEnv* jni,
			BillboardGraphics* graphics = nullptr);

		const char* getCanonicalName() const
		{
			return MAKE_CANONICAL_NAME(PACKAGE, BillboardGraphics);
		}

		void initialize(JNIEnv *env);
		void mapFields();

		static void java_initialize3(
			JNIEnv* jni,
			jobject object,
			jstring image_path);

		static void java_initialize2(
			JNIEnv* jni,
			jobject object,
			jstring image_path,
			jstring image_id   = jstring());

		static void java_initialize(
			JNIEnv* jni,
			jobject object,
			jstring image_path,
			jstring image_id   = jstring(),
			jint   width       = Billboard::IMAGE_WIDTH,
			jint   height	   = Billboard::IMAGE_HEIGHT,
		    jint   height_clamping = 0);

		static void set_show(
			JNIEnv*  jni,
			jobject  object,
			jboolean show);

		static void set_image_path(
			JNIEnv* jni,
			jobject object,
			jstring path);

		static void set_height(
			JNIEnv* jni,
			jobject object,
			jint height);

		static void set_width(
			JNIEnv* jni,
			jobject object,
			jint width);

		static jboolean get_show(
			JNIEnv* jni,
			jobject object);

		static jstring get_image_path(
			JNIEnv* jni,
			jobject object);

		static jstring get_image_id(
			JNIEnv* jni,
			jobject object);

		static jint get_height(
			JNIEnv* jni,
			jobject object);

		static jint get_width(
			JNIEnv* jni,
			jobject object);

	};
}
