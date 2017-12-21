#pragma once

#include "Camera.h"
#include "Ellipsoid.h"
#include "Quaternion.h"

#include "Sensor.h"
#include "SensorListener.h"
#include "LocationManager.h"
#include "LocationFilter.h"
#include "Tracker1D.h"
#include "GestureListener.h"
#include "GPSListener.h"

#include "ICameraController.h"
#include "ITerrain.h"

#include <mutex>

namespace argeo
{
	class DevicePoseCameraController
		: public SensorListener
		, public LocationListener
		, public ICameraController
	{
	public:
		DevicePoseCameraController(ITerrain* terrain);

		~DevicePoseCameraController();

		void set_position(Geodetic3D value);
		void set_position(Geodetic2D value);

		Geodetic3D get_position() const;

        // SENSOR FILTER
        void   set_sensor_low_pass_filter_alpha(const double& value);
        double get_sensor_low_pass_filter_alpha() const;

        // GPS LISTENER
        GPSState get_gps_state();
        void set_network_standing_frec(const int& value);
        int  get_network_standing_frec() const;

        void set_network_moving_frec(const int& value);
        int  get_network_moving_frec() const;

        void set_gps_moving_frec(const int& value);
        int  get_gps_moving_frec() const;

        void set_gps_standing_frec(const int& value);
        int  get_gps_standing_frec() const;

        void set_moving_window(const int& value);
        int  get_moving_window() const;

        void set_standing_window(const int& value);
        int  get_standing_window() const;

        void set_initial_samples(const int& value);
        int  get_initial_samples() const;

        void set_accuracy_delta(const int& value);
        int  get_accuracy_delta() const;

        void force_location_updates();



		double get_yaw() const;
		double get_pitch() const;
		double get_roll() const;

		void disable_location_updates();
        void enable_location_updates();

        void disable_orientation_updates();
		void enable_orientation_updates();

		void update(Camera* camera);

	private:
		Geodetic2D m_position;

        std::function<void()> m_remove_update_height_callback;
        std::function<void(vec3d&)>  m_update_height_function;

		ITerrain* m_terrain;
		
		double m_ground_height;
        double m_sensor_low_pass_alpha;

		// Euler angles
		double m_yaw;
		double m_pitch;
		double m_roll;

		double m_current_yaw;
		double m_current_pitch;
		double m_current_roll;

		double m_geomagnetic_declination;

	private:
		unsigned int m_frame_count;

		void on_location_changed(const LocationChangedEventArgs& args);
		void on_provider_enabled(const std::string& provider);
		void on_provider_disabled(const std::string& provider);
		void on_status_changed(const std::string& provider, const int& status);

		void on_accuracy_changed(Sensor* sensor, int accuracy);
		void on_sensor_changed(SensorEvent event);

		std::unique_ptr<GPSListener> m_gps_listener;



		bool m_has_location_provider_height;

		bool m_location_updates_enabled;
		bool m_orientation_updates_enabled;

		bool m_location_changed;
		bool m_orientation_changed;
		bool m_first_update;

		std::mutex m_altitude_mutex;
		std::mutex m_location_mutex;
		std::mutex m_orientation_mutex;
	};
}