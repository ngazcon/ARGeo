#pragma once

#include "Geodetic3D.h"

#include <unordered_set>
#include <map>
#include <jni.h>

namespace argeo
{
	struct LocationChangedEventArgs
	{
		LocationChangedEventArgs(
			const Geodetic3D& location = Geodetic3D(),
			const double& accuracy	   = std::numeric_limits<double>::max(),
			const double& speed = 0.0,
			const std::int64_t& time    = 0,
			const std::string provider = std::string(),
			const double& geomagnetic_declination = 0.0,
			const bool& has_altitude = false,
			const bool& has_speed = false)
			: location(location)
			, accuracy(accuracy)
			, speed(speed)
			, geomagnetic_declination(geomagnetic_declination)
			, has_altitude(has_altitude)
			, has_speed(has_speed)
			, time(time)
			, provider(provider)
		{ }

		bool has_altitude;
        bool has_speed;
		std::string provider;
		int64_t time;
		Geodetic3D location;
		double geomagnetic_declination;
		double accuracy;
		double speed;
	};

	class LocationListener
	{
	public:
		virtual void on_location_changed(const LocationChangedEventArgs& args) = 0;
		virtual void on_provider_enabled(const std::string& provider) = 0;
		virtual void on_provider_disabled(const std::string& provider) = 0;
		virtual void on_status_changed(const std::string& provider, const int& status) = 0;
	};

	class LocationManager
	{
	public:
		static LocationManager& get_instance();

	public:
		// C++ 11
		// =======
		// We can use the better technique of deleting the methods
		// we don't want.
		LocationManager(LocationManager const&) = delete;
		void operator=(LocationManager const&) = delete;

	private:
		LocationManager();

		~LocationManager();

	public:

		void request_location_updates(
			const std::string& provider,
			const long& min_time,
			const float& min_distance,
			LocationListener* listener);

		void remove_updates(LocationListener* listener);

		void init(
			jobject context_object,
			jclass  location_listener_class,
			JavaVM* java_virtual_machine,
			JNIEnv* java_native_interface_environment);

	private:
		std::unordered_set<LocationListener*> m_listeners;
		std::map<LocationListener*, jobject> m_listeners_objects;

		// Cached java virtual machine.
		JavaVM* m_java_vm;

		// Android Location Listener object
		jobject m_context_object;

		// Android System Service Object
		jobject m_system_service_object;

		// Android Looper Object.
		jobject m_looper_object;

		////////////////////////////////////////////////////////////////
		//						Class pointers
		////////////////////////////////////////////////////////////////

		//---Location Listener
		jclass  m_location_listener_class;

		//---Looper
		jclass m_looper_class;

		//---Location manager
		jclass m_location_manager_class;

		//---Location Provider
		jclass m_location_provider_class;

		//---Location Object
		jclass m_location_object_class;

		//---Activity class
		jclass m_activity_class;


		////////////////////////////////////////////////////////////////
		//						Method pointers
		////////////////////////////////////////////////////////////////

		//---System service
		//Android GetSystemService Method
		jmethodID m_get_system_service_method_id;

		//Android GetMainLooper method
		jmethodID m_get_main_looper_method_id;

		//Android RequestLocationUpdates method
		jmethodID m_request_location_updates_method_id;

		//Android RemoveUpdates method
		jmethodID m_remove_updates_method_id;

		//---Location Listener
		//Android Constructor of the Location Listener calss
		jmethodID m_location_listener_constructor_method_id;

		//Load the neccesary classes
		void load_android_classes(JNIEnv* jenv);

		//Load the android methods and connect the whole thing!
		void load_android_methods(JNIEnv* jenv);


		////////////////////////////////////////////////////////////////
		//						Native methods
		////////////////////////////////////////////////////////////////

		//public methods to be linked with registerNatived
		static void location_changed(
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
			jboolean has_speed);

		static void provider_disabled(
			JNIEnv* env,
			jobject java_this,
			jlong   listener,
			jstring provider);

		static void provider_enabled(
			JNIEnv* env,
			jobject java_this,
			jlong   listener,
			jstring provider);

		static void status_changed(
			JNIEnv* env,
			jobject java_this,
			jlong   listener,
			jstring provider,
			jint status,
			jobject extras);
	};
}