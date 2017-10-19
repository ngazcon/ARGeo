#include "include/JNIHelper.h"

#include <android/asset_manager_jni.h>

namespace argeo
{
	JNIHelper::JNIHelper()
		: m_activity(nullptr)
		, m_activity_class(nullptr)
		, m_class_loader(nullptr)
		, m_class_loader_class(nullptr)
		, m_throwable_class(nullptr)
		, m_frame_class(nullptr)
		, m_asset_manager(nullptr)
		, m_java_virtual_machine(nullptr)
	{ }

	JNIHelper::~JNIHelper()
	{
		JNIEnv* jni;
		m_java_virtual_machine->AttachCurrentThread(&jni, nullptr);

		jni->DeleteGlobalRef(m_java_asset_manager);
		jni->DeleteGlobalRef(m_class_loader);
		jni->DeleteGlobalRef(m_class_loader_class);
		jni->DeleteGlobalRef(m_frame_class);
		jni->DeleteGlobalRef(m_throwable_class);

		m_java_virtual_machine->DetachCurrentThread();
	}

	JNIHelper& JNIHelper::get_instance()
	{
		static JNIHelper instance;
		return instance;
	}

	void JNIHelper::detach_current_thread(jint jni_status)
	{
		if (jni_status == JNI_EDETACHED)
		{
			m_java_virtual_machine->DetachCurrentThread();
		}
	}

	jint JNIHelper::attach_current_thread(JNIEnv** jni)
	{
		jint result = m_java_virtual_machine->GetEnv(reinterpret_cast<void**>(jni), JNI_VERSION_1_6);
		if (result == JNI_EDETACHED)
		{
			m_java_virtual_machine->AttachCurrentThread(jni, nullptr);
		}
		return result;
	}

	jclass JNIHelper::retrieve_class(
		JNIEnv* jni,
		const char* class_name)
	{
		std::string error;

		jstring str_class_name = jni->NewStringUTF(class_name);
		jclass  class_retrieved = static_cast<jclass>(jni->CallObjectMethod(
			m_class_loader,
			m_find_class_mid,
			str_class_name));

		if (check_exception(jni, error) || class_retrieved == nullptr)
		{
			throw std::runtime_error("Error cannot find class.");
		}

		jni->DeleteLocalRef(str_class_name);

		return class_retrieved;
	}

	ALooper* JNIHelper::get_main_looper()
	{
		return m_main_looper;
	}

	void JNIHelper::rethrow_cpp_exception_as_java_exception(JNIEnv* jni)
	{
		try
		{
			throw; // This allows to determine the type of the exception
		}
		catch (const std::bad_alloc& e) {
			jclass jc = jni->FindClass("java/lang/OutOfMemoryError");
			if (jc) jni->ThrowNew(jc, e.what());
		}
		catch (const std::ios_base::failure& e) {
			jclass jc = jni->FindClass("java/io/IOException");
			if (jc) jni->ThrowNew(jc, e.what());
		}
		catch (const std::exception& e) {
			/* unknown exception (may derive from std::exception) */
			jclass jc = jni->FindClass("java/lang/Error");
			if (jc) jni->ThrowNew(jc, e.what());
		}
		catch (...) {
			/* Oops I missed identifying this exception! */
			jclass jc = jni->FindClass("java/lang/Error");
			if (jc) jni->ThrowNew(jc, "Unidentified exception => "
				"Improve rethrow_cpp_exception_as_java_exception()");
		}
	}


	bool JNIHelper::check_exception(
		JNIEnv* jni,
		std::string& error)
	{
		if (!jni->ExceptionCheck())
		{
			return false;
		}

		// Get the exception and clear as no
		// JNI calls can be made while an exception exists.
		jthrowable exception = jni->ExceptionOccurred();
		jni->ExceptionClear();

		std::ostringstream error_stream;

		jstring msg_obj = static_cast<jstring>(jni->CallObjectMethod(
			exception,
			m_throwable_to_string_mid));

		const char* msg_str = jni->GetStringUTFChars(msg_obj, 0);

		error_stream << msg_str;

		jni->ReleaseStringUTFChars(msg_obj, msg_str);
		jni->DeleteLocalRef(msg_obj);

		error = error_stream.str();

		return true;
	}


	void JNIHelper::load_classes(JNIEnv* jni)
	{
		m_class_loader_class = jni->FindClass("java/lang/ClassLoader");
		m_class_loader_class = static_cast<jclass>(jni->NewGlobalRef(m_class_loader_class));

		m_throwable_class = jni->FindClass("java/lang/Throwable");
		m_throwable_class = static_cast<jclass>(jni->NewGlobalRef(m_throwable_class));

		m_frame_class = jni->FindClass("java/lang/StackTraceElement");
		m_frame_class = static_cast<jclass>(jni->NewGlobalRef(m_frame_class));
	}

