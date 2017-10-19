#include "include/JNICamera.h"
#include "include/JNIEntity.h"
#include "include/JNIGeodetic3D.h"
#include "include/JNIGeocentric3D.h"
#include "include/JNIScene.h"

#include "include/NativeObject.h"
#include "EntityCollection.h"
#include "ModelGraphics.h"

namespace argeo
{
	JNICamera::JNICamera(Camera* camera)
		: NativeObjectWrapper()
	{ }

	JNICamera::JNICamera(
		JNIEnv *env,
		Camera* camera)
		: NativeObjectWrapper(env, camera)
	{
		initialize(env);
	}

	void JNICamera::initialize(JNIEnv *env)
	{
		setClass(env);

		cacheConstructor(env);

		addNativeMethod(
			"initialize",
			(void*)&JNICamera::java_initialize,
			kTypeVoid,
			MAKE_CANONICAL_NAME(PACKAGE, Scene),
			NULL);

		addNativeMethod(
			"dispose",
			(void*)&JNICamera::dispose,
			kTypeVoid,
			NULL);

		addNativeMethod(
			"twistLeft",
			(void*)&JNICamera::twist_left,
			kTypeVoid,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"twistRight",
			(void*)&JNICamera::twist_right,
			kTypeVoid,
			kTypeDouble,
			NULL);

		addNativeMethod(
				"zoomIn",
				(void*)&JNICamera::zoom_in,
				kTypeVoid,
				kTypeDouble,
				NULL);

		addNativeMethod(
				"zoomOut",
				(void*)&JNICamera::zoom_out,
				kTypeVoid,
				kTypeDouble,
				NULL);

		addNativeMethod(
			"lookLeft",
			(void*)&JNICamera::look_left,
			kTypeVoid,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"lookRight",
			(void*)&JNICamera::look_right,
			kTypeVoid,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"lookUp",
			(void*)&JNICamera::look_up,
			kTypeVoid,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"lookDown",
			(void*)&JNICamera::look_down,
			kTypeVoid,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"rotateLeft",
			(void*)&JNICamera::rotate_left,
			kTypeVoid,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"rotateRight",
			(void*)&JNICamera::rotate_right,
			kTypeVoid,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"rotateUp",
			(void*)&JNICamera::rotate_up,
			kTypeVoid,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"rotateDown",
			(void*)&JNICamera::rotate_down,
			kTypeVoid,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"moveLeft",
			(void*)&JNICamera::move_left,
			kTypeVoid,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"moveRight",
			(void*)&JNICamera::move_right,
			kTypeVoid,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"moveUp",
			(void*)&JNICamera::move_up,
			kTypeVoid,
			kTypeDouble,
			NULL);

        addNativeMethod(
                "moveForward",
                (void*)&JNICamera::move_forward,
                kTypeVoid,
                kTypeDouble,
                NULL);

        addNativeMethod(
                "moveBackward",
                (void*)&JNICamera::move_backward,
                kTypeVoid,
                kTypeDouble,
                NULL);

		addNativeMethod(
			"moveDown",
			(void*)&JNICamera::move_down,
			kTypeVoid,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"setPercentageChanged",
			(void*)&JNICamera::set_percentage_changed,
			kTypeVoid,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"getPercentageChanged",
			(void*)&JNICamera::get_percentage_changed,
			kTypeDouble,
			NULL);

        addNativeMethod(
                "getMagnitude",
                (void*)&JNICamera::get_magnitude,
                kTypeDouble,
                NULL);

		addNativeMethod(
			"getPositionGeodetic",
			(void*)&JNICamera::get_position_geodetic,
			MAKE_CANONICAL_NAME(PACKAGE, Geodetic3D),
			NULL);

		addNativeMethod(
			"getPosition",
			(void*)&JNICamera::get_position,
			MAKE_CANONICAL_NAME(PACKAGE, Geocentric3D),
			NULL);

		addNativeMethod(
			"getDirection",
			(void*)&JNICamera::get_direction,
			MAKE_CANONICAL_NAME(PACKAGE, Geocentric3D),
			NULL);

		addNativeMethod(
			"getUp",
			(void*)&JNICamera::get_up,
			MAKE_CANONICAL_NAME(PACKAGE, Geocentric3D),
			NULL);

		addNativeMethod(
			"getPitch",
			(void*)&JNICamera::get_pitch,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"getYaw",
			(void*)&JNICamera::get_yaw,
			kTypeDouble,
			NULL);

		addNativeMethod(
			"getRoll",
			(void*)&JNICamera::get_roll,
			kTypeDouble,
			NULL);

		addNativeMethod(
            "setView",
            (void*)&JNICamera::set_view,
            kTypeVoid,
            MAKE_CANONICAL_NAME(PACKAGE, Geocentric3D),
            kTypeDouble,
            kTypeDouble,
            kTypeDouble,
            NULL);

		addNativeMethod(
			"setPosition",
			(void*)&JNICamera::set_position,
			kTypeVoid,
			MAKE_CANONICAL_NAME(PACKAGE, Geocentric3D),
			NULL);

		addNativeMethod(
			"setDirection",
			(void*)&JNICamera::set_direction,
			kTypeVoid,
			MAKE_CANONICAL_NAME(PACKAGE, Geocentric3D),
			NULL);

		addNativeMethod(
			"setUp",
			(void*)&JNICamera::set_up,
			kTypeVoid,
			MAKE_CANONICAL_NAME(PACKAGE, Geocentric3D),
			NULL);

		addNativeMethod(
			"addMoveListener",
			(void*)&JNICamera::add_move_listener,
			kTypeBool,
			MAKE_CANONICAL_NAME(PACKAGE, CameraMoveListener),
			NULL);

		addNativeMethod(
			"removeMoveListener",
			(void*)&JNICamera::remove_move_listener,
			kTypeBool,
			MAKE_CANONICAL_NAME(PACKAGE, CameraMoveListener),
			NULL);

		addNativeMethod(
			"addChangedListener",
			(void*)&JNICamera::add_changed_listener,
			kTypeBool,
			MAKE_CANONICAL_NAME(PACKAGE, CameraChangedListener),
			NULL);

		addNativeMethod(
			"removeChangedListener",
			(void*)&JNICamera::remove_changed_listener,
			kTypeBool,
			MAKE_CANONICAL_NAME(PACKAGE, CameraChangedListener),
			NULL);

		registerNativeMethods(env);
	}

