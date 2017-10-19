#include "include/LocationManager.h"
#include "include/JNIHelper.h"
#include "ArgeoMath.h"
#include "Log.h"
#include "include/JavaString.h"

#include <stdexcept>

namespace argeo
{
	LocationManager::LocationManager()
	{ }

	LocationManager::~LocationManager()
	{
		JNIEnv* jenv;
		m_java_vm->AttachCurrentThread(&jenv, nullptr);

		// Delete all global references
		jenv->DeleteGlobalRef(m_location_listener_class);
		jenv->DeleteGlobalRef(m_context_object);
		jenv->DeleteGlobalRef(m_system_service_object);
		jenv->DeleteGlobalRef(m_looper_object);

		jenv->DeleteGlobalRef(m_location_listener_class);
		jenv->DeleteGlobalRef(m_looper_class);
		jenv->DeleteGlobalRef(m_location_manager_class);
		jenv->DeleteGlobalRef(m_location_provider_class);
		jenv->DeleteGlobalRef(m_location_object_class);
		jenv->DeleteGlobalRef(m_activity_class);

		m_java_vm->DetachCurrentThread();
	}

	LocationManager& LocationManager::get_instance()
	{
		static LocationManager instance;
		return instance;
	}

	void LocationManager::init(
		jobject context_object,
		jclass  location_listener_class,
		JavaVM* java_virtual_machine,
		JNIEnv* java_native_interface_environment)
	{
		m_location_listener_class = static_cast<jclass>(
			java_native_interface_environment->NewGlobalRef(location_listener_class));

		m_context_object = static_cast<jobject>(
			java_native_interface_environment->NewGlobalRef(context_object));

		m_java_vm = java_virtual_machine;

		//Load the classes
		load_android_classes(java_native_interface_environment);

		//Call the android methods and bind Java with C++ using registeNatives
		load_android_methods(java_native_interface_environment);
	}

	void LocationManager::request_location_updates(
		const std::string& provider,
		const long& min_time,
		const float& min_distance,
		LocationListener* listener)
	{
		JNIEnv* jenv;
		jint result = JNIHelper::get_instance().attach_current_thread(&jenv);

		std::string error;

		//Creates a new listener class
		jobject location_listener_object = jenv->NewObject(
			m_location_listener_class,
			m_location_listener_constructor_method_id,
			reinterpret_cast<jlong>(listener));

		if (JNIHelper::get_instance().check_exception(jenv, error) ||
			location_listener_object == nullptr)
		{
			Log::error(error.c_str());
			throw std::runtime_error(error);
		}

		jstring args = jenv->NewStringUTF(provider.c_str());

		jenv->CallVoidMethod(
			m_system_service_object,
			m_request_location_updates_method_id,
			args,
			(jlong)min_time,
			(jfloat)min_distance,
			location_listener_object,
			m_looper_object);

		if (JNIHelper::get_instance().check_exception(jenv, error))
		{
			Log::error(error.c_str());
			throw std::runtime_error(error);
		}

		m_listeners_objects[listener] = jenv->NewGlobalRef(location_listener_object);
		m_listeners.insert(listener);

		JNIHelper::get_instance().detach_current_thread(result);
	}

	void LocationManager::remove_updates(LocationListener* listener)
	{
		if (m_listeners.find(listener) == m_listeners.end())
		{
			throw std::runtime_error("The given listener is not register.");
		}

		JNIEnv* jenv;
		jint result = JNIHelper::get_instance().attach_current_thread(&jenv);

		jobject location_listener_object = m_listeners_objects[listener];
		jenv->CallVoidMethod(
			m_system_service_object,
			m_remove_updates_method_id,
			location_listener_object);

		jenv->DeleteGlobalRef(location_listener_object);

		m_listeners.erase(listener);
		m_listeners_objects.erase(listener);

		JNIHelper::get_instance().detach_current_thread(result);
	}

