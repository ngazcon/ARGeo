#pragma once

#include "Vector.h"
#include "Frustum.h"
#include "GeodeticExtent.h"
#include "Geodetic3D.h"
#include "SceneMode.h"
#include "OrthographicOffCenterFrustum.h"
#include "Ray.h"

#include <unordered_set>
#include <memory>

namespace argeo
{
	class CameraMoveListener;
	class CameraChangedListener;
	class Scene;
    class SceneTransitioner;
	class Camera
	{
		friend class Scene;
        friend class SceneTransitioner;
		friend class DevicePoseCameraController;
	public:
        static const GeodeticExtent DEFAULT_VIEW_RECTANGLE;
        static const mat4d TRANSFORM_2D;
        static const mat4d TRANSFORM_2D_INVERSE;

		Camera(Scene* scene);
		~Camera();

		bool add_move_listener(CameraMoveListener* listener);
		bool remove_move_listener(CameraMoveListener* listener);
		bool add_changed_listener(CameraChangedListener* listener);
		bool remove_changed_listener(CameraChangedListener* listener);

		mat4d get_transform();
		vec3d get_position();
		vec3d get_up();
		vec3d get_direction();
		vec3d get_right();

		Geodetic3D get_position_geodetic();

		vec3d get_position_wc();
		vec3d get_up_wc();
		vec3d get_direction_wc();
		vec3d get_right_wc();
		vec3d get_target_wc();
		
		/// 
		/// Rotate each of the camera's orientation vectors around <code>axis</code> by <code>angle</code>
		/// @param {Cartesian3} axis The axis to rotate around.
		/// @param {Number} [angle] The angle, in radians, to rotate by. Defaults to <code>defaultLookAmount</code>.
		/// 
		void look(
			const vec3d& axis,
			const double& angle);

		void look_up(const double& angle);
		void look_down(const double& angle);
		void look_left(const double& angle);
		void look_right(const double& angle);
		void twist_left(const double& angle);
		void twist_right(const double& angle);

		/// 
		/// Rotates the camera around <code>axis</code> by <code>angle</code>. The distance
		/// of the camera's position to the center of the camera's reference frame remains the same.
		/// @param {Cartesian3} axis The axis to rotate around given in world coordinates.
		/// @param {Number} [angle] The angle, in radians, to rotate by. Defaults to <code>defaultRotateAmount</code>.
		/// 
		void rotate(
			const vec3d& axis,
			const double& angle);

		void rotate_horizontal(const double& angle);
		void rotate_vertical(const double& angle);

		void rotate_left(const double& angle);
		void rotate_right(const double& angle);
		void rotate_up(const double& angle);
		void rotate_down(const double& angle);



        void zoom_in(const double& amount  = std::numeric_limits<double>::quiet_NaN());
        void zoom_out(const double& amount = std::numeric_limits<double>::quiet_NaN());

		void move(
			const vec3d& direction,
			const double& amount);

		void move_left(const double& amount);
		void move_right(const double& amount);
		void move_up(const double& amount);
		void move_down(const double& amount);
		void move_forward(const double& amount);
		void move_backward(const double& amount);

		/// 
		/// Sets the camera position, orientation and transform.
		/// @param {Object} options Object with the following properties:
		/// @param {Cartesian3|Rectangle} [options.destination] The final position of the camera in WGS84 (world) coordinates or a rectangle that would be visible from a top-down view.
		/// @param {Object} [options.orientation] An object that contains either direction and up properties or heading, pith and roll properties. By default, the direction will point
		/// towards the center of the frame in 3D and in the negative z direction in Columbus view. The up direction will point towards local north in 3D and in the positive
		/// y direction in Columbus view. Orientation is not used in 2D when in infinite scrolling mode.
		/// @param {Matrix4} [options.endTransform] Transform matrix representing the reference frame of the camera.
		/// 
		void set_view_3D(
			const vec3d& position,
			const double& yaw   = 0.0,
			const double& pitch = 0.0,
			const double& roll  = 0.0);

