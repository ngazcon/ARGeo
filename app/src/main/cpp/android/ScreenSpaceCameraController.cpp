#include "ScreenSpaceCameraController.h"

#include "ArgeoMath.h"
#include "Quaternion.h"

#include "InputManager.h"
#include "include/SensorManager.h"
#include "Transforms.h"
#include "Log.h"

#include <vector>

namespace argeo
{
	ScreenSpaceCameraController::ScreenSpaceCameraController(ITerrain* terrain)
		: m_terrain(terrain)
		, m_zoom(45.0f)
		, m_translation_speed(0.0000001f)
		, m_rotation_speed(0.001f)
		, m_world_up_enu(0.0f, 0.0f, 1.0f)
		, m_world_direction_enu(0.0f, 1.0f, 0.0f)
		, m_world_right_enu(1.0f, 0.0f, 0.0f)
		, m_altitude_changed(false)
		, m_orientation_changed(false)
		, m_rotation_enabled(false)
		, m_translation_enabled(false)
		, m_ground_height(0.0)
		, m_height_from_ground(1.5)
		, m_yaw_offset(0.0)
		, m_pitch_offset(0.0)
		, m_roll_offset(0.0)
		, m_current_yaw_offset(0.0)
		, m_current_pitch_offset(0.0)
		, m_current_roll_offset(0.0)
		, m_last_point(0.0f, 0.0f)
	{
		set_height_from_ground(m_height_from_ground);
		set_translation_enabled(true);
		set_rotation_enabled(true);
	}


	ScreenSpaceCameraController::~ScreenSpaceCameraController()
	{
		set_translation_enabled(false);
		set_rotation_enabled(false);
	}


	void ScreenSpaceCameraController::set_rotation_enabled(bool value)
	{
		if (value != m_rotation_enabled)
		{
			m_rotation_enabled = value;

			if (m_rotation_enabled && !m_translation_enabled)
			{
				InputManager::add_gesture_listener(this);
			}
			else if (!m_rotation_enabled && !m_translation_enabled)
			{
				InputManager::remove_gesture_listener(this);
			}
		}
	}

	void ScreenSpaceCameraController::set_translation_enabled(bool value)
	{
		if (value != m_translation_enabled)
		{
			m_translation_enabled = value;

			if (!m_rotation_enabled && m_translation_enabled)
			{
				InputManager::add_gesture_listener(this);
			}
			else if (!m_rotation_enabled && !m_translation_enabled)
			{
				InputManager::remove_gesture_listener(this);
			}
		}
	}
	
	Geodetic3D ScreenSpaceCameraController::get_position_offset() const
	{
		return m_position_offset;
	}

	double ScreenSpaceCameraController::get_translation_speed() const
	{
		return m_translation_speed;
	}

	void ScreenSpaceCameraController::set_translation_speed(double value)
	{
		m_translation_speed = value;
	}

	double ScreenSpaceCameraController::get_rotation_speed() const
	{
		return m_rotation_speed;
	}

	void ScreenSpaceCameraController::set_rotation_speed(double value)
	{
		m_rotation_speed = value;
	}

	double ScreenSpaceCameraController::get_yaw_offset() const
	{
		return m_yaw_offset;
	}

	double ScreenSpaceCameraController::get_pitch_offset() const
	{
		return m_pitch_offset;
	}

	double ScreenSpaceCameraController::get_roll_offset() const
	{
		return m_roll_offset;
	}

	float ScreenSpaceCameraController::get_height_from_ground() const
	{
		return m_height_from_ground;
	}

	void ScreenSpaceCameraController::set_height_from_ground(float height)
	{
		if (height != m_height_from_ground)
		{
			m_height_from_ground = height;

			std::unique_lock<std::mutex> lock(m_altitude_mutex);
			m_altitude_changed = true;
		}
	}

	bool ScreenSpaceCameraController::on_rotate_event(
		const TouchEvent& event,
		const float& radians)
	{
		if (!m_rotation_enabled)
		{
			return false;
		}

		std::unique_lock<std::mutex> lock(m_orientation_mutex);
		m_orientation_changed = true;
		m_yaw_offset += radians * m_rotation_speed;

		return true;
	}

	bool ScreenSpaceCameraController::on_drag_event(
		const TouchEvent& event,
		const DragState& state)
	{
		if (!m_translation_enabled)
		{
			return false;
		}

		//Handle drag state 
		if (state == DragState::Move)
		{
			Point drag_delta(
				event.point.x - m_last_point.x,
				event.point.y - m_last_point.y);

			std::unique_lock<std::mutex> lock(m_orientation_mutex);
			
			m_yaw_offset   = drag_delta.x * m_rotation_speed;
			m_pitch_offset = drag_delta.y * m_rotation_speed;

			m_orientation_changed = true;

			//vec2 forward(
			//	cos(m_yaw),
			//	sin(m_yaw));
			//forward = forward * drag_delta.x;
			//
			//vec2 right(
			//	cos(m_yaw + PI / 2),
			//	sin(m_yaw + PI / 2));
			//right = right * drag_delta.y;
			//
			//m_position_offset = Geodetic2D(
			//	m_position_offset.get_longitude() + (forward.x + right.x) * m_translation_speed,
			//	m_position_offset.get_latitude()  + (forward.y + right.y) * m_translation_speed);
			//
			//
			//std::unique_lock<std::mutex> altitude_lock(m_altitude_mutex);
			//m_altitude_changed = true;
		}

		m_last_point = event.point;
		return true;
	}

	void ScreenSpaceCameraController::update(Camera* camera)
	{
		return;

		std::unique_lock<std::mutex> location_lock(m_location_mutex);
		std::unique_lock<std::mutex> altitude_lock(m_altitude_mutex);

		Geodetic2D position2D(
			m_terrain->get_ellipsoid().to_geodetic2D(camera->get_position_wc()) + m_position_offset);

		if (m_altitude_changed)
		{
			m_ground_height = m_terrain->get_point_height(position2D);
		}

		Geodetic3D position3D(
			position2D,
			m_ground_height + m_height_from_ground);


		std::unique_lock<std::mutex> orientation_lock(m_orientation_mutex);
		if (m_orientation_changed)
		{
			const double delta_yaw   = m_current_yaw_offset   - m_yaw_offset;
			const double delta_pitch = m_current_pitch_offset - m_pitch_offset;
			const double delta_roll  = m_current_roll_offset  - m_roll_offset;

			camera->look_right(delta_yaw);
			camera->look_up(delta_pitch);
			camera->twist_right(delta_roll);
		}

		//if (m_altitude_changed || m_orientation_changed)
		{
			//Ellipsoid ellipsoid = m_terrain->get_ellipsoid();
			//
			//camera->set_eye(ellipsoid.to_geocentric3D(position3D).to_vec3d());
			//camera->set_up(up_ecef);
			//camera->set_target(camera->get_eye() + front_ecef);
		}

		m_orientation_changed = m_altitude_changed = false;
	}
}
