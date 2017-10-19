#include "include/JNIScene.h"
#include "include/JNIITerrain.h"

namespace argeo
{
	JNIScene::JNIScene(Scene* scene)
		: NativeObjectWrapper(scene)
	{ }

	JNIScene::JNIScene(
		JNIEnv* env,
		Scene* scene)
		: NativeObjectWrapper(env, scene)
	{
		initialize(env);
	}

	void JNIScene::initialize(JNIEnv *env)
	{
		setClass(env);

		cacheConstructor(env);

		addNativeMethod(
			"dispose",
			(void*)&JNIScene::dispose,
			kTypeVoid,
			NULL);

		addNativeMethod(
			"nativeGetTerrain",
			(void*)&JNIScene::get_terrain,
			MAKE_CANONICAL_NAME(PACKAGE, ITerrain),
			NULL);

		addNativeMethod(
			"nativeGetCamera",
			(void*)&JNIScene::get_camera,
			MAKE_CANONICAL_NAME(PACKAGE, Camera),
			NULL);

		addNativeMethod(
			"getTerrainOpacity",
			(void*)&JNIScene::get_terrain_opacity,
			kTypeFloat,
			NULL);

		addNativeMethod(
			"setTerrainOpacity",
			(void*)&JNIScene::set_terrain_opacity,
			kTypeVoid,
			kTypeFloat,
			NULL);

		addNativeMethod(
                "setMode",
				(void*)&JNIScene::set_mode,
				kTypeVoid,
				kTypeInt,
				NULL);

		addNativeMethod(
			"addSnapshotListener",
			(void*)&JNIScene::add_snapshot_listener,
			kTypeBool,
			MAKE_CANONICAL_NAME(PACKAGE, SnapshotListener),
			NULL);

		addNativeMethod(
			"removeSnapshotListener",
			(void*)&JNIScene::remove_snapshot_listener,
			kTypeBool,
			MAKE_CANONICAL_NAME(PACKAGE, SnapshotListener),
			NULL);

		addNativeMethod(
			"takeSnapshot",
			(void*)&JNIScene::take_snapshot,
			kTypeVoid,
			NULL);

		addNativeMethod(
			"takeARBufferSnapshot",
			(void*)&JNIScene::take_ar_buffer_snapshot,
			kTypeVoid,
			NULL);

		addNativeMethod(
				"getUseVR",
				(void*)&JNIScene::get_use_vr,
				kTypeBool,
				NULL);

		addNativeMethod(
				"setUseVR",
				(void*)&JNIScene::set_use_vr,
				kTypeVoid,
				kTypeBool,
				NULL);

		addNativeMethod(
			"takeTTARSnapshot",
			(void*)&JNIScene::take_ttar_snapshot,
			kTypeVoid,
			"android/graphics/Bitmap",
            "android/graphics/Bitmap",
			MAKE_CANONICAL_NAME(PACKAGE, Camera),
			kTypeInt,
			kTypeInt,
			NULL);

		registerNativeMethods(env);
	}

	void JNIScene::mapFields()
	{ }

	void JNIScene::dispose(JNIEnv *env, jobject java_this)
	{
		JNIScene *object = gClasses.getNativeInstance<JNIScene>(env, java_this);
		if (object != NULL)
		{
			object->destroy(env, java_this);
		}
	}

	jobject JNIScene::get_camera(
		JNIEnv* env,
		jobject java_this)
	{
		JNIScene *object = gClasses.getNativeInstance<JNIScene>(env, java_this);
		return JNICamera::create(env, object->getNativeObject()->get_camera());
	}

	jobject JNIScene::get_terrain(
		JNIEnv* env,
		jobject java_this)
	{
		JNIScene *object = gClasses.getNativeInstance<JNIScene>(env, java_this);
		return JNIITerrain::create(env, object->getNativeObject()->get_terrain());
	}

	void JNIScene::set_terrain_opacity(
		JNIEnv* env,
		jobject java_this,
		jfloat  value)
	{
		JNIScene *object = gClasses.getNativeInstance<JNIScene>(env, java_this);
		object->getNativeObject()->set_terrain_opacity(value);
	}

