#include <Transforms.h>
#include "UniformState.h"
#include "ShadowMap.h"

namespace argeo
{
	UniformState::UniformState()
		: m_model_matrix(mat4d::identity())
		, m_terrain_depth_texture(nullptr)
        , m_morph_time(0.0)
		, m_diffuse_intensity(0.65f)
		, m_specular_intensity(0.25f)
		, m_ambient_intensity(0.10f)
		, m_shininess(12)
		, m_high_resolution_snap_scale(1)
		, m_viewport(0, 0, 1024, 1024)
		, m_viewport_dirty(true)
		, m_inverse_model_view_matrix_dirty(true)
		, m_inverse_projection_matrix_dirty(true)
		, m_view_projection_matrix_dirty(true)
		, m_model_z_to_clip_coordinates_matrix_dirty(true)
		, m_inverse_view_projection_matrix_dirty(true)
		, m_model_view_projection_relative_to_eye_matrix_dirty(true)
		, m_model_view_projection_matrix_dirty(true)
		, m_model_view_matrix_dirty(true)
		, m_normal_matrix_dirty(true)
	{ }


	UniformState::~UniformState()
	{ }

	Texture2D* UniformState::get_terrain_depth_texture()
	{
		return m_terrain_depth_texture;
	}

	void UniformState::set_terrain_depth_texture(Texture2D* value)
	{
		m_terrain_depth_texture = value;
	}

	double UniformState::get_diffuse_intensity()
	{
		return m_diffuse_intensity;
	}

	void  UniformState::set_diffuse_intensity(const double& value)
	{
		m_diffuse_intensity = value;
	}

	double UniformState::get_specular_intensity()
	{
		return m_specular_intensity;
	}

	void UniformState::set_specular_intensity(const double& value)
	{
		m_specular_intensity = value;
	}

	double UniformState::get_ambient_intensity()
	{
		return m_ambient_intensity;
	}

	void UniformState::set_ambient_intensity(const double& value)
	{
		m_ambient_intensity = value;
	}

	float UniformState::get_shininess()
	{
		return m_shininess;
	}

	void UniformState::set_shininess(const double& value)
	{
		m_shininess = value;
	}

	vec3d UniformState::get_sun_direction_eye_coordinates()
	{
		return m_sun_direction_eye_coordinates;
	}
		
	vec3d UniformState::get_sun_direction_world_coordinates()
	{
		return m_sun_direction_world_coordinates;
	}

	vec3d UniformState::get_camera_light_position()
	{
		return m_camera_eye;
	}

	vec2d UniformState::get_pixel_dimension()
	{
		return m_pixel_dimension;
	}

	double UniformState::get_high_resolution_snap_scale()
	{
		return m_high_resolution_snap_scale;
	}

	void UniformState::set_high_resolution_snap_scale(const double& value)
	{
		m_high_resolution_snap_scale = value;
	}

	double UniformState::get_morph_time()
	{
		return m_morph_time;
	}

	void UniformState::set_morph_time(const double &value)
	{
		m_morph_time = value;
	}
	
	mat4d UniformState::get_projection_matrix()
	{
		return m_projection_matrix;
	}

	mat4d UniformState::get_view_matrix()
	{
		return m_view_matrix;
	}

	mat4d UniformState::get_model_matrix()
	{
		return m_model_matrix;
	}

	vec3 UniformState::get_camera_eye_low_model_coordinates()
	{
		clean_camera_eye();
		return m_camera_eye_model_coordinates.low;
	}

	vec3 UniformState::get_camera_eye_high_model_coordinates()
	{
		clean_camera_eye();
		return m_camera_eye_model_coordinates.high;
	}

	mat4d UniformState::get_inverse_model_matrix()
	{
		clean_inverse_model_matrix();
		return m_inverse_model_matrix;
	}

	mat4d UniformState::get_inverse_model_view_matrix()
	{
		clean_inverse_model_view_matrix();
		return m_inverse_model_view_matrix;
	}

	mat4d UniformState::get_inverse_view_projection_matrix()
	{
		clean_inverse_view_projection_matrix();
		return m_inverse_view_projection_matrix;
	}

	mat4d UniformState::get_view_projection_matrix()
	{
		clean_view_projection_matrix();
		return m_view_projection_matrix;
	}

	mat3d UniformState::get_normal_matrix()
	{
		clean_normal_matrix();
		return m_normal_matrix;
	}

