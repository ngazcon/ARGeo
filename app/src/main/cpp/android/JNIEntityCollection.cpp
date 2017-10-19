#include "include/JNIEntityCollection.h"
#include "include/JNIEntity.h"

namespace argeo
{
	JNIEntityCollection::JNIEntityCollection(EntityCollection* entity_collection)
		: NativeObjectWrapper()
	{ }

	JNIEntityCollection::JNIEntityCollection(
		JNIEnv *env,
		EntityCollection* entity_collection)
		: NativeObjectWrapper(env, entity_collection)
	{
		initialize(env);
	}

	void JNIEntityCollection::initialize(JNIEnv *env)
	{
		setClass(env);

		cacheConstructor(env);

		addNativeMethod(
			"dispose",
			(void*)&JNIEntityCollection::dispose,
			kTypeVoid,
			NULL);

		addNativeMethod(
			"initialize",
			(void*)&JNIEntityCollection::java_initialize,
			kTypeVoid,
			NULL);

		addNativeMethod(
			"add",
			(void*)&JNIEntityCollection::add,
			kTypeBool,
			MAKE_CANONICAL_NAME(PACKAGE, Entity),
			NULL);

		addNativeMethod(
			"remove",
			(void*)&JNIEntityCollection::remove,
			kTypeBool,
			MAKE_CANONICAL_NAME(PACKAGE, Entity),
			NULL);

		addNativeMethod(
			"getById",
			(void*)&JNIEntityCollection::get_by_id,
			MAKE_CANONICAL_NAME(PACKAGE, Entity),
			kTypeString,
			NULL);

		registerNativeMethods(env);
	}

	void JNIEntityCollection::mapFields()
	{ }

	void JNIEntityCollection::dispose(JNIEnv *env, jobject java_this)
	{
		JNIEntityCollection *object = gClasses.getNativeInstance<JNIEntityCollection>(env, java_this);
		if (object != NULL)
		{
			object->destroy(env, java_this);
		}
	}

	void JNIEntityCollection::java_initialize(
		JNIEnv* jni,
		jobject java_this)
	{
		JNIEntityCollection* object = gClasses.newInstance<JNIEntityCollection>(jni, java_this);
		object->setNativeObject(new EntityCollection());
		object->persist(jni, java_this);
	}

	jobject JNIEntityCollection::create(
		JNIEnv* jni,
		EntityCollection* collection)
	{
		JNIEntityCollection* object = gClasses.newInstance<JNIEntityCollection>(jni, nullptr);
		object->setNativeObject(collection);
		jobject java_this = object->toJavaObject(jni);
		object->persist(jni, java_this);
		return java_this;
	}

	jboolean JNIEntityCollection::add(
		JNIEnv* jni,
		jobject java_this,
		jobject java_entity)
	{ 
		JNIEntityCollection *object = gClasses.getNativeInstance<JNIEntityCollection>(jni, java_this);
		JNIEntity *entity_object    = gClasses.getNativeInstance<JNIEntity>(jni, java_entity);
		return object->getNativeObject()->add(entity_object->getNativeObject());
	}

	jboolean JNIEntityCollection::remove(
		JNIEnv* jni,
		jobject java_this,
		jobject java_entity)
	{
		JNIEntityCollection *object = gClasses.getNativeInstance<JNIEntityCollection>(jni, java_this);
		JNIEntity *entity_object    = gClasses.getNativeInstance<JNIEntity>(jni, java_entity);
		return object->getNativeObject()->remove(entity_object->getNativeObject());
	}

	jobject JNIEntityCollection::get_by_id(
		JNIEnv* jni,
		jobject java_this,
		jstring java_id)
	{
		JNIEntityCollection *object = gClasses.getNativeInstance<JNIEntityCollection>(jni, java_this);
		JavaString native_id(jni, java_id);
		Entity* entity = object->getNativeObject()->get_by_id(native_id.get());
		return JNIEntity::create(jni, entity);
	}
}