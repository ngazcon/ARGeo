#pragma once

#include "JniHelpers.h"
#include "Bootstrap.h"
#include "Entity.h"
#include "NativeObjectWrapper.h"
#include "ITerrain.h"

#include <memory>

namespace argeo
{
	namespace jni
	{
		class JNIITerrain
			: public NativeObjectWrapper<ITerrain>
		{
		public:
			JNIITerrain(ITerrain* terrain = nullptr);
			JNIITerrain(
				JNIEnv* jni,
				ITerrain* terrain = nullptr);

			const char* getCanonicalName() const
			{
				return MAKE_CANONICAL_NAME(PACKAGE, ITerrain);
			}

			void initialize(JNIEnv *env);
			void mapFields();

			static void dispose(
				JNIEnv *env,
				jobject java_this);

			static jobject create(
				JNIEnv* env,
				ITerrain* terrain);

			static void set_wireframe(
				JNIEnv   *env,
				jobject  java_this,
				jboolean value);

			static jboolean get_wireframe(
				JNIEnv   *env,
				jobject  java_this);

			static void set_height_exaggeration(
				JNIEnv   *env,
				jobject  java_this,
				jdouble  value);

			static jdouble get_height_exaggeration(
				JNIEnv   *env,
				jobject  java_this);

			static void set_lighting(
				JNIEnv   *env,
				jobject  java_this,
				jboolean value);

			static jboolean get_lighting(
				JNIEnv   *env,
				jobject  java_this);

			static void set_show(
				JNIEnv   *env,
				jobject  java_this,
				jboolean value);

			static jboolean get_show(
				JNIEnv   *env,
				jobject  java_this);
		};
	}
}
