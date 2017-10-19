#include "include/NativeObject.h"
#include "Entity.h"
#include "include/JNIEntity.h"
#include "include/JNIIGraphics.h"
#include "include/JNIGeocentric3D.h"
#include "include/JNIGeodetic3D.h"
#include "include/JNIQuaternion.h"

#include "EllipsoidTransformations.h"

namespace argeo
{
	JNIEntity::JNIEntity(Entity* entity)
		: NativeObjectWrapper(entity)
	{ }

	JNIEntity::JNIEntity(
		JNIEnv* env,
		Entity* entity)
		: NativeObjectWrapper(env, entity)
	{
		initialize(env);
	}

	void JNIEntity::initialize(JNIEnv *env)
	{
		setClass(env);

		cacheConstructor(env);

		addNativeMethod(
			"dispose",
			(void*)&JNIEntity::dispose,
			kTypeVoid,
			NULL);

		addNativeMethod(
			"initialize",
			(void*)&JNIEntity::java_initialize,
			kTypeVoid,
			kTypeString,
			MAKE_CANONICAL_NAME(PACKAGE, IGraphics),
			NULL);

		addNativeMethod(
			"initialize",
			(void*)&JNIEntity::java_initialize2,
			kTypeVoid,
			kTypeString,
			kTypeString,
			MAKE_CANONICAL_NAME(PACKAGE, IGraphics),
			NULL);

		addNativeMethod(
			"getPosition",
			(void*)&JNIEntity::get_geocentric_location,
			MAKE_CANONICAL_NAME(PACKAGE, Geocentric3D),
			NULL);

		addNativeMethod(
			"setPosition",
			(void*)&JNIEntity::set_geocentric_location,
			kTypeVoid,
			MAKE_CANONICAL_NAME(PACKAGE, Geocentric3D),
			NULL);

		addNativeMethod(
			"setPosition",
			(void*)&JNIEntity::set_geodetic_location,
			kTypeVoid,
			MAKE_CANONICAL_NAME(PACKAGE, Geodetic3D),
			NULL);

		addNativeMethod(
			"nativeGetGraphics",
			(void*)&JNIEntity::get_graphics,
			MAKE_CANONICAL_NAME(PACKAGE, IGraphics),
			NULL);

		addNativeMethod(
			"setOrientation",
			(void*)&JNIEntity::set_orientation,
			kTypeVoid,
			MAKE_CANONICAL_NAME(PACKAGE, Quaternion),
			NULL);

		addNativeMethod(
			"getOrientation",
			(void*)&JNIEntity::get_orientation,
			MAKE_CANONICAL_NAME(PACKAGE, Quaternion),
			NULL);

		addNativeMethod(
			"getId",
			(void*)&JNIEntity::get_id,
			kTypeString,
			NULL);

		addNativeMethod(
			"getName",
			(void*)&JNIEntity::get_name,
			kTypeString,
			NULL);

		registerNativeMethods(env);
	}

	void JNIEntity::mapFields()
	{ }

	void JNIEntity::dispose(JNIEnv *env, jobject java_this)
	{
		JNIEntity *object = gClasses.getNativeInstance<JNIEntity>(env, java_this);
		if (object != NULL)
		{
			object->destroy(env, java_this);
		}
	}

	jobject JNIEntity::create(
		JNIEnv* jni,
		Entity* entity)
	{
		JNIEntity* object = gClasses.newInstance<JNIEntity>(jni, nullptr);
		object->setNativeObject(entity);
		jobject java_this = object->toJavaObject(jni);
		object->persist(jni, java_this);
		return java_this;
	}

	void JNIEntity::java_initialize(
		JNIEnv* jni,
		jobject java_this,
		jstring name,
		jobject graphics_object)
	{
		JNIIGraphics* graphics = gClasses.getNativeInstance<JNIIGraphics>(jni, graphics_object);
		JavaString native_name(jni, name);
		
		JNIEntity* object = gClasses.newInstance<JNIEntity>(jni, java_this);
		object->setNativeObject(new Entity(
			native_name.get(),
			graphics->getNativeObject()));
		object->persist(jni, java_this);
	}

