#include "Camera.h"

#include <algorithm>
#include <OrthographicFrustum.h>

#include "Quaternion.h"
#include "PerspectiveFrustum.h"
#include "Transforms.h"
#include "EllipsoidGeodesic.h"
#include "CameraMoveListener.h"
#include "CameraChangedListener.h"
#include "Scene.h"

namespace argeo
{
	/**
	* The default rectangle the camera will view on creation.
	* @type Rectangle
	*/
	const GeodeticExtent Camera::DEFAULT_VIEW_RECTANGLE = GeodeticExtent::from_degrees(
		-0.2, 
		-0.2,
		 0.2, 
		 0.2);


    const mat4d Camera::TRANSFORM_2D = mat4d(
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            1.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 1.0);

    const mat4d Camera::TRANSFORM_2D_INVERSE = mat_inverse_transformation(TRANSFORM_2D);

	/**
	* A scalar to multiply to the camera position and add it back after setting the camera to view the rectangle.
	* A value of zero means the camera will view the entire {@link Camera#DEFAULT_VIEW_RECTANGLE}, a value greater than zero
	* will move it further away from the extent, and a value less than zero will move it close to the extent.
	* @type Number
	*/
	const double DEFAULT_VIEW_FACTOR = 0.5;

	Camera::Camera(Scene* scene)
		: m_transform(mat4d::identity())	
		, m_inverse_transform(mat4d::identity())
		, m_view_matrix(mat4d::identity())
		, m_inverse_view_matrix(mat4d::identity())
		, m_current_transform(mat4d::identity())
		, m_current_inverse_transform(mat4d::identity())
		, m_transform_changed(false)
		, m_scene(scene)
		, m_percentage_changed(0.5)
		, m_changed_direction(std::numeric_limits<double>::quiet_NaN())
		, m_changed_position(std::numeric_limits<double>::quiet_NaN())
		, m_mode(SceneMode::Scene3D)
        , m_mode_changed(true)
        , m_changed_frustum(nullptr)
        , m_default_zoom_amount(100000.0)
        , m_maximum_zoom_factor(1.5)
	{
        m_max_coord = m_scene->get_projection()->project(Geodetic3D(PI, PI_OVER_TWO)).to_vec3d();

		PerspectiveFrustum* frustum = new PerspectiveFrustum();
		frustum->set_field_of_view_y(PI / 3.0);
		frustum->set_aspect_ratio(
			(double)scene->get_context()->get_drawing_surface_width() /
			(double)scene->get_context()->get_drawing_surface_height());

		m_frustum = frustum;

		clean_view_matrix();

		// set default view
		m_position = rectangle_camera_position(
			DEFAULT_VIEW_RECTANGLE,
			true);

		double mag = vec_length(m_position);
		mag += mag * DEFAULT_VIEW_FACTOR;

		m_position = vec_normalize(m_position) * mag;
	}


	Camera::~Camera()
	{ }
	
	void Camera::set_frustum(Frustum* value)
    {
        m_frustum = value;
    }

	Frustum* Camera::get_frustum()
	{
		return m_frustum;
	}

	vec3d Camera::get_position()
	{
		return m_position;
	}

	vec3d Camera::get_right()
	{
		return m_right;
	}

	vec3d Camera::get_direction()
	{
		return m_direction;
	}

	vec3d Camera::get_up()
	{
		return m_up;
	}

	vec3d Camera::get_target_wc()
	{
		update_members();
		return m_position_wc + m_direction_wc;
	}

	Geodetic3D Camera::get_position_geodetic()
	{
		update_members();
		return m_position_geodetic;
	}

	vec3d Camera::get_position_wc()
	{
		update_members();
		return m_position_wc;
	}

	void Camera::set_position(vec3d value)
	{
		m_position = value;
	}

	vec3d Camera::get_direction_wc()
	{
		update_members();
		return m_direction_wc;
	}

	void Camera::set_direction(vec3d value)
	{
		m_direction = value;
	}

	vec3d Camera::get_up_wc()
	{
		update_members();
		return m_up_wc;
	}

	void Camera::set_up(vec3d value)
	{
		m_up = value;
	}

	vec3d Camera::get_right_wc()
	{
		update_members();
		return m_right_wc;
	}

	void Camera::set_right(vec3d value)
	{
		m_right = value;
	}

	double Camera::get_yaw()
	{
        if (m_mode == SceneMode::Morphing)
        {
            return std::numeric_limits<double>::quiet_NaN();
        }

		mat4d current_transform = m_transform;

		mat4d transform = Transforms::enu_to_fixed_frame(
			get_position_wc(),
			m_scene->get_projection()->get_ellipsoid());

		set_transform(transform);

		double yaw = calculate_yaw(m_direction, m_up);

		set_transform(current_transform);

		return yaw;
	}

	double Camera::get_pitch()
	{
        if (m_mode == SceneMode::Morphing)
        {
            return std::numeric_limits<double>::quiet_NaN();
        }

		mat4d current_transform = m_transform;

		mat4d transform = Transforms::enu_to_fixed_frame(
			get_position_wc(),
			m_scene->get_projection()->get_ellipsoid());

		set_transform(transform);

		double pitch = calculate_pitch(m_direction);

		set_transform(current_transform);

		return pitch;
	}

	double Camera::get_roll()
	{
        if (m_mode == SceneMode::Morphing)
        {
            return std::numeric_limits<double>::quiet_NaN();
        }

		mat4d current_transform = m_transform;

		mat4d transform = Transforms::enu_to_fixed_frame(
			get_position_wc(),
			m_scene->get_projection()->get_ellipsoid());

		set_transform(transform);

		double roll = calculate_roll(m_direction, m_right, m_up);

		set_transform(current_transform);

		return roll;
	}

