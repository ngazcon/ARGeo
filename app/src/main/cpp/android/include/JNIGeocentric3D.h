#pragma once

#include "NativeObjectWrapper.h"
#include "JniHelpers.h"
#include "Geocentric3D.h"
#include "Bootstrap.h"

namespace argeo
{
	namespace jni
	{
		class JNIGeocentric3D
			: public NativeObjectWrapper<Geocentric3D>
		{
		public:
			JNIGeocentric3D(Geocentric3D* geocentric = nullptr);
			JNIGeocentric3D(
				JNIEnv* jni,
				Geocentric3D* geocentric = nullptr);

			const char* getCanonicalName() const
			{
				return MAKE_CANONICAL_NAME(PACKAGE, Geocentric3D);
			}

			void mapFields();

			void initialize(JNIEnv *env);
			
			static void java_initialize(
				JNIEnv* env,
				jobject java_this,
				jdouble x,
				jdouble y,
				jdouble z);

			static jobject create(
				JNIEnv* env,
				const Geocentric3D& geocentric);

			static void dispose(
				JNIEnv *env,
				jobject java_this);

			static jdouble get_x(
				JNIEnv* env,
				jobject java_this);

			static jdouble get_y(
				JNIEnv* env,
				jobject java_this);

			static jdouble get_z(
				JNIEnv* env,
				jobject java_this);

		};
	}
}

