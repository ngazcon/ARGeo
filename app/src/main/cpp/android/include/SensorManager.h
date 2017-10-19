#pragma once

#include "Sensor.h"
#include "SensorListener.h"
#include "Matrix.h"

#include <memory>
#include <map>

namespace argeo
{
	class SensorManager
	{
		friend class Sensor;

	public:
		/** Standard gravity (g) on Earth. This value is equivalent to 1G */
		static constexpr float STANDARD_GRAVITY = 9.80665f;
		
		/** Sun's gravity in SI units (m/s^2) */
	    static constexpr float GRAVITY_SUN = 275.0f;
		/** Mercury's gravity in SI units (m/s^2) */
	    static constexpr float GRAVITY_MERCURY = 3.70f;
		/** Venus' gravity in SI units (m/s^2) */
	    static constexpr float GRAVITY_VENUS = 8.87f;
		/** Earth's gravity in SI units (m/s^2) */
	    static constexpr float GRAVITY_EARTH = 9.80665f;
		/** The Moon's gravity in SI units (m/s^2) */
	    static constexpr float GRAVITY_MOON = 1.6f;
		/** Mars' gravity in SI units (m/s^2) */
	    static constexpr float GRAVITY_MARS = 3.71f;
		/** Jupiter's gravity in SI units (m/s^2) */
	    static constexpr float GRAVITY_JUPITER = 23.12f;
		/** Saturn's gravity in SI units (m/s^2) */
	    static constexpr float GRAVITY_SATURN = 8.96f;
		/** Uranus' gravity in SI units (m/s^2) */
	    static constexpr float GRAVITY_URANUS = 8.69f;
		/** Neptune's gravity in SI units (m/s^2) */
	    static constexpr float GRAVITY_NEPTUNE = 11.0f;
		/** Pluto's gravity in SI units (m/s^2) */
	    static constexpr float GRAVITY_PLUTO = 0.6f;
		/** Gravity (estimate) on the first Death Star in Empire units (m/s^2) */
	    static constexpr float GRAVITY_DEATH_STAR_I = 0.000000353036145f;
		/** Gravity on the island */
	    static constexpr float GRAVITY_THE_ISLAND = 4.815162342f;
		
		
		 /** Maximum magnetic field on Earth's surface */
	    static constexpr float MAGNETIC_FIELD_EARTH_MAX = 60.0f;
		/** Minimum magnetic field on Earth's surface */
	    static constexpr float MAGNETIC_FIELD_EARTH_MIN = 30.0f;
		
		
	    /** Standard atmosphere, or average sea-level pressure in hPa (millibar) */
	   static constexpr float PRESSURE_STANDARD_ATMOSPHERE = 1013.25f;
		
		
	    /** Maximum luminance of sunlight in lux */
	    static constexpr float LIGHT_SUNLIGHT_MAX = 120000.0f;
		/** luminance of sunlight in lux */
	    static constexpr float LIGHT_SUNLIGHT = 110000.0f;
		/** luminance in shade in lux */
	    static constexpr float LIGHT_SHADE = 20000.0f;
		/** luminance under an overcast sky in lux */
	    static constexpr float LIGHT_OVERCAST = 10000.0f;
		/** luminance at sunrise in lux */
	    static constexpr float LIGHT_SUNRISE = 400.0f;
		/** luminance under a cloudy sky in lux */
	    static constexpr float LIGHT_CLOUDY = 100.0f;
		/** luminance at night with full moon in lux */
	    static constexpr float LIGHT_FULLMOON = 0.25f;
		/** luminance at night with no moon in lux*/
	    static constexpr float LIGHT_NO_MOON = 0.001f;


		static constexpr int AXIS_X = 1;
	    static constexpr int AXIS_Y = 2;
	    static constexpr int AXIS_Z = 3;
	    static constexpr int AXIS_MINUS_X = AXIS_X | 0x80;
	    static constexpr int AXIS_MINUS_Y = AXIS_Y | 0x80;
	    static constexpr int AXIS_MINUS_Z = AXIS_Z | 0x80;


	public:
		static SensorManager& get_instance();
		

		static void get_rotation_matrix_from_vector(
			float* rotation_matrix,
			float* rotation_vector,
			int quaternion_size,
			int matrix_size);

		static bool remap_coordinate_system(
			float* in_rotation_matrix,
			int X,
			int Y,
			float* out_rotation_matrix,
			int matrix_size);
		
		static float* get_orientation(
			float* rotation_matrix,
			float* orientation, 
			int matrix_size);

		Sensor* get_default_sensor(Sensor::SensorType type);

		bool add_sensor_listener(
			SensorListener* listener,
			Sensor* sensor,
			int sampling_period_us);

		bool remove_sensor_listener(SensorListener* listener);

		void loop();

	private:
		static bool remap_coordinate_system_impl(
			float* in_rotation_matrix,
			int X,
			int Y,
			float* out_rotation_matrix,
			int matrix_size);

	private:
		SensorManager();
		~SensorManager();

	public:
		// C++ 11
		// =======
		// We can use the better technique of deleting the methods
		// we don't want.
		SensorManager(SensorManager const&)  = delete;
		void operator=(SensorManager const&) = delete;

	private:

		std::map<Sensor::SensorType, Sensor*> m_sensors;

	private:
		ASensorManager* m_sensor_manager;
	};
}
