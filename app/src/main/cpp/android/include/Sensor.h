#pragma once

#include "SensorListener.h"
#include <android/sensor.h>

#include <unordered_map>

namespace argeo
{
	/* Android sensor types - this should match the defines/enumeration in Android's sensors.h */
	/* Note that these are defined as SENSOR_ instead of SENSOR_TYPE_ to avoid clash with Android defines in
	*  situations where this header in included for packet processing in Sensor HAL
	*/
	//typedef enum ASensorType
	//{
	//	SENSOR_META_DATA = 0,
	//	SENSOR_ACCELEROMETER = 1,
	//	SENSOR_GEOMAGNETIC_FIELD = 2,
	//	SENSOR_MAGNETIC_FIELD = SENSOR_GEOMAGNETIC_FIELD,
	//	SENSOR_ORIENTATION = 3,
	//	SENSOR_GYROSCOPE = 4,
	//	SENSOR_LIGHT = 5,
	//	SENSOR_PRESSURE = 6,
	//	SENSOR_TEMPERATURE = 7,
	//	SENSOR_PROXIMITY = 8,
	//	SENSOR_GRAVITY = 9,
	//	SENSOR_LINEAR_ACCELERATION = 10,
	//	SENSOR_ROTATION_VECTOR = 11,
	//	SENSOR_RELATIVE_HUMIDITY = 12,
	//	SENSOR_AMBIENT_TEMPERATURE = 13,
	//	SENSOR_MAGNETIC_FIELD_UNCALIBRATED = 14,
	//	SENSOR_GAME_ROTATION_VECTOR = 15,
	//	SENSOR_GYROSCOPE_UNCALIBRATED = 16,
	//	SENSOR_SIGNIFICANT_MOTION = 17,
	//	SENSOR_STEP_DETECTOR = 18,
	//	SENSOR_STEP_COUNTER = 19,
	//	SENSOR_GEOMAGNETIC_ROTATION_VECTOR = 20,
	//	SENSOR_HEART_RATE = 21,
	//	SENSOR_WAKE_UP_TILT_DETECTOR = 22,
	//	SENSOR_TILT_DETECTOR = 23,
	//	SENSOR_POSE_6DOF = 28,
	//};

	
	class Sensor
	{
	public:
		typedef enum class ReportingModes
		{
			Continous = 0,
			OnChange  = 1,
			OneShot   = 2,
			SpecialTrigger = 3,
		};

		typedef enum class SensorType
		{
			MetaData = 0,
			Accelerometer = 1,
			GeomagneticField = 2,
			MagneticField = GeomagneticField,
			Orientation = 3,
			Gyroscope = 4,
			Light = 5,
			Pressure = 6,
			Temperature = 7,
			Proximity = 8,
			Gravity = 9,
			LinearAcceleration = 10,
			RotationVector = 11,
			RelativeHumity = 12,
			AmbientTemperature = 13,
			MagneticFieldUncalibrated = 14,
			GameRotationVector = 15,
			GyroscopeUncalibrated = 16,
			SignificantMotion = 17,
			StepDetector = 18,
			StepCounter = 19,
			GeomagneticRotationVector = 20,
			HeartRate = 21,
			WakeUpTiltDetector = 22,
			TiltDetector = 23,
			Pose6DOF = 28,
		};

		typedef enum SensorDelay
		{
			Fastest = 0,
			Game = 1,
			Normal = 2,
			UI = 3,
		};


		friend class SensorManager;

		struct sensor_queue_data
		{
			Sensor* sensor;
			ASensorEventQueue* queue;
			SensorListener* listener;
		};

	public:
		Sensor(SensorType type);

		~Sensor();
		
		SensorType get_type() const;

	private:
		bool register_listener(
			SensorListener* listener, 
			int sampling_period_us,
			ALooper* looper);

		bool unregister_listener(SensorListener* listener);

		int32_t process_sensor_event(sensor_queue_data* queue_data);

		static int32_t get_sensor_events(int fd, int events, void* data);

	private:
		const ASensor* m_sensor;
		const SensorType m_type;
		
		int32_t m_min_delay;
		ReportingModes m_reporting_mode;

		std::vector<sensor_queue_data*> m_queues;
	
	};
}