	double Camera::calculate_yaw(
		const vec3d& direction,
		const vec3d& up) const
	{
		double yaw;

		if (!ArgeoMath::equals_epsilon(std::abs(direction.z), 1.0, EPSILON3))
		{
			yaw = std::atan2(direction.y, direction.x) - PI_OVER_TWO;
		}
		else
		{
			yaw = std::atan2(up.y, up.x) - PI_OVER_TWO;
		}

		return TWO_PI - ArgeoMath::zero_to_two_pi(yaw);
	}
	
	double Camera::calculate_pitch(const vec3d& direction) const
	{
		return PI_OVER_TWO - ArgeoMath::acos_clamped(direction.z);
	}

	double Camera::calculate_roll(
		const vec3d& direction,
		const vec3d& right,
		const vec3d& up) const
	{
		double roll = 0.0;

		if (!ArgeoMath::equals_epsilon(std::abs(direction.z), 1.0, EPSILON3))
		{
			roll = std::atan2(-right.z, up.z);
			roll = ArgeoMath::zero_to_two_pi(roll + TWO_PI);
		}

		return roll;
	}

	double Camera::get_magnitude() const
	{
		if (m_mode == SceneMode::Scene3D)
		{
			return vec_length(m_position);
		}
		else if (m_mode == SceneMode::SceneCV)
		{
			return std::abs(m_position.z);
		}
		else if (m_mode == SceneMode::Scene2D)
		{
			return std::max(
					m_frustum->get_right() - m_frustum->get_left(),
					m_frustum->get_top() - m_frustum->get_bottom());
		}
	}

    void Camera::zoom_2D(const double &amount)
    {
        if (dynamic_cast<OrthographicOffCenterFrustum*>(m_frustum) == nullptr)
        {
            throw std::logic_error("The camera frustum is expected to be orthographic for 2D camera control.");
        }

        double half_amount = amount * 0.5;

        double new_right = m_frustum->get_right() - half_amount;
        double new_left  = m_frustum->get_left() + half_amount;
        double max_right = m_max_coord.x;

        if (m_scene->get_map_mode_2D() == MapMode2D::Rotate)
        {
            max_right *= m_maximum_zoom_factor;
        }

        if (new_right > max_right)
        {
            new_right =  max_right;
            new_left  = -max_right;
        }

        if (new_right <= new_left)
        {
            new_right =  1.0;
            new_left  = -1.0;
        }

        const double ratio = m_frustum->get_top() / m_frustum->get_right();

        m_frustum->set_right(new_right);
        m_frustum->set_left(new_left);
        m_frustum->set_top(new_right * ratio);
        m_frustum->set_bottom(-m_frustum->get_top());
    }

    void Camera::zoom_3D(const double &amount)
    {
        move(m_direction, amount);
    }

    void Camera::zoom_in(const double& amount)
    {
        double value = amount;
        if (std::isnan(amount))
        {
            value = m_default_zoom_amount;
        }

        if (m_mode == SceneMode ::Scene2D)
        {
            zoom_2D(value);
        }
        else
        {
            zoom_3D(value);
        }
    }

    void Camera::zoom_out(const double& amount)
    {
        double value = amount;
        if (std::isnan(amount))
        {
            value = m_default_zoom_amount;
        }


        if (m_mode == SceneMode ::Scene2D)
        {
            zoom_2D(-value);
        }
        else
        {
            zoom_3D(-value);
        }
    }

	void Camera::move_left(const double& amount)
	{
		move(m_right, -amount);
	}

	void Camera::move_right(const double& amount)
	{
		move(m_right, amount);
	}

	void Camera::move_up(const double& amount)
	{
		move(m_up, amount);
	}

	void Camera::move_down(const double& amount)
	{
		move(m_up, -amount);
	}

	void Camera::move_forward(const double& amount)
	{
		move(m_direction, amount);
	}

	void Camera::move_backward(const double& amount)
	{
		move(m_direction, -amount);
	}

	void Camera::move(
		const vec3d& direction,
		const double& amount)
	{
		m_position += direction * amount;

        if (m_mode == SceneMode::Scene2D)
        {
            clamp_move_2D(m_position);
        }

        adjust_orthographic_frustum(true);
	}

	void Camera::rotate(
		const vec3d& axis,
		const double& angle)
	{
		quaternion quaternion = quaternion::from_axis_angle(axis, -angle);

		m_position	 = quaternion * m_position;
		m_direction	 = quaternion * m_direction;
		m_up	     = quaternion * m_up;
		m_right      = vec_cross(m_direction, m_up);
		m_up		 = vec_cross(m_right, m_direction);

        adjust_orthographic_frustum(true);
	}

	void Camera::rotate_vertical(const double& angle)
	{
		rotate(m_right, angle);
	}

	void Camera::rotate_horizontal(const double& angle)
	{
		rotate(m_up, angle);
	}

	void Camera::rotate_left(const double& angle)
	{
		rotate_horizontal(-angle);
	}

	void Camera::rotate_right(const double& angle)
	{
		rotate_horizontal(angle);
	}

	void Camera::rotate_up(const double& angle)
	{
		rotate_vertical(-angle);
	}

	void Camera::rotate_down(const double& angle)
	{
		rotate_vertical(angle);
	}

