#pragma once

#include "JniHelpers.h"
#include "Bootstrap.h"
#include "NativeObjectWrapper.h"
#include "IGraphics.h"

#include <memory>

namespace argeo
{
	namespace jni
	{
		class JNIIGraphics
			: public NativeObjectWrapper<IGraphics>
		{
		public:
			JNIIGraphics(IGraphics* graphics = nullptr);
			JNIIGraphics(JNIEnv* jni);
			JNIIGraphics(
				JNIEnv* jni,
				IGraphics* graphics);

			const char* getCanonicalName() const
			{
				return MAKE_CANONICAL_NAME(PACKAGE, IGraphics);
			}

			virtual void initialize(JNIEnv *env);
			virtual void mapFields();

			static void dispose(
				JNIEnv *env,
				jobject java_this);

			static jobject create(
				JNIEnv* env,
				IGraphics* graphics);
		};
	}
}