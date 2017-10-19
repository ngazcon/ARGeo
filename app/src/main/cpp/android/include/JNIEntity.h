#pragma once

#include "JniHelpers.h"
#include "Bootstrap.h"
#include "Entity.h"
#include "NativeObjectWrapper.h"

#include <memory>

namespace argeo
{
	namespace jni
	{
		class JNIEntity 
			: public NativeObjectWrapper<Entity>
		{
		public:
			JNIEntity(Entity* entity = nullptr);
			JNIEntity(
				JNIEnv* jni,
				Entity* entity = nullptr);

			const char* getCanonicalName() const
			{
				return MAKE_CANONICAL_NAME(PACKAGE, Entity);
			}

			void initialize(JNIEnv *env);
			void mapFields();

			static void dispose(
				JNIEnv *env,
				jobject java_this);

			static jobject create(
				JNIEnv* env,
				Entity* entity);

			static void java_initialize(
				JNIEnv* jni,
				jobject java_this,
				jstring name,
				jobject graphics_object);

			static void java_initialize2(
				JNIEnv* jni,
				jobject java_this,
				jstring id,
				jstring name,
				jobject graphics_object);

			static jobject get_geocentric_location(
				JNIEnv* jni,
				jobject java_this);

			static void set_geodetic_location(
				JNIEnv* jni,
				jobject java_this,
				jobject java_geodetic);

			static void set_geocentric_location(
				JNIEnv* jni,
				jobject java_this,
				jobject java_geocentric);

			static jobject get_graphics(
				JNIEnv* jni,
				jobject java_this);

			static void set_orientation(
				JNIEnv* jni,
				jobject java_this,
				jobject java_quaternion);

			static jobject get_orientation(
				JNIEnv* jni,
				jobject java_this);

			static jstring get_id(
				JNIEnv* jni,
				jobject java_this);

			static jstring get_name(
				JNIEnv* jni,
				jobject java_this);
		};
	}
}