    void Camera::switch_to_perspective_frustum()
    {
        if (m_mode == SceneMode::Scene2D || dynamic_cast<PerspectiveFrustum*>(m_frustum) != nullptr)
        {
            return;
        }

        m_frustum = new PerspectiveFrustum(
                ArgeoMath::to_radians(60.0),
                m_scene->get_drawing_surface_width() / m_scene->get_drawing_surface_height());

    }

    void Camera::switch_to_orthographic_frustum()
    {
        if (m_mode == SceneMode::Scene2D || dynamic_cast<OrthographicFrustum*>(m_frustum) != nullptr)
        {
            return;
        }

        // It doesn't matter what we set this to.
        // The adjust below will correct the width based on the camera position.
        m_frustum = new OrthographicFrustum(
                m_scene->get_drawing_surface_width() / m_scene->get_drawing_surface_height(),
                vec_length(m_position));

        // Check the projection matrix. It will always be defined, but we need to force an off-center update.
        const mat4d projection_matrix = m_frustum->get_projection();
        adjust_orthographic_frustum(true);
    }

	void Camera::look(
		const vec3d& axis,
		const double& angle)
	{
		quaternion quaternion = quaternion::from_axis_angle(axis, -angle);

		m_direction = quaternion * m_direction;
		m_up		= quaternion * m_up;
		m_right		= quaternion * m_right;
	}

	void Camera::look_up(const double& angle)
	{
		look(m_right, -angle);
	}

	void Camera::look_down(const double& angle)
	{
		look(m_right, angle);
	}

	void Camera::look_left(const double& angle)
	{
		look(m_up, -angle);
	}

	void Camera::look_right(const double& angle)
	{
		look(m_up, angle);
	}

	void Camera::twist_left(const double& angle)
	{
		look(m_direction, angle);
	}

	void Camera::twist_right(const double& angle)
	{
		look(m_direction, -angle);
	}

    void Camera::set_view(
            const vec3d& position,
            const double& yaw,
            const double& pitch,
            const double& roll)
    {
        if (m_mode == SceneMode::Morphing)
        {
            return;
        }

        const bool convert = true;

        if (m_mode == SceneMode::Scene2D)
        {
            set_view_2D(
                    position,
                    yaw,
                    pitch,
                    roll,
                    convert);
        }
        else if (m_mode == SceneMode::Scene3D)
        {
            set_view_3D(
                    position,
                    yaw,
                    pitch,
                    roll);
        }
        else
        {
            set_view_CV(
                    position,
                    yaw,
                    pitch,
                    roll,
                    convert);
        }
    }

	void Camera::set_view(
		const vec3d& position,
		const double& yaw,
		const double& pitch,
		const double& roll,
		const mat4d&  transform)
	{
        if (m_mode == SceneMode::Morphing)
        {
            return;
        }
		set_transform(transform);
        set_view(position, yaw, pitch, roll);
	}

	void Camera::set_view_3D(
		const vec3d& position,
		const double& yaw,
		const double& pitch,
		const double& roll)
	{
		mat4d current_transform = m_transform;

		mat4d local_Transform = 
			Transforms::enu_to_fixed_frame(
				position,
				m_scene->get_projection()->get_ellipsoid());
		
		set_transform(local_Transform);

		quaternion rot_quat = quaternion::from_yaw_pitch_roll(yaw - PI_OVER_TWO, pitch, roll);
		mat3d rot_mat = quat_to_matrix(rot_quat);

		m_position	    = vec3d::zero();
		m_direction = vec3d(rot_mat.data[0], rot_mat.data[1], rot_mat.data[2]);
		m_up        = vec3d(rot_mat.data[6], rot_mat.data[7], rot_mat.data[8]);
		m_right     = vec_cross(m_direction, m_up);

		set_transform(current_transform);

        adjust_orthographic_frustum(true);
	}

    void Camera::set_view_CV(
            const vec3d& position,
            const double& yaw,
            const double& pitch,
            const double& roll,
            const bool& convert)
    {
        mat4d current_transform = m_transform;
        mat4d local_Transform   = mat4d::identity();

        set_transform(local_Transform);

        if (position != get_position_wc())
        {
            vec3d converted_position = position;
            if (convert)
            {
                MapProjection* projection  = m_scene->get_projection();
                Geodetic3D cartographic = projection->get_ellipsoid().to_geodetic3D(position);
                converted_position = projection->project(cartographic).to_vec3d();
            }
            m_position = converted_position;
        }

        quaternion rot_quat = quaternion::from_yaw_pitch_roll(yaw - PI_OVER_TWO, pitch, roll);
        mat3d rot_mat = quat_to_matrix(rot_quat);

        m_direction = mat_get_column(rot_mat, 0);
        m_up        = mat_get_column(rot_mat, 2);
        m_right     = vec_cross(m_direction, m_up);

        set_transform(current_transform);

        adjust_orthographic_frustum(true);
    }

    void Camera::set_view_2D(
            const vec3d& position,
            const double& yaw,
            const double& pitch,
            const double& roll,
            const bool& convert)
    {
        mat4d current_transform = m_transform;
        mat4d local_Transform   = mat4d::identity();

        set_transform(local_Transform);

        if (position != get_position_wc())
        {
            vec3d converted_position = position;
            if (convert)
            {
                MapProjection* projection  = m_scene->get_projection();
                Geodetic3D cartographic = projection->get_ellipsoid().to_geodetic3D(position);
                converted_position = projection->project(cartographic).to_vec3d();
            }
            m_position.x = converted_position.x;
            m_position.y = converted_position.y;

            const double new_left  = -converted_position.z * 0.5;
            const double new_right = -new_left;

            OrthographicOffCenterFrustum* frustum  = static_cast<OrthographicOffCenterFrustum*>(m_frustum);
            if (new_right > new_left)
            {
                const double ratio = frustum->get_top() / frustum->get_right();
                frustum->set_right(new_right);
                frustum->set_left(new_left);
                frustum->set_top(frustum->get_right() * ratio);
                frustum->set_bottom(-frustum->get_top());
            }
        }

        if (m_scene->get_map_mode_2D() == MapMode2D::Rotate)
        {
            quaternion rot_quat = quaternion::from_yaw_pitch_roll(yaw - PI_OVER_TWO, -PI_OVER_TWO, 0.0);
            mat3d rot_mat = quat_to_matrix(rot_quat);

            m_up        = mat_get_column(rot_mat, 2);
            m_right     = vec_cross(m_direction, m_up);
        }

        set_transform(current_transform);
    }

