#pragma once

#include "JNIIGraphics.h"
#include "ModelGraphics.h"

namespace argeo
{
	namespace jni
	{
		class JNIModelGraphics
			: public JNIIGraphics
		{
		public:
			JNIModelGraphics();
			JNIModelGraphics(
				JNIEnv* jni,
				ModelGraphics* graphics = nullptr);

			const char* getCanonicalName() const
			{
				return MAKE_CANONICAL_NAME(PACKAGE, ModelGraphics);
			}

			void initialize(JNIEnv *env);
			void mapFields();

			static void java_initialize(
				JNIEnv* jni,
				jobject java_this,
				jstring path);

			static void java_initialize2(
				JNIEnv* jni,
				jobject java_this,
				jstring path,
				jint    height_clamping,
				jint    orientation_clamping);

			static void set_height_clamping(
				JNIEnv* jni,
				jobject java_this,
				jint    clamping);

			static void set_orientation_clamping(
				JNIEnv* jni,
				jobject java_this,
				jint    clamping);
		};
	}
}