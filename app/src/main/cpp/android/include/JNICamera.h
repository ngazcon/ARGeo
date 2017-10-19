#pragma once

#include "JNIHelper.h"
#include "JniHelpers.h"
#include "Bootstrap.h"
#include "NativeObjectWrapper.h"
#include "Camera.h"

#include "CameraMoveListener.h"
#include "CameraChangedListener.h"

namespace argeo
{
	class JNICameraMoveListener;
	class JNICameraChangedListener;
	class JNICamera
		: public NativeObjectWrapper<Camera>
	{
	public:
		JNICamera(Camera* camera = nullptr);
		JNICamera(
			JNIEnv* jni,
			Camera* camera = nullptr);

		const char* getCanonicalName() const
		{
			return MAKE_CANONICAL_NAME(PACKAGE, Camera);
		}

		void initialize(JNIEnv *env);
		void mapFields();

		static void dispose(
			JNIEnv *env,
			jobject java_this);

		static void java_initialize(
			JNIEnv   *jni,
			jobject   java_this,
			jobject   java_scene);


		static jobject create(
			JNIEnv* jni,
			Camera* camera);

		static void zoom_in(
				JNIEnv   *jni,
				jobject  java_this,
				jdouble  amount);

		static void zoom_out(
				JNIEnv   *jni,
				jobject  java_this,
				jdouble  amount);

		static void twist_left(
			JNIEnv   *jni,
			jobject  java_this,
			jdouble  angle);

		static void twist_right(
			JNIEnv   *jni,
			jobject  java_this,
			jdouble  angle);

		static void look_left(
			JNIEnv   *jni,
			jobject  java_this,
			jdouble  angle);

		static void look_right(
			JNIEnv   *jni,
			jobject  java_this,
			jdouble  angle);

		static void look_up(
			JNIEnv   *jni,
			jobject  java_this,
			jdouble  angle);

		static void look_down(
			JNIEnv   *jni,
			jobject  java_this,
			jdouble  angle);

		static void rotate_left(
			JNIEnv   *jni,
			jobject  java_this,
			jdouble  angle);

		static void rotate_right(
			JNIEnv   *jni,
			jobject  java_this,
			jdouble  angle);

		static void rotate_up(
			JNIEnv   *jni,
			jobject  java_this,
			jdouble  angle);

		static void rotate_down(
			JNIEnv   *jni,
			jobject  java_this,
			jdouble  angle);

		static void move_forward(
				JNIEnv   *jni,
				jobject  java_this,
				jdouble  amount);

		static void move_backward(
				JNIEnv   *jni,
				jobject  java_this,
				jdouble  amount);

		static void move_left(
			JNIEnv   *jni,
			jobject  java_this,
			jdouble  amount);

		static void move_right(
			JNIEnv   *jni,
			jobject  java_this,
			jdouble  amount);

		static void move_up(
			JNIEnv   *jni,
			jobject  java_this,
			jdouble  amount);

		static void move_down(
			JNIEnv   *jni,
			jobject  java_this,
			jdouble  amount);

		static void set_percentage_changed(
			JNIEnv   *jni,
			jobject  java_this,
			jdouble  percentage);

		static jdouble get_percentage_changed(
			JNIEnv   *jni,
			jobject  java_this);


        static jdouble get_magnitude(
                JNIEnv *env,
                jobject java_this);

		static jobject get_position_geodetic(
			JNIEnv *env,
			jobject java_this);

		static jobject get_position(
			JNIEnv *env,
			jobject java_this);

		static jobject get_direction(
			JNIEnv *env,
			jobject java_this);

		static jobject get_up(
			JNIEnv *env,
			jobject java_this);

		static jdouble get_pitch(
			JNIEnv   *jni,
			jobject  java_this);

		static jdouble get_yaw(
			JNIEnv   *jni,
			jobject  java_this);

		static jdouble get_roll(
			JNIEnv   *jni,
			jobject  java_this);

		static void set_position(
			JNIEnv *env,
			jobject java_this,
			jobject java_position);

		static void set_direction(
			JNIEnv *env,
			jobject java_this,
			jobject java_direction);

		static void set_up(
			JNIEnv *env,
			jobject java_this,
			jobject java_up);

		static void set_view(
                JNIEnv* env,
                jobject java_this,
                jobject java_position,
                jdouble yaw,
                jdouble pitch,
                jdouble roll);

		static jboolean add_move_listener(
			JNIEnv   *jni,
			jobject  java_this,
			jobject  java_listener);

		static jboolean remove_move_listener(
			JNIEnv   *jni,
			jobject  java_this,
			jobject  java_listener);

		static jboolean add_changed_listener(
			JNIEnv   *jni,
			jobject  java_this,
			jobject  java_listener);

		static jboolean remove_changed_listener(
			JNIEnv   *jni,
			jobject  java_this,
			jobject  java_listener);

		private:
			struct changed_entry_equals
			{
				changed_entry_equals(
					JNIEnv* jni,
					jobject key)
					: m_jni(jni)
					, m_key(key)
				{ }

				bool operator()(
					const std::pair<jobject, JNICameraChangedListener*>& v) const
				{
					return m_jni->IsSameObject(m_key, v.first);
				}

			private:
				JNIEnv* m_jni;
				jobject m_key;
			};

			struct moved_entry_equals
			{
				moved_entry_equals(
					JNIEnv* jni,
					jobject key)
					: m_jni(jni)
					, m_key(key)
				{ }

				bool operator()(
					const std::pair<jobject, JNICameraMoveListener*>& v) const
				{
					return m_jni->IsSameObject(m_key, v.first);
				}

			private:
				JNIEnv* m_jni;
				jobject m_key;
			};

		private:
			std::map<jobject, JNICameraMoveListener*> m_move_listeners;
			std::map<jobject, JNICameraChangedListener*> m_changed_listeners;
	};