	mat4d Camera::get_view_matrix()
	{
		update_members();
		return m_view_matrix;
	}

	mat4d Camera::get_inverse_view_matrix()
	{
		update_members();
		return m_inverse_view_matrix;
	}

	mat4d Camera::get_transform()
	{
		return m_transform;
	}

	void Camera::set_percentage_changed(const double& value)
	{
		m_percentage_changed = value;
	}

	double Camera::get_percentage_changed() const
	{
		return m_percentage_changed;
	}

	void Camera::clean_view_matrix()
	{
		m_view_matrix = compute_view(
			m_current_position,
			m_current_direction,
			m_current_up,
			m_current_right);

		m_view_matrix = m_view_matrix * m_current_inverse_transform;
		m_inverse_view_matrix = mat_inverse_transformation(m_view_matrix);
	}

	void Camera::set_transform(const mat4d& value)
	{
		const vec3d position = get_position_wc();
		const vec3d direction = get_direction_wc();
		const vec3d up = get_up_wc();

		m_transform         = value;
		m_transform_changed = true;
		update_members();

		m_position = mat_multiply_by_point(m_current_inverse_transform, position);
		m_up  = mat_multiply_by_point_as_vector(m_current_inverse_transform, up);
		m_direction = mat_multiply_by_point_as_vector(m_current_inverse_transform, direction);
		m_right     = vec_cross(m_direction, m_up);

		update_members();
	}

    void Camera::convert_transform_for_CV()
    {
        m_current_transform = Transforms::basis_to_2D(m_scene->get_projection(), m_transform);
    }

    void Camera::convert_transform_for_2D()
    {
        Ellipsoid ellipsoid = m_scene->get_projection()->get_ellipsoid();

        vec4d origin        = mat_get_column(m_transform, 3);
        Geodetic3D geodetic = ellipsoid.to_geodetic3D(Geocentric3D(origin.x, origin.y, origin.z));

        vec3d projected_position = m_scene->get_projection()->project(geodetic).to_vec3d();
        vec4d new_origin(
            projected_position.z,
            projected_position.x,
            projected_position.y,
            1.0);

        vec4d new_z_axis = vec4d::unit_x();
        vec4d x_axis     = mat_get_column(m_transform, 0) + origin;

        geodetic = ellipsoid.to_geodetic3D(Geocentric3D(x_axis.x, x_axis.y, x_axis.z));
        projected_position = m_scene->get_projection()->project(geodetic).to_vec3d();

        vec4d new_x_axis(
                projected_position.z,
                projected_position.x,
                projected_position.y,
                1.0);

        new_x_axis  -= new_origin;
        new_x_axis.x = 0.0;

        vec4d new_y_axis;

        if (vec_length_squared(new_x_axis) > EPSILON10)
        {
            new_y_axis = vec4d(
                    vec_cross(to_vec3d(new_z_axis), to_vec3d(x_axis)),
                    new_y_axis.w);
        }
        else
        {
            vec4d y_axis =
                    mat_get_column(m_transform, 1) + origin;

            geodetic = ellipsoid.to_geodetic3D(Geocentric3D(y_axis.x, y_axis.y, y_axis.z));
            projected_position = m_scene->get_projection()->project(geodetic).to_vec3d();

            new_y_axis = vec4d(
                    projected_position.z,
                    projected_position.x,
                    projected_position.y,
                    0.0);

            new_y_axis  -= new_origin;
            new_y_axis.x = 0.0;

            if (vec_length_squared(new_y_axis) < EPSILON10)
            {
                new_x_axis = vec4d::unit_y();
                new_y_axis = vec4d::unit_z();
            }
        }

        new_x_axis = vec4d(
                vec_normalize(vec_cross(to_vec3d(new_y_axis), to_vec3d(new_z_axis))),
                new_x_axis.w);

        new_y_axis = vec4d(
                vec_normalize(vec_cross(to_vec3d(new_z_axis), to_vec3d(new_x_axis))),
                new_y_axis.w);

        mat_set_column(m_current_transform, 0, new_x_axis);
        mat_set_column(m_current_transform, 1, new_y_axis);
        mat_set_column(m_current_transform, 2, new_z_axis);
        mat_set_column(m_current_transform, 3, new_origin);
    }