        void set_view_CV(
                const vec3d& position,
                const double& yaw   = 0.0,
                const double& pitch = 0.0,
                const double& roll  = 0.0,
                const bool& convert = false);

        void set_view_2D(
                const vec3d& position,
                const double& yaw   = 0.0,
                const double& pitch = 0.0,
                const double& roll  = 0.0,
                const bool& convert = false);

		void set_view(
			const vec3d& position,
			const double& yaw,
			const double& pitch,
			const double& roll,
			const mat4d&  transform);

        void set_view(
                const vec3d& position,
                const double& yaw,
                const double& pitch,
                const double& roll);

        void switch_to_perspective_frustum();
        void switch_to_orthographic_frustum();

		void set_position(vec3d value);
		void set_direction(vec3d value);
		void set_up(vec3d value);
		void set_right(vec3d value);

		void   set_percentage_changed(const double& value);
		double get_percentage_changed() const;

		double calculate_yaw(
			const vec3d& direction,
			const vec3d& up) const;

		double calculate_pitch(
			const vec3d& direction) const;

		double calculate_roll(
			const vec3d& direction,
			const vec3d& right,
			const vec3d& up) const;

		double get_yaw();
		double get_pitch();
		double get_roll();


		double compute_d(
			const vec3d& direction,
			const vec3d& up_or_right,
			const vec3d& corner,
			const double& tan_theta_or_phi);

		vec3d rectangle_camera_position(
			const GeodeticExtent& rectangle,
			const bool& update_camera);

		void set_frustum(Frustum* value);
		Frustum* get_frustum();

		mat4d get_view_matrix();
		mat4d get_inverse_view_matrix();

		void changed(const double& percentage);
		void move_start();
		void move_end();

        double get_pixel_size(
                BoundingSphere* bounding_sphere,
                const unsigned int& surface_width,
                const unsigned int& surface_height);

        Ray get_pick_ray_orthographic(const vec2d& window_position);
        Ray get_pick_ray_perspective(const vec2d& window_position);
        Ray get_pick_ray(const vec2d& window_position);

		Camera* clone(Camera* result = nullptr);

        /**
         * Gets the magnitude of the camera position. In 3D, this is the vector magnitude. In 2D and
         * Columbus view, this is the distance to the map.
         *
         * @returns {Number} The magnitude of the position.
         */
		double get_magnitude() const;

	private:
        double get_distance_to_bounding_sphere(BoundingSphere* bounding_sphere);

        void zoom_2D(const double& amount);
        void zoom_3D(const double& amount);

        void update(const SceneMode& mode);
		void update_camera_changed();
		void set_transform(const mat4d& value);
		void update_members();
		void clean_view_matrix();
        void clamp_move_2D(vec3d& position);
        void adjust_orthographic_frustum(const bool& zooming);
        void convert_transform_for_2D();
        void convert_transform_for_CV();

	private:
        double m_maximum_zoom_factor;
        double m_default_zoom_amount;

		Scene*   m_scene;
		Frustum* m_frustum;

		vec3d m_current_position;
		vec3d m_position_wc;
		vec3d m_position;
		Geodetic3D m_position_geodetic;

		vec3d m_current_direction;
		vec3d m_direction_wc;
		vec3d m_direction;

		vec3d m_current_up;
		vec3d m_up_wc;
		vec3d m_up;

		vec3d m_current_right;
		vec3d m_right_wc;
		vec3d m_right;
		
		vec3d m_changed_position;
		vec3d m_changed_direction;
        Frustum* m_changed_frustum;
		double m_percentage_changed;
		
		mat4d m_inverse_view_matrix;
		mat4d m_view_matrix;

		bool  m_transform_changed;
		mat4d m_current_transform;
		mat4d m_transform;
		mat4d m_current_inverse_transform;
		mat4d m_inverse_transform;

        OrthographicOffCenterFrustum* m_max_2D_frustum;
        vec3d m_max_coord;
		SceneMode m_mode;
		bool m_mode_changed;

		std::unordered_set<CameraMoveListener*> m_move_listeners;
		std::unordered_set<CameraChangedListener*> m_changed_listeners;
	};
}