	//This function load the neccesary Android classes and return true if all good
	void LocationManager::load_android_classes(JNIEnv* jenv)
	{
		std::string error;

		//We try to find all the neccesary Android classes
		m_location_manager_class = jenv->FindClass("android/location/LocationManager");
		if (JNIHelper::get_instance().check_exception(jenv, error) ||
			m_location_manager_class == nullptr)
		{
			throw std::runtime_error("Cannot find location Manager.");
		}
		m_location_manager_class = static_cast<jclass>(
			jenv->NewGlobalRef(m_location_manager_class));

		m_location_provider_class = jenv->FindClass("android/location/LocationProvider");
		if (JNIHelper::get_instance().check_exception(jenv, error) ||
			m_location_provider_class == nullptr)
		{
			throw std::runtime_error("Cannot find location Provider.");
		}
		m_location_provider_class = static_cast<jclass>(
			jenv->NewGlobalRef(m_location_provider_class));

		m_looper_class = jenv->FindClass("android/os/Looper");
		if (JNIHelper::get_instance().check_exception(jenv, error) ||
			m_looper_class == nullptr)
		{
			throw std::runtime_error("Cannot find looper Object.");
		}
		m_looper_class = static_cast<jclass>(
			jenv->NewGlobalRef(m_looper_class));

		m_location_object_class = jenv->FindClass("android/location/Location");
		if (JNIHelper::get_instance().check_exception(jenv, error) ||
			m_location_object_class == nullptr)
		{
			throw std::runtime_error("Cannot find location Object.");
		}
		m_location_object_class = static_cast<jclass>(
			jenv->NewGlobalRef(m_location_object_class));

		m_activity_class = jenv->GetObjectClass(m_context_object);
		if (JNIHelper::get_instance().check_exception(jenv, error) ||
			m_activity_class == nullptr)
		{
			throw std::runtime_error("Cannot find Activity.");
		}
		m_activity_class = static_cast<jclass>(
			jenv->NewGlobalRef(m_activity_class));
	}

	void LocationManager::load_android_methods(JNIEnv* jenv)
	{
		std::string error;

		m_get_system_service_method_id = jenv->GetMethodID(
			m_activity_class,
			"getSystemService",
			"(Ljava/lang/String;)Ljava/lang/Object;");
		if (JNIHelper::get_instance().check_exception(jenv, error) ||
			m_get_system_service_method_id == nullptr)
		{
			throw std::runtime_error("Cannot get method getSystemService().");
		}

		//We get a java String from a normal c++ string
		jstring args = jenv->NewStringUTF("location");

		//We get the system service
		m_system_service_object = jenv->CallObjectMethod(
			m_context_object,
			m_get_system_service_method_id,
			args);
		if (JNIHelper::get_instance().check_exception(jenv, error) ||
			m_system_service_object == nullptr)
		{
			throw std::runtime_error("Error at getting the System service.");
		}
		m_system_service_object = static_cast<jobject>(
			jenv->NewGlobalRef(m_system_service_object));

		m_get_main_looper_method_id = jenv->GetStaticMethodID(
			m_looper_class,
			"getMainLooper",
			"()Landroid/os/Looper;");
		if (JNIHelper::get_instance().check_exception(jenv, error) ||
			m_get_main_looper_method_id == nullptr)
		{
			throw std::runtime_error("Cannot get method getMainLooper().");
		}

		m_looper_object = jenv->CallStaticObjectMethod(
			m_looper_class,
			m_get_main_looper_method_id);
		if (JNIHelper::get_instance().check_exception(jenv, error) ||
			m_looper_object == nullptr)
		{
			throw std::runtime_error("Error at getting main looper.");
		}
		m_looper_object = static_cast<jobject>(
			jenv->NewGlobalRef(m_looper_object));

		//Now that we have the system service we need to create a new Location Listener object.
		// Gets the constructor of the Location Listener class. Please note that
		// listenerClass comes from the modified qtmain_android.cpp
		// It will not work with the orignal qtmain_android.cpp from android-lighthouse
		m_location_listener_constructor_method_id = jenv->GetMethodID(
			m_location_listener_class,
			"<init>",
			"(J)V");
		if (JNIHelper::get_instance().check_exception(jenv, error) ||
			m_location_listener_constructor_method_id == nullptr)
		{
			throw std::runtime_error("Error registering Listener constructor.");
		}

		//Declaring the methods. This will be used by registerNatives
		JNINativeMethod methods[] =
		{
			{"locationChanged",  "(JDDDDDJLjava/lang/String;DZZ)V", (void*)&LocationManager::location_changed},
			{"providerDisabled", "(JLjava/lang/String;)V", (void*)&LocationManager::provider_disabled},
			{"providerEnabled",  "(JLjava/lang/String;)V", (void*)&LocationManager::provider_enabled},
			{"statusChanged",    "(JLjava/lang/String;ILandroid/os/Bundle;)V", (void*)&LocationManager::status_changed}
		};


		// Register the native methods.
		// Basically this joins the methods inside the gpsListener.java class
		// with our c++ methods.
		int methods_size = sizeof(methods) / sizeof(methods[0]);
		if (jenv->RegisterNatives(m_location_listener_class, methods, methods_size) < 0 ||
			JNIHelper::get_instance().check_exception(jenv, error))
		{
			throw std::runtime_error("Error running RegisterNatives.");
		}

		//Get the method id of requestLocationUpdates
		m_request_location_updates_method_id = jenv->GetMethodID(
			m_location_manager_class,
			"requestLocationUpdates",
			"(Ljava/lang/String;JFLandroid/location/LocationListener;Landroid/os/Looper;)V");

		if (JNIHelper::get_instance().check_exception(jenv, error) ||
			m_request_location_updates_method_id == nullptr)
		{
			throw std::runtime_error("Cannot get method RequestLocationUpdates().");
		}

		//Get the method id of removeUpdates
		m_remove_updates_method_id = jenv->GetMethodID(
			m_location_manager_class,
			"removeUpdates",
			"(Landroid/location/LocationListener;)V");

		if (JNIHelper::get_instance().check_exception(jenv, error) ||
			m_remove_updates_method_id == nullptr)
		{
			throw std::runtime_error("Cannot get method RemoveUpdates().");
		}
	}