	jfloat JNIScene::get_terrain_opacity(
		JNIEnv* env,
		jobject java_this)
	{
		JNIScene *object = gClasses.getNativeInstance<JNIScene>(env, java_this);
		return object->getNativeObject()->get_terrain_opacity();
	}

    void JNIScene::set_use_vr(
            JNIEnv* env,
            jobject java_this,
            jboolean value)
    {
        JNIScene *object = gClasses.getNativeInstance<JNIScene>(env, java_this);
        object->getNativeObject()->set_use_vr(value);
    }

    jboolean JNIScene::get_use_vr(
            JNIEnv* env,
            jobject java_this)
    {
        JNIScene *object = gClasses.getNativeInstance<JNIScene>(env, java_this);
        return object->getNativeObject()->get_use_vr();
    }

	jobject JNIScene::create(
		JNIEnv* jni,
		Scene* scene)
	{
		JNIScene* object = gClasses.newInstance<JNIScene>(jni, nullptr);
		object->setNativeObject(scene);
		jobject java_this = object->toJavaObject(jni);
		object->persist(jni, java_this);
		return java_this;
	}

	jboolean JNIScene::add_snapshot_listener(
		JNIEnv* jni,
		jobject java_this,
		jobject java_listener)
	{
		JNIScene* object = gClasses.getNativeInstance<JNIScene>(jni, java_this);

		auto entry = std::find_if(
			object->m_listeners.begin(),
			object->m_listeners.end(),
			entry_equals(jni, java_listener));
		
		if (entry == object->m_listeners.end())
		{
			java_listener = jni->NewGlobalRef(java_listener);
			JNISnapshotListener* native_listener = new JNISnapshotListener(jni, java_listener);
			object->getNativeObject()->add_snapshot_listener(native_listener);
			object->m_listeners.emplace(java_listener,  native_listener);

			return true;
		}

		return false;
	}

	jboolean JNIScene::remove_snapshot_listener(
		JNIEnv* jni,
		jobject java_this,
		jobject java_listener)
	{
		JNIScene* object = gClasses.getNativeInstance<JNIScene>(jni, java_this);

		auto entry = std::find_if(
			object->m_listeners.begin(),
			object->m_listeners.end(),
			entry_equals(jni, java_listener));

		if (entry != object->m_listeners.end())
		{
			JNISnapshotListener* native_listener = object->m_listeners[entry->first];
			object->getNativeObject()->remove_snapshot_listener(native_listener);
			object->m_listeners.erase(entry->first);
			jni->DeleteGlobalRef(entry->first);
			delete native_listener;
			return true;
		}

		return false;
	}

	void JNIScene::take_ttar_snapshot(
		JNIEnv* jni,
		jobject java_this,
		jobject java_reality_snapshot,
		jobject java_ttar_snapshot,
		jobject java_camera,
        jint width,
        jint height)
	{
		JNIScene* object  = gClasses.getNativeInstance<JNIScene>(jni, java_this);

		Camera*    camera			= java_camera == nullptr ? nullptr : gClasses.getNativeInstance<JNICamera>(jni, java_camera)->getNativeObject();
		Bitmap*    reality_snapshot = java_reality_snapshot == nullptr ? nullptr : new Bitmap(java_reality_snapshot);
		Bitmap*    ttar_snapshot    = java_ttar_snapshot == nullptr ? nullptr : new Bitmap(java_ttar_snapshot);

		object->getNativeObject()->take_ttar_snapshot(
			reality_snapshot,
			ttar_snapshot,
			camera,
            width,
            height);
	}

	void JNIScene::set_mode(
			JNIEnv *jni,
			jobject java_this,
			int mode)
	{
		JNIScene* object = gClasses.getNativeInstance<JNIScene>(jni, java_this);
		object->getNativeObject()->set_mode(static_cast<SceneMode>(mode));
	}

	void JNIScene::take_snapshot(
		JNIEnv* jni,
		jobject java_this)
	{
		JNIScene* object = gClasses.getNativeInstance<JNIScene>(jni, java_this);
		object->getNativeObject()->take_snapshot();
	}

	void JNIScene::take_ar_buffer_snapshot(
		JNIEnv* jni,
		jobject java_this)
	{
		JNIScene* object = gClasses.getNativeInstance<JNIScene>(jni, java_this);
		object->getNativeObject()->take_ar_buffer_snapshot();
	}
}