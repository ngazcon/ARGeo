#pragma once

#include "JNIIGraphics.h"
#include "PlaneGraphics.h"

namespace argeo
{
	class JNIPlaneGraphics
			: public JNIIGraphics
	{
	public:
		JNIPlaneGraphics();
		JNIPlaneGraphics(
				JNIEnv* jni,
				PlaneGraphics* graphics = nullptr);

		const char* getCanonicalName() const
		{
			return MAKE_CANONICAL_NAME(PACKAGE, PlaneGraphics);
		}

		void initialize(JNIEnv *env);
		void mapFields();

		static void java_initialize(
				JNIEnv* jni,
				jobject java_this,
				jobject java_ellipsoid_tangent_plane,
				jboolean flag);

		static void set_width(
				JNIEnv* jni,
				jobject java_this,
				jdouble width);

		static jdouble get_width(
				JNIEnv* jni,
				jobject java_this);

		static void set_height(
				JNIEnv* jni,
				jobject java_this,
				jdouble height);

		static jdouble get_height(
				JNIEnv* jni,
				jobject java_this);
	};
}