#pragma once

#include "Camera.h"
#include "Ellipsoid.h"
#include "Quaternion.h"

#include "GestureListener.h"
#include "ICameraController.h"
#include "ITerrain.h"

#include <mutex>

namespace argeo
{
	class ScreenSpaceCameraController
		: public ICameraController
		, public GestureListener
	{
	public:
		ScreenSpaceCameraController(ITerrain* terrain);
		~ScreenSpaceCameraController();

		Geodetic3D get_position_offset() const;

		double get_yaw_offset() const;
		double get_pitch_offset() const;
		double get_roll_offset() const;

		double get_translation_speed() const;
		void   set_translation_speed(double value);

		double get_rotation_speed() const;
		void   set_rotation_speed(double value);

		void set_rotation_enabled(bool value);
		void set_translation_enabled(bool value);

		float get_height_from_ground() const;
		void  set_height_from_ground(float height);

		bool on_drag_event(
			const TouchEvent& args,
			const DragState& state);

		bool on_rotate_event(
			const TouchEvent& args,
			const float& radians);

		void update(Camera* camera);

	private:
		Geodetic2D m_position_offset;

		vec3d m_world_up_enu;
		vec3d m_world_direction_enu;
		vec3d m_world_right_enu;

		ITerrain* m_terrain;

		double m_ground_height;

		double m_yaw_offset;
		double m_pitch_offset;
		double m_roll_offset;

		double m_current_yaw_offset;
		double m_current_pitch_offset;
		double m_current_roll_offset;

		// Camera options
		double m_translation_speed;
		double m_rotation_speed;
		double m_height_from_ground;
		double m_zoom;

	private:
		bool m_rotation_enabled;
		bool m_translation_enabled;

		bool m_altitude_changed;
		bool m_orientation_changed;

		Point    m_last_point;

		std::mutex m_altitude_mutex;
		std::mutex m_location_mutex;
		std::mutex m_orientation_mutex;
	};
}