	mat4d UniformState::get_model_view_matrix()
	{
		clean_model_view_matrix();
		return m_model_view_matrix;
	}

	mat4d UniformState::get_model_view_projection_matrix()
	{
		clean_model_view_projection_matrix();
		return m_model_view_projection_matrix;
	}

	mat4d UniformState::get_model_view_projection_matrix_relative_to_eye()
	{
		clean_model_view_projection_relative_to_eye_matrix();
		return m_model_view_projection_relative_to_eye_matrix;
	}

	mat4d UniformState::get_model_view_matrix_relative_to_eye()
	{
		clean_model_view_relative_to_eye_matrix();
		return m_model_view_relative_to_eye_matrix;
	}

	mat4d UniformState::get_viewport_orthographic_matrix()
	{
		clean_viewport();
		return m_viewport_orthographic_matrix;
	}

	mat4d UniformState::get_viewport_transformation_matrix()
	{
		clean_viewport();
		return m_viewport_transformation_matrix;
	}

	mat42 UniformState::get_model_z_to_clip_coordinates()
	{
		clean_model_z_to_clip_coordinates_matrix();
		return m_model_z_to_clip_coordinates_matrix;	
	}

	vec2 UniformState::get_current_frustum()
	{
		return m_current_frustum;
	}

	vec3d UniformState::get_camera_eye()
	{
		return m_camera_eye;
	}

	vec3d UniformState::get_camera_target()
	{
		return m_camera_target;
	}

	vec3d UniformState::get_camera_up()
	{
		return m_camera_up;
	}	

	const Rectangle UniformState::get_viewport()
	{
		return m_viewport;
	}

	void UniformState::set_viewport(const Rectangle value)
	{
		if (m_viewport != value)
		{
			update_viewport(value);
		}
	}

	void UniformState::set_sun_and_moon_position(FrameState* frame_state)
	{
		m_sun_direction_world_coordinates = vec3d(200000, 300000, 200000) * 102222;
		m_sun_direction_world_coordinates = vec_normalize(m_sun_direction_world_coordinates);

		m_sun_direction_eye_coordinates   = to_rotation(get_view_matrix()) * m_sun_direction_world_coordinates;
		m_sun_direction_eye_coordinates   = vec_normalize(m_sun_direction_eye_coordinates);
	}

	void UniformState::update(FrameState* frame_state)
	{
		update_camera(frame_state->get_camera());
		set_sun_and_moon_position(frame_state);

        m_morph_time = frame_state->get_morph_time();
	}

	void UniformState::set_camera(
		const vec3d& position,
		const vec3d& target,
		const vec3d& up)
	{
		m_camera_eye      = position;
		m_camera_target   = target;
		m_camera_up       = up;
	
		m_camera_eye_dirty = true;
	}

	void UniformState::set_camera(Camera* value)
	{
		m_camera_eye      = value->get_position_wc();
		m_camera_target   = value->get_target_wc();
		m_camera_up       = value->get_up_wc();
	
		m_camera_eye_dirty = true;
	}

	void UniformState::update_camera(Camera* value)
	{
		set_view(value->get_view_matrix());
		set_camera(value);

		update_frustum(value->get_frustum());
	}

	void UniformState::update_camera(const ShadowMapCamera& value)
	{
		set_view(value.view_matrix);
		set_camera(
			value.position_wc,
			value.position_wc + value.direction_wc,
			value.up_wc);

		m_current_frustum = vec2(
			value.frustum->get_near(),
			value.frustum->get_far());
		update_frustum(value.frustum.get());
	}

	void UniformState::update_frustum(Frustum* frustum)
	{
		m_current_frustum = vec2(
			frustum->get_near(),
			frustum->get_far());

		update_projection(frustum->get_projection());
	}

	void UniformState::update_viewport(const Rectangle value)
	{
		m_viewport = value;
		m_viewport_dirty = true;
	}

	void UniformState::set_model_matrix(const mat4d value)
	{
		m_model_matrix = value;

		m_inverse_model_view_matrix_dirty	                 = true;
		m_normal_matrix_dirty								 = true;
		m_camera_eye_dirty								     = true;
		m_inverse_model_matrix_dirty		                 = true;
		m_inverse_view_projection_matrix_dirty               = true;
		m_model_view_matrix_dirty							 = true;
		m_model_view_projection_matrix_dirty				 = true;
		m_model_view_relative_to_eye_matrix_dirty			 = true;
		m_model_view_projection_relative_to_eye_matrix_dirty = true;
		m_model_z_to_clip_coordinates_matrix_dirty			 = true;
	}