	void Camera::update_members()
	{
        bool height_changed = false;
        double height = 0.0;
        if (m_mode == SceneMode::Scene2D)
        {
            OrthographicOffCenterFrustum* frustum = static_cast<OrthographicOffCenterFrustum*>(m_frustum);
            height = frustum->get_right() - frustum->get_left();
            height_changed = height != m_position_geodetic.get_height();
        }

		bool position_changed = m_position != m_current_position || height_changed;
		if (position_changed)
		{
			m_current_position = m_position;
		}

		bool up_changed = m_up != m_current_up;
		if (up_changed)
		{
			m_up = vec_normalize(m_up);
			m_current_up = m_up;
		}
			
		bool direction_changed = m_direction != m_current_direction;
		if (direction_changed)
		{
			m_direction = vec_normalize(m_direction);
			m_current_direction = m_direction;
		}

		bool right_changed = m_right != m_current_right;
		if (right_changed)
		{
			m_right = vec_normalize(m_right);
			m_current_right = m_right;
		}
		
		bool transform_changed = m_mode_changed || m_transform_changed;
		m_transform_changed    = false;
		if (transform_changed)
		{
			m_inverse_transform = mat_inverse_transformation(m_transform);

            if (m_mode == SceneMode::SceneCV || m_mode == SceneMode::Scene2D)
            {
               if (m_transform == mat4d::identity())
               {
                  m_current_transform = TRANSFORM_2D;
               }
               else if (m_mode == SceneMode::Scene2D)
               {
                  convert_transform_for_2D();
               }
               else
               {
                  convert_transform_for_CV();
               }
            }
            else
            {
                m_current_transform = m_transform;
            }

			m_current_inverse_transform = mat_inverse_transformation(m_current_transform);
			m_mode_changed              = false;
		}

		if (position_changed || transform_changed)
		{
            m_position_wc = mat_multiply_by_point(m_current_transform, m_current_position);

            // Compute the Cartographic position of the camera.
            if (m_mode == SceneMode::Scene3D || m_mode == SceneMode::Morphing)
            {
                m_position_geodetic = m_scene->get_projection()->get_ellipsoid().to_geodetic3D(
                        m_position_wc);
            }
            else
            {
                // The camera position is expressed in the 2D coordinate system where the Y axis is to the East,
                // the Z axis is to the North, and the X axis is out of the map.  Express them instead in the ENU axes where
                // X is to the East, Y is to the North, and Z is out of the local horizontal plane.
                vec3d position_enu;

                position_enu.x = m_position_wc.y;
                position_enu.y = m_position_wc.z;
                position_enu.z = m_position_wc.x;


                // In 2D, the camera height is always 12.7 million meters.
                // The apparent height is equal to half the frustum width.
                if (m_mode == SceneMode::Scene2D)
                {
                    position_enu.z = height;
                }

                m_position_geodetic = m_scene->get_projection()->unproject(position_enu);
            }
		}

		if (direction_changed || up_changed || right_changed)
		{
			double det = vec_dot(
				m_direction,
				vec_cross(m_up, m_right));

			if (std::abs(1.0 - det) > EPSILON2) 
			{
				//orthonormalize axes
				double inv_up_magnitude = 1.0 / vec_length_squared(m_up);
				double scalar = vec_dot(m_up, m_direction) * inv_up_magnitude;

				vec3d w0 = m_direction * scalar;

				m_up    = m_current_up    = vec_normalize(m_up - w0);
				m_right = m_current_right = vec_cross(m_direction, m_up);
			}
		}

		if (direction_changed || transform_changed)
		{
			m_direction_wc = mat_multiply_by_point_as_vector(m_current_transform, m_current_direction);
		}

		if (up_changed || transform_changed)
		{
			m_up_wc = mat_multiply_by_point_as_vector(m_current_transform, m_current_up);
		}

		if (right_changed || transform_changed)
		{
			m_right_wc = mat_multiply_by_point_as_vector(m_current_transform, m_current_right);
		}

		if (position_changed || direction_changed || up_changed || right_changed || transform_changed)
		{
			clean_view_matrix();
		}
	}

