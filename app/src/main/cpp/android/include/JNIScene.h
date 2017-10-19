#pragma once

#include "JNIHelper.h"
#include "JniHelpers.h"
#include "Bootstrap.h"
#include "Entity.h"
#include "NativeObjectWrapper.h"
#include "Scene.h"

#include "JNICamera.h"

#include <memory>
#include <Log.h>

namespace argeo
{
	namespace jni
	{
		class JNISnapshotListener : public SnapshotListener
		{
		public:
			JNISnapshotListener(
				JNIEnv* jni,
				jobject java_listener)
				: m_java_listener(java_listener)
			{ 
				m_clazz			= jni->GetObjectClass(m_java_listener);

				std::string on_snapshot_available_signature;
				JavaClassUtils::makeSignature(
					on_snapshot_available_signature,
					kTypeVoid,
					"android/graphics/Bitmap",
					NULL);
				
				m_on_snapshot_available = jni->GetMethodID(
					m_clazz,
					"onSnapshotAvailable",
					on_snapshot_available_signature.c_str());


				std::string on_ar_buffer_snapshot_available_signature;
				JavaClassUtils::makeSignature(
					on_ar_buffer_snapshot_available_signature,
					kTypeVoid,
					"android/graphics/Bitmap",
					"android/graphics/Bitmap",
					"android/graphics/Bitmap",	
					NULL);

				m_on_ar_buffer_snapshot_available = jni->GetMethodID(
					m_clazz,
					"onARBufferSnapshotAvailable",
					on_ar_buffer_snapshot_available_signature.c_str());

				std::string on_ttar_snapshot_available_signature;
				JavaClassUtils::makeSignature(
					on_ttar_snapshot_available_signature,
					kTypeVoid,
					"android/graphics/Bitmap",
					"android/graphics/Bitmap",
					MAKE_CANONICAL_NAME(PACKAGE, Camera),
					NULL);

				m_on_ttar_snapshot_available = jni->GetMethodID(
					m_clazz,
					"onTTARSnapshotAvailable",
					on_ttar_snapshot_available_signature.c_str());
			}

			~JNISnapshotListener()
			{ }

			inline void on_snapshot_available(Bitmap* snapshot)
			{
				JNIEnv* jni;
				jint status = JNIHelper::get_instance().attach_current_thread(&jni);
				
				jobject bitmap = static_cast<jobject>(snapshot->get_handle());

				jni->CallVoidMethod(
					m_java_listener,
					m_on_snapshot_available,
					bitmap
                );

				JNIHelper::get_instance().detach_current_thread(status);
			}

			inline void on_ar_buffer_snapshot_available(
				Bitmap* reality_snapshot,
				Bitmap* virtuality_snapshot,
				Bitmap* mixed_reality_snapshot)
			{
				JNIEnv* jni;
				jint status = JNIHelper::get_instance().attach_current_thread(&jni);

				jobject reality_bitmap = static_cast<jobject>(reality_snapshot->get_handle());
				jobject virtuality_bitmap = static_cast<jobject>(virtuality_snapshot->get_handle());
				jobject mixed_reality_bitmap = static_cast<jobject>(mixed_reality_snapshot->get_handle());

				jni->CallVoidMethod(
					m_java_listener,
					m_on_ar_buffer_snapshot_available,
					reality_bitmap,
					virtuality_bitmap,
					mixed_reality_bitmap);

				JNIHelper::get_instance().detach_current_thread(status);
			}

			inline void on_ttar_snapshot_available(
				Bitmap* reality_snapshot,				
				Bitmap* ttar_snapshot,
				Camera* camera)
			{
				JNIEnv* jni;
				jint status = JNIHelper::get_instance().attach_current_thread(&jni);

				jobject reality_bitmap = static_cast<jobject>(reality_snapshot->get_handle());
				jobject ttar_bitmap    = static_cast<jobject>(ttar_snapshot->get_handle());
				
				jni->CallVoidMethod(
					m_java_listener,
					m_on_ttar_snapshot_available,
                    reality_bitmap,
                    ttar_bitmap,
					JNICamera::create(jni, camera));

				JNIHelper::get_instance().detach_current_thread(status);
			}

		private:
			jclass    m_clazz;

			jmethodID m_on_snapshot_available;
			jmethodID m_on_ttar_snapshot_available;
			jmethodID m_on_ar_buffer_snapshot_available;

			jobject   m_java_listener;
		};

		class JNIScene
			: public NativeObjectWrapper<Scene>
		{
		public:
			JNIScene(Scene*  scene = nullptr);
			JNIScene(
				JNIEnv* jni,
				Scene*  scene = nullptr);

			const char* getCanonicalName() const
			{
				return MAKE_CANONICAL_NAME(PACKAGE, Scene);
			}

			void initialize(JNIEnv *env);
			void mapFields();

			static void dispose(
				JNIEnv *env,
				jobject java_this);

			static jobject create(
				JNIEnv* env,
				Scene*  scene);

			static jobject get_camera(
				JNIEnv* env,
				jobject java_this);

			static jobject get_terrain(
				JNIEnv* env,
				jobject java_this);

			static void set_terrain_opacity(
				JNIEnv* env,
				jobject java_this,
				jfloat  value);

			static jfloat get_terrain_opacity(
				JNIEnv* env,
				jobject java_this);

			static void set_use_vr(
					JNIEnv* env,
					jobject java_this,
					jboolean  value);

			static jboolean get_use_vr(
					JNIEnv* env,
					jobject java_this);

			static jboolean add_snapshot_listener(
				JNIEnv* jni,
				jobject java_this,
				jobject java_listener);

			static jboolean remove_snapshot_listener(
				JNIEnv* jni,
				jobject java_this,
				jobject java_listener);

			static void take_snapshot(
				JNIEnv* jni,
				jobject java_this);

			static void take_ar_buffer_snapshot(
				JNIEnv* jni,
				jobject java_this);

			static void take_ttar_snapshot(
				JNIEnv* jni,
				jobject java_this,
				jobject java_reality_snapshot,
				jobject java_ttar_snapshot,
				jobject java_camera,
				jint width,
				jint height);

			static void set_mode(
				JNIEnv* jni,
				jobject	java_this,
				int mode);


		private:
			
			struct entry_equals
			{
				entry_equals(
					JNIEnv* jni,
					jobject key)
					: m_jni(jni)
					, m_key(key)
				{ }

				bool operator()(					
					const std::pair<jobject, JNISnapshotListener*>& v) const
				{
					return m_jni->IsSameObject(m_key, v.first);
				}

			private:
				JNIEnv* m_jni;
				jobject m_key;
			};

			std::map<jobject, JNISnapshotListener*> m_listeners;
		};
	}
}