	void JNIEntity::java_initialize2(
		JNIEnv* jni,
		jobject java_this,
		jstring id,
		jstring name,
		jobject graphics_object)
	{
		JNIIGraphics* graphics = gClasses.getNativeInstance<JNIIGraphics>(jni, graphics_object);

		JavaString native_name(jni, name);
		JavaString native_id(jni, id);

		JNIEntity* object = gClasses.newInstance<JNIEntity>(jni, java_this);

		object->setNativeObject(new Entity(
			native_id.get(),
			native_name.get(),
			graphics->getNativeObject()));
		object->persist(jni, java_this);
	}

	void JNIEntity::set_geodetic_location(
		JNIEnv* jni,
		jobject java_this,
		jobject java_geodetic)
	{
		JNIEntity* entity       = gClasses.getNativeInstance<JNIEntity>(jni, java_this);
		JNIGeodetic3D* geodetic = gClasses.getNativeInstance<JNIGeodetic3D>(jni, java_geodetic);

		entity->getNativeObject()->set_position(EllipsoidTransformations::geocentric3D_from_geodetic3D(
			Geodetic3D(
				geodetic->getNativeObject()->get_longitude(),
				geodetic->getNativeObject()->get_latitude(),
				geodetic->getNativeObject()->get_height())));
	}

	void JNIEntity::set_geocentric_location(
		JNIEnv* jni,
		jobject java_this,
		jobject java_geocentric)
	{
		JNIEntity* entity = gClasses.getNativeInstance<JNIEntity>(jni, java_this);
		JNIGeocentric3D* geocentric = gClasses.getNativeInstance<JNIGeocentric3D>(jni, java_geocentric);
		entity->getNativeObject()->set_position(*geocentric->getNativeObject());
	}

	jobject JNIEntity::get_geocentric_location(
		JNIEnv* jni,
		jobject java_this)
	{
		JNIEntity* entity		= gClasses.getNativeInstance<JNIEntity>(jni, java_this);
		Geocentric3D geocentric = entity->getNativeObject()->get_position();
		return JNIGeocentric3D::create(jni, geocentric);
	}

	jobject JNIEntity::get_graphics(
		JNIEnv* jni,
		jobject java_this)
	{
		JNIEntity* entity   = gClasses.getNativeInstance<JNIEntity>(jni, java_this);
		IGraphics* graphics = entity->getNativeObject()->get_graphics();
		return JNIIGraphics::create(jni, graphics);
	}

	void JNIEntity::set_orientation(
		JNIEnv* jni,
		jobject java_this,
		jobject java_orientation)
	{
		JNIQuaternion* quaternion = gClasses.getNativeInstance<JNIQuaternion>(jni, java_orientation);
		JNIEntity* object = gClasses.getNativeInstance<JNIEntity>(jni, java_this);
		object->getNativeObject()->set_orientation(*quaternion->getNativeObject());
	}

	jobject JNIEntity::get_orientation(
		JNIEnv* jni,
		jobject java_this)
	{
		JNIEntity* object = gClasses.getNativeInstance<JNIEntity>(jni, java_this);
		quaternion quaternion = object->getNativeObject()->get_orientation();
		return JNIQuaternion::create(jni, quaternion);
	}

	jstring JNIEntity::get_id(
		JNIEnv* jni,
		jobject java_this)
	{
		JNIEntity* object = gClasses.getNativeInstance<JNIEntity>(jni, java_this);
		return JavaString(object->getNativeObject()->get_id()).toJavaString(jni).leak();
	}

	jstring JNIEntity::get_name(
		JNIEnv* jni,
		jobject java_this)
	{
		JNIEntity* object = gClasses.getNativeInstance<JNIEntity>(jni, java_this);
		return JavaString(object->getNativeObject()->get_name()).toJavaString(jni).leak();
	}
}