	void JNICamera::mapFields()
	{ }

	void JNICamera::dispose(JNIEnv *env, jobject java_this)
	{
		JNICamera *object = gClasses.getNativeInstance<JNICamera>(env, java_this);
		if (object != NULL)
		{
			object->destroy(env, java_this);
		}
	}

	void JNICamera::java_initialize(
		JNIEnv* env,
		jobject java_this,
		jobject java_scene)
	{
		JNICamera* object = gClasses.newInstance<JNICamera>(env, java_this);
		JNIScene*  scene  = gClasses.newInstance<JNIScene>(env, java_scene);

		object->setNativeObject(new Camera(scene->getNativeObject()));
		object->persist(env, java_this);
	}

	jobject JNICamera::create(
		JNIEnv* jni,
		Camera* camera)
	{
		JNICamera* object = gClasses.newInstance<JNICamera>(jni, nullptr);
		object->setNativeObject(camera);
		jobject java_this = object->toJavaObject(jni);
		object->persist(jni, java_this);
		return java_this;
	}

	void JNICamera::twist_left(
		JNIEnv *jni,
		jobject java_this,
		jdouble angle)
	{
		JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
		object->getNativeObject()->twist_left(angle);
	}

	void JNICamera::twist_right(
		JNIEnv *jni,
		jobject java_this,
		jdouble angle)
	{
		JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
		object->getNativeObject()->twist_right(angle);
	}

    void JNICamera::zoom_in(
            JNIEnv *jni,
            jobject java_this,
            jdouble amount)
    {
        JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
        object->getNativeObject()->zoom_in(amount);
    }

    void JNICamera::zoom_out(
            JNIEnv *jni,
            jobject java_this,
            jdouble amount)
    {
        JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
        object->getNativeObject()->zoom_out(amount);
    }

	void JNICamera::look_left(
		JNIEnv *jni,
		jobject java_this,
		jdouble angle)
	{
		JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
		object->getNativeObject()->look_left(angle);
	}

	void JNICamera::look_right(
		JNIEnv *jni,
		jobject java_this,
		jdouble angle)
	{
		JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
		object->getNativeObject()->look_right(angle);
	}