	vec3d Camera::rectangle_camera_position(
		const GeodeticExtent& rectangle,
		const bool& update_camera)
	{
		Ellipsoid ellipsoid = m_scene->get_projection()->get_ellipsoid();

		double north = rectangle.get_north();
		double south = rectangle.get_south();
		double east  = rectangle.get_east();
		double west  = rectangle.get_west();

		// If we go across the International Date Line
		if (west > east) 
		{
			east += TWO_PI;
		}

		// Find the midpoint latitude.
		//
		// EllipsoidGeodesic will fail if the north and south edges are very close to being on opposite sides of the ellipsoid.
		// Ideally we'd just call EllipsoidGeodesic.setEndPoints and let it throw when it detects this case, but sadly it doesn't
		// even look for this case in optimized builds, so we have to test for it here instead.
		//
		// Fortunately, this case can only happen (here) when north is very close to the north pole and south is very close to the south pole,
		// so handle it just by using 0 latitude as the center.  It's certainliy possible to use a smaller tolerance
		// than one degree here, but one degree is safe and putting the center at 0 latitude should be good enough for any
		// rectangle that spans 178+ of the 180 degrees of latitude.

		double longitude = (west + east) * 0.5;
		double latitude;

		if (south < -PI_OVER_TWO + RADIANS_PER_DEGREE && north > PI_OVER_TWO - RADIANS_PER_DEGREE) 
		{
			latitude = 0.0;
		}
		else 
		{
			Geodetic3D north_cartographic(
				longitude,
				north);

			Geodetic3D south_cartographic(
				longitude,
				south);

			EllipsoidGeodesic ellipsoid_geodesic(south_cartographic, north_cartographic, ellipsoid);
			latitude = ellipsoid_geodesic.interpolateUsingFraction(0.5).get_latitude();
		}

		Geodetic3D center_cartographic(
			longitude,
			latitude);

		vec3d center       = ellipsoid.to_geocentric3D(center_cartographic).to_vec3d();
		vec3d north_east   = ellipsoid.to_geocentric3D(Geodetic2D(east, north)).to_vec3d();
		vec3d north_west   = ellipsoid.to_geocentric3D(Geodetic2D(west, north)).to_vec3d();
		vec3d north_center = ellipsoid.to_geocentric3D(Geodetic2D(longitude, north)).to_vec3d();
		vec3d south_center = ellipsoid.to_geocentric3D(Geodetic2D(longitude, south)).to_vec3d();
		vec3d south_east   = ellipsoid.to_geocentric3D(Geodetic2D(east, south)).to_vec3d();
		vec3d south_west   = ellipsoid.to_geocentric3D(Geodetic2D(west, south)).to_vec3d();

		north_west -= center;
		south_east -= center;
		north_east -= center;
		south_west -= center;
		north_center -= center;
		south_center -= center;

		vec3d direction = ellipsoid.geodetic_surface_normal(center).to_vec3d();
		direction       = vec_normalize(direction);
		direction       = -direction;

		vec3d right = vec_cross(direction, vec3d::unit_z());
		right = vec_normalize(right);

		vec3d up = vec_cross(right, direction);

		if (update_camera)
		{
			m_direction = direction;
			m_right = right;
			m_up = up;
		}

		double tan_phi   = std::tan(m_frustum->get_field_of_view_y() * 0.5);
		double tan_theta = m_frustum->get_aspect_ratio() * tan_phi;

		double d = compute_d(direction, up, north_west, tan_phi);
		d = std::max(d, compute_d(direction, up, south_east, tan_phi));
		d = std::max(d, compute_d(direction, up, north_east, tan_phi));
		d = std::max(d, compute_d(direction, up, south_west, tan_phi));
		d = std::max(d, compute_d(direction, up, north_center, tan_phi));
		d = std::max(d, compute_d(direction, up, south_center, tan_phi));

		d = std::max(d, compute_d(direction, right, north_west, tan_theta));
		d = std::max(d, compute_d(direction, right, south_east, tan_theta));
		d = std::max(d, compute_d(direction, right, north_east, tan_theta));
		d = std::max(d, compute_d(direction, right, south_west, tan_theta));
		d = std::max(d, compute_d(direction, right, north_center, tan_theta));
		d = std::max(d, compute_d(direction, right, south_center, tan_theta));

		// If the rectangle crosses the equator, compute D at the equator, too, because that's the
		// widest part of the rectangle when projected onto the globe.
		if (south < 0 && north > 0) 
		{
			vec3d equator_position = ellipsoid.to_geocentric3D(Geodetic3D(west, 0.0)).to_vec3d();
			equator_position -= center;

			d = std::max(d, compute_d(direction, up, equator_position, tan_phi));
			d = std::max(d, compute_d(direction, right, equator_position, tan_theta));		
		
			equator_position  = ellipsoid.to_geocentric3D(Geodetic3D(east, 0.0)).to_vec3d();
			equator_position -= center;

			d = std::max(d, compute_d(direction, up, equator_position, tan_phi));
			d = std::max(d, compute_d(direction, right, equator_position, tan_theta));
		}

		return center - (direction * d);
	}

	double Camera::compute_d(
		const vec3d& direction,
		const vec3d& up_or_right,
		const vec3d& corner,
		const double& tan_theta_or_phi)
	{
		double opposite = std::abs(vec_dot(up_or_right, corner));
		return opposite / tan_theta_or_phi - vec_dot(direction, corner);
	}


	void Camera::move_start()
	{
		for (CameraMoveListener* listener : m_move_listeners)
		{
			listener->on_move_start(this);
		}
	}

	void Camera::move_end()
	{
		for (CameraMoveListener* listener : m_move_listeners)
		{
			listener->on_move_end(this);
		}
	}

	void Camera::changed(const double& percentage)
	{
		for (CameraChangedListener* listener : m_changed_listeners)
		{
			listener->on_changed(percentage, this);
		}
	}

	bool Camera::add_move_listener(CameraMoveListener* listener)
	{
		return m_move_listeners.insert(listener).second;
	}

	bool Camera::remove_move_listener(CameraMoveListener* listener)
	{
		return m_move_listeners.erase(listener);
	}

	bool Camera::add_changed_listener(CameraChangedListener* listener)
	{
		return m_changed_listeners.insert(listener).second;
	}

	bool Camera::remove_changed_listener(CameraChangedListener* listener)
	{
		return m_changed_listeners.erase(listener);
	}

	Camera* Camera::clone(Camera* result)
	{
		if (result == nullptr)
		{
			result = new Camera(m_scene);
		}
				
		result->set_direction(m_direction);
		result->set_position(m_position);
		result->set_right(m_right);
		result->set_up(m_up);
        result->set_transform(m_transform);
        result->m_transform_changed = true;

		return result;
	}

