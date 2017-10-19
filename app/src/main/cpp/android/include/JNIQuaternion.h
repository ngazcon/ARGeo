#pragma once

#include "NativeObjectWrapper.h"
#include "JniHelpers.h"
#include "Quaternion.h"
#include "Bootstrap.h"

namespace argeo
{
	namespace jni
	{
		class JNIQuaternion : public NativeObjectWrapper<quaternion>
		{
		public:
			JNIQuaternion(quaternion* quat = nullptr);
			JNIQuaternion(
				JNIEnv* jni,
				quaternion* quat = nullptr);

			const char* getCanonicalName() const
			{
				return MAKE_CANONICAL_NAME(PACKAGE, Quaternion);
			}

			void mapFields();

			void initialize(JNIEnv *env);

			static void java_initialize(
				JNIEnv* env,
				jobject java_this,
				jdouble w,
				jdouble x,
				jdouble y,
				jdouble z);

			static jobject create(
				JNIEnv* env,
				const quaternion& quat);

			static void dispose(
				JNIEnv *env,
				jobject java_this);

			static jdouble get_w(
				JNIEnv* env,
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

			static jobject from_two_vectors(
				JNIEnv* env,
				jobject java_clazz,
				jobject java_u, 
				jobject java_v);

			static jobject from_yaw_pitch_roll(
				JNIEnv* env,
				jobject java_clazz,
				jdouble yaw,
				jdouble pitch,
				jdouble roll);

			static jobject from_axis_angle(
				JNIEnv* env,
				jobject java_clazz,
				jobject java_axis,
				jdouble angle);
		};
	}
}

