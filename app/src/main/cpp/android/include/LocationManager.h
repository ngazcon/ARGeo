#pragma once

#include "Geodetic3D.h"

#include <cmath>
#include <unordered_set>
#include <map>
#include <jni.h>

namespace argeo
{

	struct BearingDistanceCache
	{
		double lat1 = 0.0;
		double lon1 = 0.0;
		double lat2 = 0.0;
		double lon2 = 0.0;
		float  distance = 0.0f;
		float  initial_bearing = 0.0f;
		float  final_bearing = 0.0f;
	};

	static BearingDistanceCache s_bearing_distance_cache;

	struct LocationChangedEventArgs;
	static void compute_distance_and_bearing(
			const double& lat1,
			const double& lon1,
			const double& lat2,
			const double& lon2,
			BearingDistanceCache& results)
	{
		int MAXITERS = 20;


		double a = 6378137.0; // WGS84 major axis
		double b = 6356752.3142; // WGS84 semi-major axis
		double f = (a - b) / a;
		double aSqMinusBSqOverBSq = (a * a - b * b) / (b * b);
		double L = lon2 - lon1;
		double A = 0.0;
		double U1 = ::atan((1.0 - f) * ::tan(lat1));
		double U2 = ::atan((1.0 - f) * ::tan(lat2));
		double cosU1 = ::cos(U1);
		double cosU2 = ::cos(U2);
		double sinU1 = ::sin(U1);
		double sinU2 = ::sin(U2);
		double cosU1cosU2 = cosU1 * cosU2;
		double sinU1sinU2 = sinU1 * sinU2;
		double sigma = 0.0;
		double deltaSigma = 0.0;
		double cosSqAlpha = 0.0;
		double cos2SM = 0.0;
		double cosSigma = 0.0;
		double sinSigma = 0.0;
		double cosLambda = 0.0;
		double sinLambda = 0.0;
		double lambda = L; // initial guess

		for (int iter = 0; iter < MAXITERS; iter++)
		{
			double lambdaOrig = lambda;
			cosLambda = ::cos(lambda);
			sinLambda = ::sin(lambda);
			double t1 = cosU2 * sinLambda;
			double t2 = cosU1 * sinU2 - sinU1 * cosU2 * cosLambda;
			double sinSqSigma = t1 * t1 + t2 * t2; // (14)
			sinSigma = ::sqrt(sinSqSigma);
			cosSigma = sinU1sinU2 + cosU1cosU2 * cosLambda; // (15)
			sigma = ::atan2(sinSigma, cosSigma); // (16)
			double sinAlpha = (sinSigma == 0) ? 0.0 :
							  cosU1cosU2 * sinLambda / sinSigma; // (17)
			cosSqAlpha = 1.0 - sinAlpha * sinAlpha;
			cos2SM = (cosSqAlpha == 0) ? 0.0 :
					 cosSigma - 2.0 * sinU1sinU2 / cosSqAlpha; // (18)
			double uSquared = cosSqAlpha * aSqMinusBSqOverBSq; // defn
			A = 1 + (uSquared / 16384.0) * // (3)
					(4096.0 + uSquared *
							  (-768 + uSquared * (320.0 - 175.0 * uSquared)));
			double B = (uSquared / 1024.0) * // (4)
					   (256.0 + uSquared *
								(-128.0 + uSquared * (74.0 - 47.0 * uSquared)));
			double C = (f / 16.0) *
					   cosSqAlpha *
					   (4.0 + f * (4.0 - 3.0 * cosSqAlpha)); // (10)
			double cos2SMSq = cos2SM * cos2SM;
			deltaSigma = B * sinSigma * // (6)
						 (cos2SM + (B / 4.0) *
								   (cosSigma * (-1.0 + 2.0 * cos2SMSq) -
									(B / 6.0) * cos2SM *
									(-3.0 + 4.0 * sinSigma * sinSigma) *
									(-3.0 + 4.0 * cos2SMSq)));
			lambda = L +
					 (1.0 - C) * f * sinAlpha *
					 (sigma + C * sinSigma *
							  (cos2SM + C * cosSigma *
										(-1.0 + 2.0 * cos2SM * cos2SM))); // (11)
			double delta = (lambda - lambdaOrig) / lambda;

			if (::fabs(delta) < 1.0e-12)
			{
				break;
			}
		}

		float distance = (float) (b * A * (sigma - deltaSigma));
		results.distance = distance;
		float initialBearing = (float) ::atan2(cosU2 * sinLambda,
											   cosU1 * sinU2 - sinU1 * cosU2 * cosLambda);
		initialBearing *= 180.0 / PI;
		results.initial_bearing = initialBearing;
		float finalBearing = (float) atan2(cosU1 * sinLambda,
										   -sinU1 * cosU2 + cosU1 * sinU2 * cosLambda);
		finalBearing *= 180.0 / PI;
		results.final_bearing = finalBearing;
		results.lat1 = lat1;
		results.lat2 = lat2;
		results.lon1 = lon1;
		results.lon2 = lon2;
	}

	struct LocationChangedEventArgs
	{
		LocationChangedEventArgs(
				const Geodetic3D& location = Geodetic3D(),
				const double& accuracy    = std::numeric_limits<double>::max(),
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

		float distance_to(const LocationChangedEventArgs& dest) const
		{
			BearingDistanceCache cache = s_bearing_distance_cache;
			// See if we already have the result
			if (location.get_latitude() != cache.lat1 || location.get_longitude() != cache.lon1 ||
				dest.location.get_latitude() != cache.lat2 || dest.location.get_longitude() != cache.lon2)
			{
				compute_distance_and_bearing(
						location.get_latitude(),
						location.get_longitude(),
						dest.location.get_latitude(),
						dest.location.get_longitude(),
						cache);
			}
			return cache.distance;
		}
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
		//                Class pointers
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
		//                Method pointers
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
		//                Native methods
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