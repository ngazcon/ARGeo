#pragma once

#include <android/native_activity.h>

#include <jni.h>
#include <string>
#include <stdexcept>
#include <sstream>

namespace argeo
{
	class JNIHelper
	{
	public:
		static JNIHelper& get_instance();

	private:
		JNIHelper();
		~JNIHelper();

	public:
		// C++ 11
		// =======
		// We can use the better technique of deleting the methods
		// we don't want.
		JNIHelper(JNIHelper const&) = delete;
		void operator=(JNIHelper const&) = delete;


	public:
		static void init(
			JNIEnv *jni,
			jobject activity);

		jint attach_current_thread(JNIEnv** jni);
		void detach_current_thread(jint jni_status);

		jclass retrieve_class(
			JNIEnv* jni,
			const char* class_name);

		bool check_exception(
			JNIEnv* jni,
			std::string& error);

		void rethrow_cpp_exception_as_java_exception(
			JNIEnv* jni);

		JavaVM* get_java_vm();
		AAssetManager* get_asset_manager();
		std::string get_internal_data_path();
		std::string get_external_data_path();
		ALooper* get_main_looper();

	private:
		void load_classes(JNIEnv* jni);
		void load_methods(JNIEnv* jni);
		void load_objects(JNIEnv* jni);


	private:
		std::string m_app_name;
		std::string m_internal_data_path;
		std::string m_external_data_path;

		JavaVM* m_java_virtual_machine;
		AAssetManager* m_asset_manager;
		ANativeWindow* m_window;

		jobject m_activity;
		jobject m_class_loader;
		jobject m_java_asset_manager;

		jclass m_activity_class;
		jclass m_class_loader_class;
		jclass m_throwable_class;
		jclass m_frame_class;
		
		jmethodID m_get_class_loader_mid;
		jmethodID m_find_class_mid;
		jmethodID m_throwable_get_cause_mid;
		jmethodID m_throwable_get_stack_trace_mid;
		jmethodID m_throwable_to_string_mid;
		jmethodID m_frame_to_tring_mid;
		jmethodID m_get_assets_mid;

		ALooper*  m_main_looper;
	};
}
