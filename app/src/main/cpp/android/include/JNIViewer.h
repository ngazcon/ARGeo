#pragma once

#include "TouchEvent.h"
#include "Viewer.h"
#include "NativeObjectWrapper.h"
#include "JniHelpers.h"
#include "JNIHelper.h"
#include "Bootstrap.h"

#include "VideoCapture.h"

#include <map>
#include <memory>

namespace argeo
{
	namespace jni
	{
		class JNIViewerStateListener : public ViewerStateListener
		{
		public:
			JNIViewerStateListener(jobject java_listener)
				: m_java_listener(java_listener)
			{ }

			~JNIViewerStateListener()
			{ }

			inline void on_start_render_loop()
			{
				JNIEnv* jni;
				jint status  = JNIHelper::get_instance().attach_current_thread(&jni);
				jclass clazz = jni->GetObjectClass(m_java_listener);

				std::string on_start_render_loop_signature;
				JavaClassUtils::makeSignature(
					on_start_render_loop_signature,
					kTypeVoid,
					nullptr);

				jni->CallVoidMethod(
					m_java_listener,
					jni->GetMethodID(
						clazz,
						"onStartRenderLoop",
						on_start_render_loop_signature.c_str()));
				JNIHelper::get_instance().detach_current_thread(status);
			}

			inline void on_stop_render_loop()
			{
				JNIEnv* jni;
				jint status  = JNIHelper::get_instance().attach_current_thread(&jni);
				jclass clazz = jni->GetObjectClass(m_java_listener);

				std::string on_stop_render_loop_signature;
				JavaClassUtils::makeSignature(
					on_stop_render_loop_signature,
					kTypeVoid,
					nullptr);

				jni->CallVoidMethod(
					m_java_listener,
					jni->GetMethodID(
						clazz,
						"onStopRenderLoop",
						on_stop_render_loop_signature.c_str()));
				JNIHelper::get_instance().detach_current_thread(status);
			}

		private:
			jobject m_java_listener;
		};

		class JNIViewer
			: public NativeObjectWrapper<Viewer>
		{
		public:
			JNIViewer(Viewer* viewer = nullptr);
			JNIViewer(
				JNIEnv* jni,
				Viewer* viewer = nullptr);

			const char* getCanonicalName() const
			{
				return MAKE_CANONICAL_NAME(PACKAGE, Viewer);
			}

			void initialize(JNIEnv *env);
			void mapFields();

			static void java_initialize(
				JNIEnv* env,
				jobject java_this);

			static jobject create(
				JNIEnv* jni,
				Viewer* viewer);

			static void dispose(
				JNIEnv *env,
				jobject java_this);

			static jboolean add_state_listener(
				JNIEnv* jni,
				jobject java_this,
				jobject java_listener);

			static jboolean remove_state_listener(
				JNIEnv* jni,
				jobject java_this,
				jobject java_listener);

			static void stop_render_loop(
				JNIEnv* env,
				jobject java_this);

			static void start_render_loop(
				JNIEnv* jni,
				jobject java_this);

			static void execute_on_render_loop(
				JNIEnv* jni,
				jobject java_this,
				jobject java_runnable);

			static jboolean is_running(
				JNIEnv* jni,
				jobject java_this);

			static jobject get_entities(
				JNIEnv* jni,
				jobject java_this);

			static jobject get_scene(
				JNIEnv* jni,
				jobject java_this);

			static jobject get_device_pose_camera_controller(
				JNIEnv* jni,
				jobject java_this);

			static jobject get_screen_space_camera_controller(
				JNIEnv* jni,
				jobject java_this);

			static jobject pick_position(
				JNIEnv* jni,
				jobject java_this,
				jint window_x,
				jint window_y);

			static jobject pick_entity(
				JNIEnv* jni,
				jobject java_this,
				jint window_x,
				jint window_y);

		private:
			std::map<jobject, JNIViewerStateListener*> m_listeners;
			std::unique_ptr<VideoCapture> m_direct_video;
		};
	}
}