	void Camera::update_camera_changed()
	{
		if (m_changed_listeners.empty())
		{
			return;
		}

        if (m_mode == SceneMode::Scene2D)
        {
            if (m_changed_frustum == nullptr)
            {
                m_changed_position = m_position;
                m_changed_frustum  = m_frustum->clone();
                return;
            }

            const vec3d position      = m_position;
            const vec3d last_position = m_changed_position;

            OrthographicOffCenterFrustum* frustum      = static_cast<OrthographicOffCenterFrustum*>(m_frustum);
            OrthographicOffCenterFrustum* last_frustum = static_cast<OrthographicOffCenterFrustum*>(m_changed_frustum);

            double x0 = position.x      + frustum->get_left();
            double x1 = position.x      + frustum->get_right();
            double x2 = last_position.x + last_frustum->get_left();
            double x3 = last_position.x + last_frustum->get_right();

            double y0 = position.y      + frustum->get_bottom();
            double y1 = position.y      + frustum->get_top();
            double y2 = last_position.y + last_frustum->get_bottom();
            double y3 = last_position.y + last_frustum->get_top();

            double left_x   = ::fmax(x0, x2);
            double right_x  = ::fmin(x1, x3);
            double bottom_y = ::fmax(y0, y2);
            double top_y    = ::fmin(y1, y3);

            double area_percentage;

            if (left_x >= right_x || bottom_y >= y1)
            {
                area_percentage = 1.0;
            }
            else
            {
                OrthographicOffCenterFrustum* area_ref = last_frustum;

                if (x0 < x2 && x1 > x3 && y0 < y2 && y1 > y3)
                {
                    area_ref = frustum;
                }

                area_percentage =
                        1.0 -
                       ((right_x - left_x) * (top_y - bottom_y)) / ((area_ref->get_right() - area_ref->get_left()) * (area_ref->get_top() - area_ref->get_bottom()));
            }

            if (area_percentage > m_percentage_changed)
            {
                changed(area_percentage);
                m_changed_position  = m_position;
                m_changed_frustum   = m_frustum->clone(m_changed_frustum);
            }

            return;
        }

		if (std::isnan(m_changed_direction.x) ||
			std::isnan(m_changed_direction.y) || 
			std::isnan(m_changed_direction.z) || 
			std::isnan(m_changed_position.x) || 
			std::isnan(m_changed_position.y) || 
			std::isnan(m_changed_position.z))
		{
			m_changed_position  = m_position_wc;
			m_changed_direction = m_direction_wc;
			return;
		}

		double dir_angle = ArgeoMath::acos_clamped(
			vec_dot(m_direction_wc, m_changed_direction));

		double dir_percentage = dir_angle / (m_frustum->get_field_of_view_y() * 0.5);

		double distance = vec_distance(m_position_wc, m_changed_position);
		double height_percentage = distance / m_position_geodetic.get_height();

		if (dir_percentage > m_percentage_changed || height_percentage > m_percentage_changed) 
		{
			changed(std::max(dir_percentage, height_percentage));
			m_changed_position  = m_position_wc;
			m_changed_direction = m_direction_wc;
		}
	}

    void Camera::update(const SceneMode& mode)
    {
        if ((mode == SceneMode::Scene2D) &&
            (dynamic_cast<OrthographicOffCenterFrustum*>(m_frustum) == nullptr))
        {
            throw std::logic_error("An OrthographicOffCenterFrustum is required in 2D.");
        }

        if ((mode == SceneMode::Scene3D || mode == SceneMode::Morphing) &&
            ((dynamic_cast<PerspectiveFrustum*>(m_frustum) == nullptr) && (dynamic_cast<OrthographicFrustum*>(m_frustum) == nullptr)))
        {
            throw std::logic_error("A PerspectiveFrustum or OrthographicFrustum is required in 3D and Columbus view");
        }


        bool update_frustum = false;

        if (m_mode != mode)
        {
            m_mode   = mode;
            m_mode_changed  = mode   != SceneMode::Morphing;
            update_frustum  = m_mode == SceneMode::Scene2D;
        }

        if (update_frustum)
        {
            OrthographicOffCenterFrustum* frustum
                    = m_max_2D_frustum
                    = static_cast<OrthographicOffCenterFrustum*>(m_frustum->clone());


            double max_zoom_out = 2.0;
            double ratio = frustum->get_top() / frustum->get_right();

            frustum->set_right(m_max_coord.x * max_zoom_out);
            frustum->set_left(-frustum->get_right());
            frustum->set_top(ratio * frustum->get_right());
            frustum->set_bottom(-frustum->get_top());
        }


        if (m_mode == SceneMode::Scene2D)
        {
            clamp_move_2D(m_position);
        }

        //var globe = this._scene.globe;
        //var globeFinishedUpdating = !defined(globe) || (globe._surface.tileProvider.ready && globe._surface._tileLoadQueueHigh.length === 0 && globe._surface._tileLoadQueueMedium.length === 0 && globe._surface._tileLoadQueueLow.length === 0 && globe._surface._debug.tilesWaitingForChildren === 0);
        //if (this._suspendTerrainAdjustment)
        //{
        //    this._suspendTerrainAdjustment = !globeFinishedUpdating;
        //}
        //this._adjustHeightForTerrain();
    }

    void Camera::clamp_move_2D(vec3d& position)
    {
        const bool rotatable_2D  = m_scene->get_map_mode_2D() == MapMode2D::Rotate;

        const double max_projected_x = m_max_coord.x;
        const double max_projected_y = m_max_coord.y;

        double min_x;
        double max_x;

        if (rotatable_2D)
        {
            max_x = max_projected_x;
            min_x = -max_x;
        }
        else
        {
            max_x = position.x - max_projected_x * 2.0;
            min_x = position.x + max_projected_x * 2.0;
        }

        if (position.x > max_projected_x)
        {
            position.x = max_x;
        }

        if (position.x < -max_projected_x)
        {
            position.x = min_x;
        }

        if (position.y > max_projected_y)
        {
            position.y = max_projected_y;
        }

        if (position.y < -max_projected_y)
        {
            position.y = -max_projected_y;
        }
    }

