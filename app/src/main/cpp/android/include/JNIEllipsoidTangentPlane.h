#pragma once

#include "JniHelpers.h"
#include "Bootstrap.h"
#include "NativeObjectWrapper.h"
#include "EllipsoidTangentPlane.h"

namespace argeo
{
	class JNIEllipsoidTangentPlane
		: public NativeObjectWrapper<EllipsoidTangentPlane>
	{
	public:
		JNIEllipsoidTangentPlane(EllipsoidTangentPlane* ellipsoid_tangent_plane = nullptr);
		JNIEllipsoidTangentPlane(
			JNIEnv* jni,
			EllipsoidTangentPlane* ellipsoid_tangent_plane = nullptr);

		const char* getCanonicalName() const
		{
			return MAKE_CANONICAL_NAME(PACKAGE, EllipsoidTangentPlane);
		}

		void initialize(JNIEnv *env);
		void mapFields();

		static void dispose(
			JNIEnv *env,
			jobject java_this);

		static void java_initialize(
			JNIEnv* jni,
			jobject java_this,
			jobject java_geodetic);

		static jobject get_origin(
			JNIEnv* jni,
			jobject java_this);

		static jobject get_x_axis(
			JNIEnv* jni,
			jobject java_this);

		static jobject get_y_axis(
			JNIEnv* jni,
			jobject java_this);

		static jobject get_normal(
			JNIEnv* jni,
			jobject java_this);

	};
}