	void JNICamera::look_up(
		JNIEnv *jni,
		jobject java_this,
		jdouble angle)
	{
		JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
		object->getNativeObject()->look_up(angle);
	}

	void JNICamera::look_down(
		JNIEnv *jni,
		jobject java_this,
		jdouble angle)
	{
		JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
		object->getNativeObject()->look_down(angle);
	}

	void JNICamera::rotate_left(
		JNIEnv *jni,
		jobject java_this,
		jdouble angle)
	{
		JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
		object->getNativeObject()->rotate_left(angle);
	}

	void JNICamera::rotate_right(
		JNIEnv *jni,
		jobject java_this,
		jdouble angle)
	{
		JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
		object->getNativeObject()->rotate_right(angle);
	}

	void JNICamera::rotate_up(
		JNIEnv *jni,
		jobject java_this,
		jdouble angle)
	{
		JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
		object->getNativeObject()->rotate_up(angle);
	}

	void JNICamera::rotate_down(
		JNIEnv *jni,
		jobject java_this,
		jdouble angle)
	{
		JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
		object->getNativeObject()->rotate_down(angle);
	}

	void JNICamera::move_left(
		JNIEnv *jni,
		jobject java_this,
		jdouble amount)
	{
		JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
		object->getNativeObject()->move_left(amount);
	}

    void JNICamera::move_forward(
            JNIEnv *jni,
            jobject java_this,
            jdouble amount)
    {
        JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
        object->getNativeObject()->move_forward(amount);
    }

    void JNICamera::move_backward(
            JNIEnv *jni,
            jobject java_this,
            jdouble amount)
    {
        JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
        object->getNativeObject()->move_backward(amount);
    }

	void JNICamera::move_right(
		JNIEnv *jni,
		jobject java_this,
		jdouble amount)
	{
		JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
		object->getNativeObject()->move_right(amount);
	}

	void JNICamera::move_up(
		JNIEnv *jni,
		jobject java_this,
		jdouble amount)
	{
		JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
		object->getNativeObject()->move_up(amount);
	}

	void JNICamera::move_down(
		JNIEnv *jni,
		jobject java_this,
		jdouble amount)
	{
		JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
		object->getNativeObject()->move_down(amount);
	}

	void JNICamera::set_percentage_changed(
		JNIEnv *jni,
		jobject java_this,
		jdouble percentage)
	{
		JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
		object->getNativeObject()->set_percentage_changed(percentage);
	}

	jdouble JNICamera::get_percentage_changed(
		JNIEnv *jni,
		jobject java_this)
	{
		JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
		return object->getNativeObject()->get_percentage_changed();
	}

	jdouble JNICamera::get_magnitude(
			JNIEnv *jni,
			jobject java_this)
	{
		JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
		return object->getNativeObject()->get_magnitude();
	}

	jobject JNICamera::get_position_geodetic(
		JNIEnv *jni,
		jobject java_this)
	{
		JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
		return JNIGeodetic3D::create(jni, object->getNativeObject()->get_position_geodetic());
	}

	jobject JNICamera::get_position(
		JNIEnv *jni,
		jobject java_this)
	{
		JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
		return JNIGeocentric3D::create(jni, object->getNativeObject()->get_position_wc());
	}

	jobject JNICamera::get_direction(
		JNIEnv *jni,
		jobject java_this)
	{
		JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
		return JNIGeocentric3D::create(jni, object->getNativeObject()->get_direction_wc());
	}

	jobject JNICamera::get_up(
		JNIEnv *jni,
		jobject java_this)
	{
		JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
		return JNIGeocentric3D::create(jni, object->getNativeObject()->get_up_wc());
	}

	jdouble JNICamera::get_yaw(
		JNIEnv   *jni,
		jobject  java_this)
	{
		JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
		return object->getNativeObject()->get_yaw();
	}

	jdouble JNICamera::get_pitch(
		JNIEnv   *jni,
		jobject  java_this)
	{
		JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
		return object->getNativeObject()->get_pitch();
	}

	jdouble JNICamera::get_roll(
		JNIEnv   *jni,
		jobject  java_this)
	{
		JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
		return object->getNativeObject()->get_roll();
	}