    Ray Camera::get_pick_ray_orthographic(const vec2d& window_position)
    {
        const unsigned int width  = m_scene->get_drawing_surface_width();
        const unsigned int height = m_scene->get_drawing_surface_height();

        OrthographicOffCenterFrustum* frustum;

        if (dynamic_cast<OrthographicFrustum*>(m_frustum) != nullptr)
        {
            frustum = static_cast<OrthographicFrustum*>(m_frustum)->get_offcenter_frustum();
        }
        else
        {
            frustum = static_cast<OrthographicOffCenterFrustum*>(m_frustum);
        }

        double x = (2.0 / width) * window_position.x - 1.0;
        x *= (frustum->get_right() - frustum->get_left()) * 0.5;

        double y = (2.0 / height) * (height - window_position.y) - 1.0;
        y *= (frustum->get_top() - frustum->get_bottom()) * 0.5;

        vec3d origin    = m_position;
        origin         += m_right * x + m_up * y;

        Ray result;
        result.origin    = origin;
        result.direction = m_direction_wc;

        if (m_mode == SceneMode::SceneCV)
        {
            result.origin = vec3d(
                    result.origin.z,
                    result.origin.x,
                    result.origin.y);
        }

        return result;
    }

    Ray Camera::get_pick_ray_perspective(const vec2d& window_position)
    {
        const unsigned int width  = m_scene->get_drawing_surface_width();
        const unsigned int height = m_scene->get_drawing_surface_height();

        const double tan_phi   = ::tan(m_frustum->get_field_of_view_y() * 0.5);
        const double tan_theta = m_frustum->get_aspect_ratio() * tan_phi;
        const double near      = m_frustum->get_near();

        const double x = (2.0 / width) * window_position.x - 1.0;
        const double y = (2.0 / height) * (height - window_position.y) - 1.0;

        vec3d origin      = m_position_wc;
        vec3d near_center = m_direction_wc * near + m_position_wc;

        vec3d x_dir = m_right_wc * x * near * tan_theta;
        vec3d y_dir = m_up_wc * y * near * tan_phi;

        vec3d direction = near_center + x_dir;
        direction += y_dir;
        direction -= m_position_wc;
        direction  = vec_normalize(direction);

        Ray result;
        result.origin    = origin;
        result.direction = direction;

        return result;
    }

    Ray Camera::get_pick_ray(const vec2d& window_position)
    {
        if (dynamic_cast<PerspectiveFrustum*>(m_frustum) != nullptr ||
            dynamic_cast<PerspectiveOffCenterFrustum*>(m_frustum) != nullptr)
        {
            return get_pick_ray_perspective(window_position);
        }

        return get_pick_ray_orthographic(window_position);
    }

    void Camera::adjust_orthographic_frustum(const bool& zooming)
    {
        if (dynamic_cast<OrthographicFrustum*>(m_frustum) == nullptr)
        {
            return;
        }

        if (!zooming && m_position_geodetic.get_height() < 150000.0)
        {
            return;
        }

        if (m_transform != mat4d::identity())
        {
            static_cast<OrthographicFrustum*>(m_frustum)->set_width(vec_length(m_position));
            return;
        }

        vec3d ray_intersection;
        vec3d depth_intersection;
        bool depth_intersection_defined = false;
        bool ray_intersection_defined   = false;

        ITerrain* terrain = m_scene->get_terrain();

        if (terrain != nullptr)
        {
            vec2d mouse_position = vec2d(
            m_scene->get_drawing_surface_width() / 2.0,
            m_scene->get_drawing_surface_height() / 2.0);

            Ray ray = get_pick_ray(mouse_position);

            ray_intersection   = terrain->pick(ray, m_scene);
            depth_intersection = m_scene->pick_position_world_coordinates(mouse_position);

            depth_intersection_defined = !vec_undefined(depth_intersection);
            ray_intersection_defined   = !vec_undefined(ray_intersection);

            if (depth_intersection_defined &&
                ray_intersection_defined)
            {
                const double depth_distance = depth_intersection_defined ?
                                             vec_distance(depth_intersection, m_position_wc) :
                                             std::numeric_limits<double>::infinity();

                const double ray_distance   = ray_intersection_defined ?
                                             vec_distance(ray_intersection, m_position_wc) :
                                             std::numeric_limits<double>::infinity();

                static_cast<OrthographicFrustum*>(m_frustum)->set_width(::fmin(depth_distance, ray_distance));
            }
            else if (depth_intersection_defined)
            {
                static_cast<OrthographicFrustum*>(m_frustum)->set_width(vec_distance(depth_intersection, m_position_wc));
            }
            else if (ray_intersection_defined)
            {
                static_cast<OrthographicFrustum*>(m_frustum)->set_width(vec_distance(ray_intersection, m_position_wc));
            }
        }

        if (terrain == nullptr || (!ray_intersection_defined && !depth_intersection_defined))
        {
            const double distance = ::fmax(m_position_geodetic.get_height(), 0.0);
            static_cast<OrthographicFrustum*>(m_frustum)->set_width(distance);
        }
    }

	double Camera::get_distance_to_bounding_sphere(BoundingSphere *bounding_sphere)
	{
		vec3d to_center =  m_position_wc - bounding_sphere->get_center();
		vec3d proj      = m_direction_wc * vec_dot(to_center, m_direction_wc);
		return ::fmax(0.0, vec_length(proj) - bounding_sphere->get_radius());
	}

    double Camera::get_pixel_size(
            BoundingSphere *bounding_sphere,
            const unsigned int &surface_width,
            const unsigned int &surface_height)
    {
        double distance  = get_distance_to_bounding_sphere(bounding_sphere);
        vec2d pixel_size = m_frustum->get_pixel_dimension(surface_width, surface_height, distance);
        return std::max(pixel_size.x, pixel_size.y);
    }
}