	// Implementation of the native methods bound to the gpsListener using registerNatives
	// basically move the values to external variables. This because they are
	// static methods.. So we cannot access the class from them
	// Maybe someone can help me here on how to use no static methods and make it run!
	void LocationManager::location_changed(
		JNIEnv* env,
		jobject location_listener_object,
		jlong   listener,
		jdouble latitude,
		jdouble longitude,
		jdouble altitude,
		jdouble accuracy,
		jdouble speed,
		jlong   time,
		jstring provider,
		jdouble geomagnetic_declination,
		jboolean has_altitude,
		jboolean has_speed)
	{
		jni::JavaString native_provider(env, provider);
		reinterpret_cast<LocationListener*>(listener)->on_location_changed(LocationChangedEventArgs(
			Geodetic3D(
				ArgeoMath::to_radians(longitude),
				ArgeoMath::to_radians(latitude),
				altitude),
			accuracy,
			speed,
			static_cast<std::int64_t>(time),
			native_provider.get(),
			ArgeoMath::to_radians(geomagnetic_declination),
			has_altitude,
		    has_speed));
	}

	void LocationManager::provider_disabled(
		JNIEnv* env,
		jobject java_this,
		jlong   listener,
		jstring provider)
	{
		jni::JavaString native_provider(env, provider);
		reinterpret_cast<LocationListener*>(listener)->on_provider_disabled(native_provider.get());
	}

	void LocationManager::provider_enabled(
		JNIEnv* env,
		jobject java_this,
		jlong   listener,
		jstring provider)
	{
		jni::JavaString native_provider(env, provider);
		reinterpret_cast<LocationListener*>(listener)->on_provider_enabled(native_provider.get());
	}

	void LocationManager::status_changed(
		JNIEnv* env,
		jobject java_this,
		jlong   listener,
		jstring provider,
		jint status,
		jobject bundle)
	{
		jni::JavaString native_provider(env, provider);
		reinterpret_cast<LocationListener*>(listener)->on_status_changed(native_provider.get(), status);
	}
}