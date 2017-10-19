#include "include/JNIQuaternion.h"
#include "include/JNIGeocentric3D.h"

namespace argeo
{
	namespace jni 
	{
		JNIQuaternion::JNIQuaternion(quaternion* quat)
			: NativeObjectWrapper(quat)
		{ }

		JNIQuaternion::JNIQuaternion(
			JNIEnv *env,
			quaternion* quat)
			: NativeObjectWrapper(env, quat)
		{
			initialize(env);
		}

		void JNIQuaternion::initialize(JNIEnv *env)
		{
			setClass(env);

			cacheConstructor(env);

			addNativeMethod(
				"dispose",
				(void*)&JNIQuaternion::dispose,
				kTypeVoid,
				NULL);

			addNativeMethod(
				"initialize",
				(void*)&JNIQuaternion::java_initialize,
				kTypeVoid,
				kTypeDouble,
				kTypeDouble,
				kTypeDouble,
				kTypeDouble,
				NULL);

			addNativeMethod(
				"getW",
				(void*)&JNIQuaternion::get_w,
				kTypeDouble,
				NULL);

			addNativeMethod(
				"getX",
				(void*)&JNIQuaternion::get_x,
				kTypeDouble,
				NULL);

			addNativeMethod(
				"getY",
				(void*)&JNIQuaternion::get_y,
				kTypeDouble,
				NULL);

			addNativeMethod(
				"getZ",
				(void*)&JNIQuaternion::get_z,
				kTypeDouble,
				NULL);

			addNativeMethod(
				"fromTwoVectors",
				(void*)&JNIQuaternion::from_two_vectors,
				MAKE_CANONICAL_NAME(PACKAGE, Quaternion),
				MAKE_CANONICAL_NAME(PACKAGE, Geocentric3D),
				MAKE_CANONICAL_NAME(PACKAGE, Geocentric3D),
				NULL);

			addNativeMethod(
				"fromAxisAngle",
				(void*)&JNIQuaternion::from_axis_angle,
				MAKE_CANONICAL_NAME(PACKAGE, Quaternion),
				MAKE_CANONICAL_NAME(PACKAGE, Geocentric3D),
				kTypeDouble,
				NULL);

			addNativeMethod(
				"fromYawPitchRoll",
				(void*)&JNIQuaternion::from_yaw_pitch_roll,
				MAKE_CANONICAL_NAME(PACKAGE, Quaternion),
				kTypeDouble,
				kTypeDouble,
				kTypeDouble,
				NULL);

			registerNativeMethods(env);
		}

		void JNIQuaternion::mapFields()
		{ }

		void JNIQuaternion::dispose(JNIEnv *env, jobject java_this)
		{
			JNIQuaternion *object = gClasses.getNativeInstance<JNIQuaternion>(env, java_this);
			if (object != NULL)
			{
				object->destroy(env, java_this);
			}
		}

		jobject JNIQuaternion::create(
			JNIEnv* jni,
			const quaternion& quat)
		{
			JNIQuaternion* object = gClasses.newInstance<JNIQuaternion>(jni, nullptr);
			object->setNativeObject(new quaternion(quat));
			jobject java_this = object->toJavaObject(jni);
			object->persist(jni, java_this);
			return java_this;
		}

		void JNIQuaternion::java_initialize(
			JNIEnv* env,
			jobject java_this,
			jdouble w,
			jdouble x,
			jdouble y,
			jdouble z)
		{
			JNIQuaternion* object = gClasses.newInstance<JNIQuaternion>(env, java_this);
			object->setNativeObject(new quaternion(w, x, y, z));
			object->persist(env, java_this);
		}

		jdouble JNIQuaternion::get_w(
			JNIEnv* env,
			jobject java_this)
		{
			JNIQuaternion* object = gClasses.getNativeInstance<JNIQuaternion>(env, java_this);
			return object->getNativeObject()->w;
		}

		jdouble JNIQuaternion::get_x(
			JNIEnv* env,
			jobject java_this)
		{
			JNIQuaternion* object = gClasses.getNativeInstance<JNIQuaternion>(env, java_this);
			return object->getNativeObject()->x;
		}


		jdouble JNIQuaternion::get_y(
			JNIEnv* env,
			jobject java_this)
		{
			JNIQuaternion* object = gClasses.getNativeInstance<JNIQuaternion>(env, java_this);
			return object->getNativeObject()->y;
		}

		jdouble JNIQuaternion::get_z(
			JNIEnv* env,
			jobject java_this)
		{
			JNIQuaternion* object = gClasses.getNativeInstance<JNIQuaternion>(env, java_this);
			return object->getNativeObject()->z;
		}

		jobject JNIQuaternion::from_two_vectors(
			JNIEnv* env,
			jobject java_clazz,
			jobject java_u,
			jobject java_v)
		{
			JNIGeocentric3D* u = gClasses.getNativeInstance<JNIGeocentric3D>(env, java_u);
			JNIGeocentric3D* v = gClasses.getNativeInstance<JNIGeocentric3D>(env, java_v);
			JNIQuaternion* object = gClasses.newInstance<JNIQuaternion>(env, nullptr);
			object->setNativeObject(new quaternion(quaternion::from_two_vectors(u->getNativeObject()->to_vec3d(), v->getNativeObject()->to_vec3d())));
			jobject java_this = object->toJavaObject(env);
			object->persist(env, java_this);
			return java_this;
		}

		jobject JNIQuaternion::from_yaw_pitch_roll(
			JNIEnv* env,
			jobject java_clazz,
			jdouble yaw,
			jdouble pitch,
			jdouble roll)
		{
			JNIQuaternion* object = gClasses.newInstance<JNIQuaternion>(env, nullptr);
			object->setNativeObject(new quaternion(quaternion::from_yaw_pitch_roll(yaw, pitch, roll)));
			jobject java_this = object->toJavaObject(env);
			object->persist(env, java_this);
			return java_this;
		}

		jobject JNIQuaternion::from_axis_angle(
			JNIEnv* env,
			jobject java_clazz,
			jobject java_axis,
			jdouble angle)
		{
			JNIGeocentric3D* axis = gClasses.getNativeInstance<JNIGeocentric3D>(env, java_axis);
			JNIQuaternion* object = gClasses.newInstance<JNIQuaternion>(env, nullptr);
			object->setNativeObject(new quaternion(quaternion::from_axis_angle(axis->getNativeObject()->to_vec3d(), angle)));
			jobject java_this = object->toJavaObject(env);
			object->persist(env, java_this);
			return java_this;
		}
	}
}