	class JNICameraMoveListener : public CameraMoveListener
	{
	public:
		JNICameraMoveListener(jobject java_listener)
			: m_java_listener(java_listener)
		{ }

		~JNICameraMoveListener()
		{ }

		inline void on_move_start(Camera* camera)
		{
			JNIEnv* jni;
			jint status = JNIHelper::get_instance().attach_current_thread(&jni);
			jclass clazz = jni->GetObjectClass(m_java_listener);

			std::string on_move_start_signature;
			JavaClassUtils::makeSignature(
				on_move_start_signature,
				kTypeVoid,
				MAKE_CANONICAL_NAME(PACKAGE, Camera),
				nullptr);

			jni->CallVoidMethod(
				m_java_listener,
				jni->GetMethodID(
					clazz,
					"onMoveStart",
					on_move_start_signature.c_str()),
				JNICamera::create(jni, camera));
			JNIHelper::get_instance().detach_current_thread(status);
		}

		inline void on_move_end(Camera* camera)
		{
			JNIEnv* jni;
			jint status = JNIHelper::get_instance().attach_current_thread(&jni);
			jclass clazz = jni->GetObjectClass(m_java_listener);

			std::string on_move_end_signature;
			JavaClassUtils::makeSignature(
				on_move_end_signature,
				kTypeVoid,
				MAKE_CANONICAL_NAME(PACKAGE, Camera),
				nullptr);

			jni->CallVoidMethod(
				m_java_listener,
				jni->GetMethodID(
					clazz,
					"onMoveEnd",
					on_move_end_signature.c_str()),
				JNICamera::create(jni, camera));
			JNIHelper::get_instance().detach_current_thread(status);
		}

	private:
		jobject m_java_listener;
	};

	class JNICameraChangedListener : public CameraChangedListener
	{
	public:
		JNICameraChangedListener(jobject java_listener)
			: m_java_listener(java_listener)
		{ }

		~JNICameraChangedListener()
		{ }

		inline void on_changed(
			const double& percentage,
			Camera* camera)
		{
			JNIEnv* jni;
			jint status  = JNIHelper::get_instance().attach_current_thread(&jni);
			jclass clazz = jni->GetObjectClass(m_java_listener);

			std::string on_changed_signature;
			JavaClassUtils::makeSignature(
				on_changed_signature,
				kTypeVoid,
				kTypeDouble,
				MAKE_CANONICAL_NAME(PACKAGE, Camera),
				nullptr);

			jni->CallVoidMethod(
				m_java_listener,
				jni->GetMethodID(
					clazz,
					"onChanged",
					on_changed_signature.c_str()),
				percentage,
				JNICamera::create(jni, camera));

			JNIHelper::get_instance().detach_current_thread(status);
		}

	private:
		jobject m_java_listener;
	};
}