	void UniformState::set_view(const mat4d view_matrix)
	{
		m_view_matrix = view_matrix;

		m_inverse_model_view_matrix_dirty	                 = true;
		m_normal_matrix_dirty								 = true;
		m_model_view_projection_relative_to_eye_matrix_dirty = true;
		m_model_view_relative_to_eye_matrix_dirty		     = true;
		m_model_view_matrix_dirty							 = true;
		m_model_view_projection_matrix_dirty				 = true;
		m_inverse_view_projection_matrix_dirty				 = true;
		m_view_projection_matrix_dirty						 = true;
		m_model_z_to_clip_coordinates_matrix_dirty			 = true;
	}

	void UniformState::update_projection(const mat4d projection_matrix)
	{
		m_projection_matrix = projection_matrix;

		m_inverse_projection_matrix_dirty					 = true;
		m_model_view_projection_matrix_dirty				 = true;
		m_view_projection_matrix_dirty					     = true;
		m_model_view_relative_to_eye_matrix_dirty		     = true;
		m_model_view_projection_relative_to_eye_matrix_dirty = true;
	}

	void UniformState::clean_viewport()
	{
		if (m_viewport_dirty)
		{
			m_viewport_transformation_matrix = compute_viewport_transformation_matrix(
				m_viewport,
				0.0,
				1.0);
			m_viewport_orthographic_matrix   = compute_viewport_orthographic_matrix(m_viewport);
			m_viewport_dirty = false;
		}
	}

	void UniformState::clean_model_z_to_clip_coordinates_matrix()
	{
		if (m_model_z_to_clip_coordinates_matrix_dirty)
		{
			// Bottom two rows of model-view-projection matrix
			mat4d m = get_model_view_projection_matrix();
			m_model_z_to_clip_coordinates_matrix = mat42(
				m(2, 0), m(2, 1), m(2, 2), m(2, 3),
				m(3, 0), m(3, 1), m(3, 2), m(3, 3));
			m_model_z_to_clip_coordinates_matrix_dirty = false;
		}
	}

	void UniformState::clean_model_view_relative_to_eye_matrix()
	{
		if (m_model_view_relative_to_eye_matrix_dirty)
		{
			mat4d m = get_model_view_matrix();
			m_model_view_relative_to_eye_matrix = mat4d(
				m.data[0], m.data[1], m.data[2],  m.data[3],
				m.data[4], m.data[5], m.data[6],  m.data[7],
				m.data[8], m.data[9], m.data[10], m.data[11],
				0.0      , 0.0     , 0.0		, m.data[15]);
			m_model_view_relative_to_eye_matrix_dirty = false;
		}
	}

	void UniformState::clean_model_view_projection_relative_to_eye_matrix()
	{
		if (m_model_view_projection_relative_to_eye_matrix_dirty)
		{
			m_model_view_projection_relative_to_eye_matrix = get_projection_matrix() * get_model_view_matrix_relative_to_eye();
			m_model_view_projection_relative_to_eye_matrix_dirty = false;
		}
	}

	void UniformState::clean_inverse_projection_matrix()
	{
		if (m_inverse_projection_matrix_dirty)
		{
            // TODO Mode 2D
			m_inverse_projection_matrix		  = mat_inverse(get_projection_matrix());
			m_inverse_projection_matrix_dirty = false;
		}
	}

	void UniformState::clean_inverse_model_view_matrix()
	{
		if (m_inverse_model_view_matrix_dirty)
		{
			m_inverse_model_view_matrix = mat_inverse(get_model_view_matrix());
			m_inverse_model_view_matrix_dirty = false;
		}
	}

	void UniformState::clean_inverse_model_matrix()
	{
		if (m_inverse_model_matrix_dirty)
		{
			m_inverse_model_matrix = mat_inverse(get_model_matrix());
			m_inverse_model_matrix_dirty = false;
		}
	}

	void UniformState::clean_inverse_view_projection_matrix()
	{
		if (m_inverse_view_projection_matrix_dirty)
		{
			m_inverse_view_projection_matrix       = mat_inverse(get_view_projection_matrix());
			m_inverse_view_projection_matrix_dirty = false;
		}
	}

	void UniformState::clean_view_projection_matrix()
	{
		if (m_view_projection_matrix_dirty)
		{
			m_view_projection_matrix	   = get_projection_matrix() * get_view_matrix();
			m_view_projection_matrix_dirty = false;
		}
	}

