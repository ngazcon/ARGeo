#pragma once

#include <algorithm>
#include <vector>

namespace argeo
{
	class Sensor;

	struct SensorEvent
	{
		SensorEvent(
			Sensor* sensor,
			int accuracy,
			long time_stamp,
			float* vector)
			: accuracy(accuracy)
			, time_stamp(time_stamp)			
		{ 
			std::copy_n(
				vector,
				8,
				this->vector);
		}

		const Sensor* sensor;
		const int accuracy;
		const long time_stamp;

		union {
			float vector[8];
			struct {
				float x;
				float y;
				float z;
			};
			struct {
				float azimuth;
				float pitch;
				float roll;
			};
		};

	};

}
