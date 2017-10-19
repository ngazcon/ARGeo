#pragma once

#include "JNIIGraphics.h"
#include "PolylineGraphics.h"

namespace argeo
{
	class JNIPolylineGraphics
		: public JNIIGraphics
	{
	public:
		JNIPolylineGraphics();
		JNIPolylineGraphics(
			JNIEnv* jni,
			PolylineGraphics* graphics = nullptr);

		const char* getCanonicalName() const
		{
			return MAKE_CANONICAL_NAME(PACKAGE, PolylineGraphics);
		}

		void initialize(JNIEnv *env);
		void mapFields();

		static void create3(
			JNIEnv* jni,
			jobject object,
			jobjectArray java_geocentric_array);

		static void create2(
			JNIEnv* jni,
			jobject object,
			jobjectArray java_geocentric_array,
			jboolean     loop);

		static void create(
			JNIEnv* jni,
			jobject object,
			jobjectArray java_geocentric_array,
			jboolean     loop,
			jdouble      width);

		static void add_point(
			JNIEnv* jni,
			jobject java_this,
			jobject java_geocentric);
	};
}