	void UniformState::clean_model_view_matrix()
	{
		if (m_model_view_matrix_dirty)
		{
			m_model_view_matrix		  = get_view_matrix() * get_model_matrix();
			m_model_view_matrix_dirty = false;
		}
	}

	void UniformState::clean_normal_matrix()
	{
		if (m_normal_matrix_dirty)
		{
			m_normal_matrix		  = to_rotation(get_inverse_model_view_matrix());
			m_normal_matrix       = mat_transpose(m_normal_matrix);
			m_normal_matrix_dirty = false;
		}
	}

	void UniformState::clean_model_view_projection_matrix()
	{
		if (m_model_view_projection_matrix_dirty)
		{
			m_model_view_projection_matrix		 = get_projection_matrix() * get_model_view_matrix();
			m_model_view_projection_matrix_dirty = false;
		}
	}

	void UniformState::clean_camera_eye()
	{
		if (m_camera_eye_dirty)
		{
			mat4d m = get_inverse_model_matrix();
			vec3d camera_eye_model_coordinates = mat_multiply_by_point(m, m_camera_eye);
			m_camera_eye_model_coordinates     = evec3d(camera_eye_model_coordinates);
			m_camera_eye_dirty = false;
		}
	}

	mat4d UniformState::view_2D_to_view_3D(
            const vec3d &position_2D,
            const vec3d &direction_2D,
            const vec3d &right_2D,
            const vec3d &up_2D,
            const double &frustum_width,
            const SceneMode &mode,
            MapProjection *projection)
	{
        // The camera position and directions are expressed in the 2D coordinate system where the Y axis is to the East,
        // the Z axis is to the North, and the X axis is out of the map.  Express them instead in the ENU axes where
        // X is to the East, Y is to the North, and Z is out of the local horizontal plane.
        vec3d p(
                position_2D.y,
                position_2D.z,
                position_2D.x);

        vec3d r(
                right_2D.y,
                right_2D.z,
                right_2D.x);

        vec3d u(
                up_2D.y,
                up_2D.z,
                up_2D.x);

        vec3d d(
                direction_2D.y,
                direction_2D.z,
                direction_2D.x);

        // In 2D, the camera height is always 12.7 million meters.
        // The apparent height is equal to half the frustum width.
        if (mode == SceneMode::Scene2D)
        {
            p.z = frustum_width * 0.5;
        }

        // Compute the equivalent camera position in the real (3D) world.
        // In 2D and Columbus View, the camera can travel outside the projection, and when it does so
        // there's not really any corresponding location in the real world.  So clamp the unprojected
        // longitude and latitude to their valid ranges.
        Geodetic3D geodetic = projection->unproject(p);
        geodetic = Geodetic3D(
                ArgeoMath::clamp(geodetic.get_longitude(), -PI, PI),
                ArgeoMath::clamp(geodetic.get_latitude(),  -PI_OVER_TWO, PI_OVER_TWO),
                geodetic.get_height());


        Ellipsoid ellipsoid  = projection->get_ellipsoid();
        vec3d position_3D     = ellipsoid.to_geocentric3D(geodetic).to_vec3d();

        // Compute the rotation from the local ENU at the real world camera position to the fixed axes.
        mat4d enu_to_fixed = Transforms::enu_to_fixed_frame(position_3D, ellipsoid);

        // Transform each camera direction to the fixed axes.
        r = mat_multiply_by_point_as_vector(enu_to_fixed, r);
        u = mat_multiply_by_point_as_vector(enu_to_fixed, u);
        d = mat_multiply_by_point_as_vector(enu_to_fixed, d);

        // Compute the view matrix based on the new fixed-frame camera position and directions.

        mat4d result;
        result.data[0]  =  r.x;
        result.data[1]  =  u.x;
        result.data[2]  = -d.x;
        result.data[3]  =  0.0;
        result.data[4]  =  r.y;
        result.data[5]  =  u.y;
        result.data[6]  = -d.y;
        result.data[7]  =  0.0;
        result.data[8]  =  r.z;
        result.data[9]  =  u.z;
        result.data[10] = -d.z;
        result.data[11] =  0.0;
        result.data[12] = -vec_dot(r, position_3D);
        result.data[13] = -vec_dot(u, position_3D);
        result.data[14] =  vec_dot(d, position_3D);
        result.data[15] = 1.0;

        return result;
	}

}