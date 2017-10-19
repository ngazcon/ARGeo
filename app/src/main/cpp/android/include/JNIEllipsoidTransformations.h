#pragma once

#include "EllipsoidTransformations.h"
#include "Bootstrap.h"
#include "JniHelpers.h"

namespace argeo
{
	namespace jni
	{
		class JNIEllipsoidTransformations
		{
		public:
			static const char* getCanonicalName()
			{
				return MAKE_CANONICAL_NAME(PACKAGE, EllipsoidTransformations);
			}

			static void register_natives(JNIEnv *env);

			static jobject geocentric3D_from_geodetic3D(
				JNIEnv* jni,
				jobject clazz,
				jobject java_geodetic);

			static jobject geodetic3D_from_geocentric3D(
				JNIEnv* jni,
				jobject clazz,
				jobject java_geocentric);


		};
	}
}
