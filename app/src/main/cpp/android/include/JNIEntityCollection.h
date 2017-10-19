#pragma once

#include "JniHelpers.h"
#include "Bootstrap.h"
#include "NativeObjectWrapper.h"
#include "EntityCollection.h"

#include <memory>

namespace argeo
{
	namespace jni
	{
		class JNIEntityCollection
			: public NativeObjectWrapper<EntityCollection>
		{
		public:
			JNIEntityCollection(EntityCollection* entity_collection = nullptr);
			JNIEntityCollection(
				JNIEnv* jni,
				EntityCollection* entity_collection = nullptr);

			const char* getCanonicalName() const
			{
				return MAKE_CANONICAL_NAME(PACKAGE, EntityCollection);
			}

			void initialize(JNIEnv *env);
			void mapFields();

			static void java_initialize(
				JNIEnv* jni,
				jobject java_this);

			static jobject create(
				JNIEnv* jni,
				EntityCollection* collection);

			static jobject get_by_id(
				JNIEnv* jni,
				jobject java_this,
				jstring java_id);

			static void dispose(
				JNIEnv *env,
				jobject java_this);

			static jboolean add(
				JNIEnv* jni,
				jobject java_this,
				jobject entity_object);

			static jboolean remove(
				JNIEnv* jni,
				jobject java_this,
				jobject entity_object);
		};
	}
}