	void JNICamera::set_position(
		JNIEnv *jni,
		jobject java_this,
		jobject java_position)
	{
		JNICamera* object       = gClasses.getNativeInstance<JNICamera>(jni, java_this);
		JNIGeocentric3D* position = gClasses.getNativeInstance<JNIGeocentric3D>(jni, java_position);
		return object->getNativeObject()->set_position(position->getNativeObject()->to_vec3d());
	}

	void JNICamera::set_direction(
		JNIEnv *jni,
		jobject java_this,
		jobject java_direction)
	{
		JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
		JNIGeocentric3D* direction = gClasses.getNativeInstance<JNIGeocentric3D>(jni, java_direction);
		return object->getNativeObject()->set_direction(direction->getNativeObject()->to_vec3d());
	}

	void JNICamera::set_up(
		JNIEnv *jni,
		jobject java_this,
		jobject java_up)
	{
		JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
		JNIGeocentric3D* up = gClasses.getNativeInstance<JNIGeocentric3D>(jni, java_up);
		return object->getNativeObject()->set_up(up->getNativeObject()->to_vec3d());
	}

    void JNICamera::set_view(
            JNIEnv *env,
            jobject java_this,
            jobject java_position,
            jdouble yaw,
            jdouble pitch,
            jdouble roll)
    {
        JNICamera* object = gClasses.getNativeInstance<JNICamera>(env, java_this);
        JNIGeocentric3D* position = gClasses.getNativeInstance<JNIGeocentric3D>(env, java_position);
        return object->getNativeObject()->set_view(
                position->getNativeObject()->to_vec3d(),
                yaw,
                pitch,
                roll);
    }

	jboolean JNICamera::add_move_listener(
		JNIEnv   *jni,
		jobject  java_this,
		jobject  java_listener)
	{
		JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
		
		auto entry = std::find_if(
			object->m_move_listeners.begin(),
			object->m_move_listeners.end(),
			moved_entry_equals(jni, java_listener));

		if (entry == object->m_move_listeners.end())
		{
			java_listener = jni->NewGlobalRef(java_listener);
			JNICameraMoveListener* native_listener = new JNICameraMoveListener(java_listener);
			object->getNativeObject()->add_move_listener(native_listener);
			object->m_move_listeners[java_listener] = native_listener;
			return true;
		}

		return false;
	}

	jboolean JNICamera::remove_move_listener(
		JNIEnv   *jni,
		jobject  java_this,
		jobject  java_listener)
	{
		JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);

		auto entry = std::find_if(
			object->m_move_listeners.begin(),
			object->m_move_listeners.end(),
			moved_entry_equals(jni, java_listener));

		if (entry != object->m_move_listeners.end())
		{
			JNICameraMoveListener* native_listener = object->m_move_listeners[entry->first];
			object->getNativeObject()->remove_move_listener(native_listener);
			object->m_move_listeners.erase(entry->first);
			jni->DeleteGlobalRef(entry->first);
			delete native_listener;
			return true;
		}

		return false;
	}

	jboolean JNICamera::add_changed_listener(
		JNIEnv   *jni,
		jobject  java_this,
		jobject  java_listener)
	{
		JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);
		
		auto entry = std::find_if(
			object->m_changed_listeners.begin(),
			object->m_changed_listeners.end(),
			changed_entry_equals(jni, java_listener));

		if (entry == object->m_changed_listeners.end())
		{
			java_listener = jni->NewGlobalRef(java_listener);
			JNICameraChangedListener* native_listener = new JNICameraChangedListener(java_listener);
			object->getNativeObject()->add_changed_listener(native_listener);
			object->m_changed_listeners[java_listener] = native_listener;
			return true;
		}

		return false;
	}

	jboolean JNICamera::remove_changed_listener(
		JNIEnv   *jni,
		jobject  java_this,
		jobject  java_listener)
	{
		JNICamera* object = gClasses.getNativeInstance<JNICamera>(jni, java_this);

		auto entry = std::find_if(
			object->m_changed_listeners.begin(),
			object->m_changed_listeners.end(),
			changed_entry_equals(jni, java_listener));

		if (entry != object->m_changed_listeners.end())
		{
			JNICameraChangedListener* native_listener = object->m_changed_listeners[entry->first];
			object->getNativeObject()->remove_changed_listener(native_listener);
			object->m_move_listeners.erase(entry->first);
			jni->DeleteGlobalRef(entry->first);
			delete native_listener;
			return true;
		}

		return false;
	}
}