#pragma once

#include "NativeObjectWrapper.h"
#include "JniHelpers.h"
#include "Bootstrap.h"
#include "Geodetic3D.h"

namespace argeo
{
	namespace jni
	{
		class JNIGeodetic3D
			: public NativeObjectWrapper<Geodetic3D>
		{
		public:
			JNIGeodetic3D(Geodetic3D* geodetic = nullptr);
			JNIGeodetic3D(
				JNIEnv* jni,
				Geodetic3D* geodetic = nullptr);

			const char* getCanonicalName() const
			{
				return MAKE_CANONICAL_NAME(PACKAGE, Geodetic3D);
			}

			void mapFields();

			void initialize(JNIEnv *env);
			
			static void java_initialize(
				JNIEnv* env,
				jobject java_this,
				jdouble latitude,
				jdouble longitude,
				jdouble altitude);

			static jobject create(
				JNIEnv* env,
				const Geodetic3D& geodetic);

			static void dispose(
				JNIEnv *env,
				jobject java_this);

			static jdouble get_altitude(
				JNIEnv* env,
				jobject java_this);

			static jdouble get_latitude(
				JNIEnv* env,
				jobject java_this);

			static jdouble get_longitude(
				JNIEnv* env,
				jobject java_this);

		};
	}
}

