#pragma once

#include "Vector.h"
#include "Matrix.h"
#include "Camera.h"
#include "Rectangle.h"
#include "Frustum.h"
#include "FrameState.h"
#include "Texture2D.h"

#include <memory>

namespace argeo
{
	struct ShadowMapCamera;

	class UniformState
	{
	public:
		UniformState();
		~UniformState();

		mat4d get_projection_matrix();
		mat4d get_view_matrix();
		mat4d get_view_projection_matrix();
		mat4d get_inverse_view_projection_matrix();

		mat4d get_model_matrix();
		void  set_model_matrix(const mat4d value);

		mat3d get_normal_matrix();

		mat4d get_model_view_matrix();
		mat4d get_model_view_matrix_relative_to_eye();
		mat4d get_model_view_projection_matrix_relative_to_eye();
		
		mat4d get_inverse_model_matrix();
		mat4d get_inverse_model_view_matrix();

		mat4d get_model_view_projection_matrix();
		mat4d get_viewport_orthographic_matrix();
		mat4d get_viewport_transformation_matrix();

		mat42 get_model_z_to_clip_coordinates();

		const Rectangle get_viewport();
		void set_viewport(const Rectangle value);

		vec3d get_camera_light_position();

		vec3d get_sun_direction_world_coordinates();
		vec3d get_sun_direction_eye_coordinates();

		vec2d get_pixel_dimension();

        double get_diffuse_intensity();
		void  set_diffuse_intensity(const double& value);

        double get_specular_intensity();
		void  set_specular_intensity(const double& value);

        double get_high_resolution_snap_scale();
		void  set_high_resolution_snap_scale(const double& value);

        double get_morph_time();
		void  set_morph_time(const double& value);

        double get_ambient_intensity();
		void  set_ambient_intensity(const double& value);

		float get_shininess();
		void  set_shininess(const double& value);

		Texture2D* get_terrain_depth_texture();
		void set_terrain_depth_texture(Texture2D* value);

		vec2  get_current_frustum();

		vec3  get_camera_eye_low_model_coordinates();
		vec3  get_camera_eye_high_model_coordinates();

		vec3d get_camera_eye();
		vec3d get_camera_target();
		vec3d get_camera_up();

		void  set_camera(
			const vec3d& position,
			const vec3d& target, 
			const vec3d& up);

		void  set_camera(Camera* value);
		void  update_camera(Camera* value);
		void  update_camera(const ShadowMapCamera& value);
		void  update_frustum(Frustum* frustum);

		void  set_sun_and_moon_position(FrameState* frame_state);

		void update(FrameState* frame_state);

	private:

		mat4d view_2D_to_view_3D(
                const vec3d& position_2D,
                const vec3d& direction_2D,
                const vec3d &right_2D,
                const vec3d& up_2D,
                const double& frustum_width,
                const SceneMode& mode,
                MapProjection* projection);

		void set_view(const mat4d view_matrix);
		void update_projection(const mat4d projection_matrix);
		void update_viewport(const Rectangle viewport);

		void clean_camera_eye();

		void clean_viewport();
		void clean_model_view_matrix();
		void clean_model_z_to_clip_coordinates_matrix();
		void clean_view_projection_matrix();
		void clean_model_view_projection_relative_to_eye_matrix();
		void clean_model_view_relative_to_eye_matrix();
		void clean_inverse_projection_matrix();
		void clean_inverse_view_projection_matrix();
		void clean_inverse_model_matrix();
		void clean_inverse_model_view_matrix();
		void clean_model_view_projection_matrix();
		void clean_normal_matrix();

	private:
		Texture2D* m_terrain_depth_texture;

		bool m_viewport_dirty;
		Rectangle m_viewport;
		mat4d m_viewport_orthographic_matrix;
		mat4d m_viewport_transformation_matrix;
		vec2d m_pixel_dimension;

		bool   m_camera_eye_dirty;
		evec3d m_camera_eye_model_coordinates;
		vec3d  m_camera_eye;
		vec3d  m_camera_target;
		vec3d  m_camera_up;

		bool  m_model_z_to_clip_coordinates_matrix_dirty;
		mat42 m_model_z_to_clip_coordinates_matrix;

		vec2  m_current_frustum;
		
		mat4d  m_view_matrix;
		mat4d  m_projection_matrix;

		mat4d m_model_matrix;

		bool  m_normal_matrix_dirty;
		mat3d m_normal_matrix;

		bool  m_model_view_matrix_dirty;
		mat4d m_model_view_matrix;

		bool   m_inverse_model_view_matrix_dirty;
		mat4d  m_inverse_model_view_matrix;

		bool   m_inverse_model_matrix_dirty;
		mat4d  m_inverse_model_matrix;

		bool   m_inverse_projection_matrix_dirty;
		mat4d  m_inverse_projection_matrix;

		bool   m_view_projection_matrix_dirty;
		mat4d  m_view_projection_matrix;

		bool   m_inverse_view_projection_matrix_dirty;
		mat4d  m_inverse_view_projection_matrix;

		bool   m_model_view_projection_matrix_dirty;
		mat4d  m_model_view_projection_matrix;

		bool   m_model_view_projection_relative_to_eye_matrix_dirty;
		mat4d  m_model_view_projection_relative_to_eye_matrix;
		
		bool   m_model_view_relative_to_eye_matrix_dirty;
		mat4d  m_model_view_relative_to_eye_matrix;

		vec3d  m_sun_direction_eye_coordinates;
		vec3d  m_sun_direction_world_coordinates;

		double m_diffuse_intensity;
        double m_specular_intensity;
        double m_ambient_intensity;
        double m_shininess;

		float  m_morph_time;
		double m_high_resolution_snap_scale;

	};
}

