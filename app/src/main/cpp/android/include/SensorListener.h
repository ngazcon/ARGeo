#pragma once

#include "SensorEvent.h"

namespace argeo
{
	class Sensor;

	class SensorListener
	{
	public:

		virtual ~SensorListener()
		{ }

		virtual void on_accuracy_changed(Sensor* sensor, int accuracy) = 0;
		virtual void on_sensor_changed(SensorEvent event) = 0;
	};
}
