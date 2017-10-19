#pragma once

#include "JniHelpers.h"

#include <memory>

namespace argeo
{
	namespace jni
	{
		template<class NativeType>
		class NativeObjectWrapper
			: public NativeObject
		{
		public:

			NativeObjectWrapper(NativeType* nativeObject = nullptr)
				: mNativeObject(nativeObject)
				, NativeObject()
			{ }

			NativeObjectWrapper(
				JNIEnv* env,
				NativeType* nativeObject = nullptr)
				: mNativeObject(nativeObject)
				, NativeObject(env)
			{ }

			virtual void setNativeObject(NativeType* nativeObject) {
				mNativeObject.reset(nativeObject);
			}

			virtual NativeType* getNativeObject() {
				return mNativeObject.get();
			}

		protected:
			std::unique_ptr<NativeType> mNativeObject;
		};
	}
}