	void JNIHelper::load_methods(JNIEnv* jni)
	{
		m_get_assets_mid = jni->GetMethodID(
			m_activity_class,
			"getAssets",
			"()Landroid/content/res/AssetManager;");

		m_get_class_loader_mid = jni->GetMethodID(
			m_activity_class,
			"getClassLoader",
			"()Ljava/lang/ClassLoader;");

		m_find_class_mid = jni->GetMethodID(
			m_class_loader_class,
			"loadClass",
			"(Ljava/lang/String;)Ljava/lang/Class;");

		m_throwable_get_cause_mid =
			jni->GetMethodID(
				m_throwable_class,
				"getCause",
				"()Ljava/lang/Throwable;");

		m_throwable_get_stack_trace_mid =
			jni->GetMethodID(
				m_throwable_class,
				"getStackTrace",
				"()[Ljava/lang/StackTraceElement;");

		m_throwable_to_string_mid =
			jni->GetMethodID(
				m_throwable_class,
				"toString",
				"()Ljava/lang/String;");

		m_frame_to_tring_mid =
			jni->GetMethodID(
				m_frame_class,
				"toString",
				"()Ljava/lang/String;");
	}


	void JNIHelper::load_objects(JNIEnv* jni)
	{
		m_class_loader = jni->CallObjectMethod(
			m_activity,
			m_get_class_loader_mid);
		m_class_loader = jni->NewGlobalRef(m_class_loader);

		m_java_asset_manager = jni->CallObjectMethod(
			m_activity,
			m_get_assets_mid);
		m_java_asset_manager = jni->NewGlobalRef(m_java_asset_manager);
	}


	void JNIHelper::init(
		JNIEnv *jni,
		jobject activity)
	{
		JNIHelper& helper = get_instance();

		jni->GetJavaVM(&helper.m_java_virtual_machine);

		helper.m_activity = activity;
		helper.m_activity_class = jni->GetObjectClass(activity);

		jmethodID get_package_name_mid = jni->GetMethodID(
			helper.m_activity_class,
			"getPackageName",
			"()Ljava/lang/String;");

		jstring package_name = static_cast<jstring>(jni->CallObjectMethod(
			activity,
			get_package_name_mid));

		const char* appname = jni->GetStringUTFChars(package_name, nullptr);
		helper.m_app_name = std::string(appname);
		
		helper.load_classes(jni);
		helper.load_methods(jni);
		helper.load_objects(jni);

		jmethodID get_files_dir_mid = jni->GetMethodID(
			helper.m_activity_class,
			"getFilesDir",
			"()Ljava/io/File;");

		jmethodID get_external_files_dir_mid = jni->GetMethodID(
			helper.m_activity_class,
			"getExternalFilesDir",
			"(Ljava/lang/String;)Ljava/io/File;");

		jclass file_class = jni->FindClass("java/io/File");
		jmethodID get_absolute_path_mid = jni->GetMethodID(
			file_class,
			"getAbsolutePath",
			"()Ljava/lang/String;");

		jobject internal_file_object = jni->CallObjectMethod(
			activity,
			get_files_dir_mid);

		jobject external_file_object = jni->CallObjectMethod(
			activity,
			get_external_files_dir_mid,
			nullptr);

		jstring internal_data_path = static_cast<jstring>(jni->CallObjectMethod(
			internal_file_object,
			get_absolute_path_mid));

		jstring external_data_path = static_cast<jstring>(jni->CallObjectMethod(
			external_file_object,
			get_absolute_path_mid));

		const char* native_internal_data_path = jni->GetStringUTFChars(internal_data_path, nullptr);
		const char* native_external_data_path = jni->GetStringUTFChars(external_data_path, nullptr);

		helper.m_internal_data_path = std::string(native_internal_data_path);
		helper.m_external_data_path = std::string(native_external_data_path);

		helper.m_asset_manager = AAssetManager_fromJava(jni, helper.m_java_asset_manager);
		helper.m_main_looper   = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);

		jni->ReleaseStringUTFChars(package_name, appname);
		jni->ReleaseStringUTFChars(internal_data_path, native_internal_data_path);
		jni->ReleaseStringUTFChars(external_data_path, native_external_data_path);
	}

	JavaVM* JNIHelper::get_java_vm()
	{
		return m_java_virtual_machine;
	}

	AAssetManager* JNIHelper::get_asset_manager()
	{
		return m_asset_manager;
	}

	std::string JNIHelper::get_internal_data_path()
	{
		return m_internal_data_path;
	}

	std::string JNIHelper::get_external_data_path()
	{
		return m_external_data_path;
	}
}

