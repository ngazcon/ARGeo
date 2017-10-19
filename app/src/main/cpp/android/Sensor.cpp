#include "include/Sensor.h"

namespace argeo
{
	Sensor::Sensor(SensorType type)
		: m_type(type)
	{		
		m_sensor = ASensorManager_getDefaultSensor(
			ASensorManager_getInstance(),
			static_cast<int>(m_type));
		
		if (m_sensor == nullptr)
		{
			throw std::runtime_error("No accelerometer found.");
		}

		m_min_delay = ASensor_getMinDelay(m_sensor);
	}

	Sensor::~Sensor()
	{ 
		for (sensor_queue_data* data : m_queues)
		{
			unregister_listener(data->listener);
			ASensorManager_destroyEventQueue(
				ASensorManager_getInstance(),
				data->queue);
		}
	}

	bool Sensor::register_listener(
		SensorListener* listener, 
		int sampling_period_us,
		ALooper* looper)
	{
		sensor_queue_data* data = new sensor_queue_data();
		data->listener = listener;
		data->sensor   = this;
		data->queue    = ASensorManager_createEventQueue(
			ASensorManager_getInstance(),
			looper,
			ALOOPER_POLL_CALLBACK,
			&Sensor::get_sensor_events,
			data);

		if (ASensorEventQueue_enableSensor(
			data->queue,	
			m_sensor) < 0)
		{
			return false;
		}

		if (m_min_delay > 0)
		{
			int32_t delay = 0;

			switch (sampling_period_us)
			{
			case SensorDelay::Fastest:
				delay = m_min_delay;
				break;

			case SensorDelay::Game:
				delay = m_min_delay + 20000;
				break;

			case SensorDelay::Normal:
				delay = m_min_delay + 200000;
				break;

			case SensorDelay::UI:
				delay = m_min_delay + 60000;
				break;

			default:
				delay = sampling_period_us;
				break;
			}

			if (m_min_delay > delay)
			{
				return false;
			}


			if (ASensorEventQueue_setEventRate(
				data->queue,
				m_sensor,
				delay) < 0)
			{
				return false;
			}
		}
		else
		{
			return false;
		}

		m_queues.push_back(data);

 		return true;
	}


	bool Sensor::unregister_listener(SensorListener* listener)
	{
		for (auto queue_data : m_queues)
		{
			if (queue_data->listener == listener)
			{
				if (ASensorEventQueue_disableSensor(
					queue_data->queue,
					m_sensor) < 0)
				{	
					return false;
				}

				return true;
			}
		}

		return false;
	}

	int32_t Sensor::get_sensor_events(int fd, int events, void* data)
	{
		sensor_queue_data* queue_data = reinterpret_cast<sensor_queue_data*>(data);
		return queue_data->sensor->process_sensor_event(queue_data);
	}

	int32_t Sensor::process_sensor_event(sensor_queue_data* queue_data)
	{
		ASensorEvent event;
		while (ASensorEventQueue_getEvents(
			queue_data->queue,
			&event,
			1) > 0)
		{
			SensorType type = static_cast<SensorType>(event.type);

			float accuracy = ASensor_getResolution(m_sensor);

			queue_data->listener->on_sensor_changed(
				SensorEvent(
					this,
					accuracy,
					event.timestamp,
					event.data));
		}

		return 1;
	}

	Sensor::SensorType Sensor::get_type() const
	{
		return m_type;
	}

}
