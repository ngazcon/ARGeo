#include "include/DevicePoseCameraController.h"
#include "include/SensorManager.h"
#include "Transforms.h"

namespace argeo
{
	// TODO: Agregar callback de cambio de estado: { Initializing, Standing, Moving, Stopped, Forced }
	// TODO: Agregar callback de nueva posicion GPS : {Accuracy, Speed}
    // TODO: Metodo forzar update.

	const double HEIGHT_FROM_GROUND_3D = 100;
    const double HEIGHT_FROM_GROUND_2D = 1.7e6;

	DevicePoseCameraController::DevicePoseCameraController(ITerrain* terrain)
		: m_terrain(terrain)
		, m_yaw(0.0)
		, m_pitch(0.0)
		, m_roll(0.0)
		, m_current_yaw(0.0)
		, m_current_pitch(0.0)
		, m_current_roll(0.0)		
		, m_orientation_updates_enabled(false)
		, m_location_updates_enabled(false)
		, m_has_location_provider_height(false)
		, m_orientation_changed(false)
		, m_location_changed(false)
		, m_position(0.0f, 0.0f)
		, m_ground_height(std::numeric_limits<double>::max())
		, m_geomagnetic_declination(0.0)
		, m_sensor_low_pass_alpha(0.05)
		, m_first_update(true)
		, m_frame_count(0)
        , m_remove_update_height_callback(nullptr)
	    , m_gps_listener(new GPSListener())
	{
        m_gps_listener->add_listener(this);
	    enable_location_updates();
        enable_orientation_updates();
	}


	DevicePoseCameraController::~DevicePoseCameraController()
	{
		disable_location_updates();
		disable_orientation_updates();
	}

    void DevicePoseCameraController::set_network_standing_frec(const int& value)
    {
        m_gps_listener->set_network_standing_frec(value);
    }

    int  DevicePoseCameraController::get_network_standing_frec() const
    {
        return m_gps_listener->get_network_standing_frec();
    }

    void DevicePoseCameraController::set_network_moving_frec(const int& value)
    {
        m_gps_listener->set_network_moving_frec(value);
    }

    int  DevicePoseCameraController::get_network_moving_frec() const
    {
        return m_gps_listener->get_network_moving_frec();
    }

    void DevicePoseCameraController::set_gps_moving_frec(const int& value)
    {
        m_gps_listener->set_gps_moving_frec(value);
    }

    int  DevicePoseCameraController::get_gps_moving_frec() const
    {
        return m_gps_listener->get_gps_moving_frec();
    }

    void DevicePoseCameraController::set_gps_standing_frec(const int& value)
    {
        m_gps_listener->set_gps_standing_frec(value);
    }

    int  DevicePoseCameraController::get_gps_standing_frec() const
    {
        return m_gps_listener->get_gps_standing_frec();
    }

    void DevicePoseCameraController::set_moving_window(const int& value)
    {
        m_gps_listener->set_moving_window(value);
    }

    int  DevicePoseCameraController::get_moving_window() const
    {
        return m_gps_listener->get_moving_window();
    }

    void DevicePoseCameraController::set_standing_window(const int& value)
    {
        set_standing_window(value);
    }

    int  DevicePoseCameraController::get_standing_window() const
    {
        return m_gps_listener->get_standing_window();
    }

    void DevicePoseCameraController::set_initial_samples(const int& value)
    {
        set_initial_samples(value);
    }

    int  DevicePoseCameraController::get_initial_samples() const
    {
        return m_gps_listener->get_initial_samples();
    }

    void DevicePoseCameraController::set_accuracy_delta(const int& value)
    {
        m_gps_listener->set_accuracy_delta(value);
    }

    int  DevicePoseCameraController::get_accuracy_delta() const
    {
        return m_gps_listener->get_accuracy_delta();
    }

    void DevicePoseCameraController::force_location_updates()
    {
        m_gps_listener->force_location_updates();
    }

    GPSState DevicePoseCameraController::get_gps_state()
    {
        return m_gps_listener->get_state();
    }

	void DevicePoseCameraController::disable_location_updates()
    {
        if (m_location_updates_enabled)
        {
            m_gps_listener->stop_network_provider();
            m_location_updates_enabled = false;
        }

        /*
        if (m_location_updates_enabled)
        {
            LocationManager::get_instance().remove_updates(this);
            m_location_updates_enabled = false;
        }
         */
    }

    void DevicePoseCameraController::enable_location_updates()
    {
        if (!m_location_updates_enabled)
        {
            m_gps_listener->start_network_provider();
            m_location_updates_enabled = true;
        }

        /*
        if (!m_location_updates_enabled)
        {
            LocationManager::get_instance().request_location_updates(
                    "gps",
                    4000,
                    0,
                    this);

            LocationManager::get_instance().request_location_updates(
                    "network",
                    1000,
                    0,
                    this);

            m_location_updates_enabled = true;
        }
         */
    }


    void DevicePoseCameraController::disable_orientation_updates()
    {
        if (m_orientation_updates_enabled)
        {
            SensorManager::get_instance().remove_sensor_listener(this);
            m_orientation_updates_enabled = false;
        }
    }

	void DevicePoseCameraController::enable_orientation_updates()
	{
        if (!m_orientation_updates_enabled)
        {
            Sensor* rotation_vector_sensor = SensorManager::get_instance()
                    .get_default_sensor(Sensor::SensorType::RotationVector);

            SensorManager::get_instance().add_sensor_listener(
                    this,
                    rotation_vector_sensor,
                    Sensor::SensorDelay::Game);

            m_orientation_updates_enabled = true;
        }
	}

	void DevicePoseCameraController::set_position(Geodetic3D value)
	{
		std::unique_lock<std::mutex> location_lock(m_location_mutex);
		if (m_position != value)
		{
			m_position = Geodetic2D(
				value.get_longitude(),
				value.get_latitude());

			m_ground_height = value.get_height();

			std::unique_lock<std::mutex> altitude_lock(m_altitude_mutex);
            m_location_changed = true;
		}
	}

	Geodetic3D DevicePoseCameraController::get_position() const
	{
		return Geodetic3D(m_position, m_ground_height);
	}

	void DevicePoseCameraController::set_position(Geodetic2D value)
	{
		std::unique_lock<std::mutex> lock(m_location_mutex);
		if (m_position != value)
		{
			m_position = value;
			std::unique_lock<std::mutex> altitude_lock(m_altitude_mutex);
			m_location_changed = true;
		}
	}

	double DevicePoseCameraController::get_yaw() const
	{
		return m_yaw;
	}

	double DevicePoseCameraController::get_pitch() const
	{
		return m_pitch;
	}

	double DevicePoseCameraController::get_roll() const
	{
		return m_roll;
	}

    double DevicePoseCameraController::get_sensor_low_pass_filter_alpha() const
    {
        return m_sensor_low_pass_alpha;
    }

    void DevicePoseCameraController::set_sensor_low_pass_filter_alpha(const double &value)
    {
        m_sensor_low_pass_alpha = value;
    }

	void DevicePoseCameraController::on_location_changed(const LocationChangedEventArgs& args)
    {
        if (!m_location_updates_enabled)
        {
            return;
        }

        //if (m_location_filter.is_better_location(args))
        {
//            m_location_filter.set_current_best_location(args);


            set_position(Geodetic2D(
                    args.location.get_longitude(),
                    args.location.get_latitude()));

            /*
                    set_position(Geodetic2D(
                    ArgeoMath::to_radians(-61.984323),
                    ArgeoMath::to_radians(-38.074678)));

            set_position(Geodetic2D(
                    ArgeoMath::to_radians(0.0),
                    ArgeoMath::to_radians(0.0)));
            m_location_changed = true;
             */

            if (args.has_altitude && m_ground_height != args.location.get_height())
            {
                return;
                m_ground_height = args.location.get_height();
                m_has_location_provider_height = true;
            }
        }

        m_geomagnetic_declination = args.geomagnetic_declination;
    }

	void DevicePoseCameraController::on_provider_enabled(const std::string& provider)
	{ }

	void DevicePoseCameraController::on_provider_disabled(const std::string& provider)
	{ }

	void DevicePoseCameraController::on_status_changed(const std::string& provider, const int& status)
	{ }

	void DevicePoseCameraController::on_accuracy_changed(Sensor* sensor, int accuracy)
	{ }

	double low_pass_filter(const double& input, double& output, const double& alpha)
	{
		output = output + alpha * (input - output);
		return output;
	}

	void DevicePoseCameraController::on_sensor_changed(SensorEvent event)
	{
        if (!m_orientation_updates_enabled)
        {
            return;
        }

        std::vector<float> orientation(3);
		std::vector<float> matrix(16);

		SensorManager::get_rotation_matrix_from_vector(
			matrix.data(),
			event.vector,
			4,
			16);

		SensorManager::remap_coordinate_system(
			matrix.data(),
			SensorManager::AXIS_Z,
			SensorManager::AXIS_MINUS_X,
			matrix.data(),
			16);

		SensorManager::get_orientation(
			matrix.data(),
			orientation.data(),
			16);

		std::unique_lock<std::mutex> lock(m_orientation_mutex);

		// Low pass filter
		m_yaw   = low_pass_filter(m_geomagnetic_declination + orientation[0], m_yaw, m_sensor_low_pass_alpha);
		m_pitch = low_pass_filter(-orientation[1], m_pitch, m_sensor_low_pass_alpha);
		m_roll  = low_pass_filter(0, m_roll, m_sensor_low_pass_alpha);// -orientation[2];

		m_orientation_changed = true;
	}

	void DevicePoseCameraController::update(Camera* camera)
	{
		if (camera->m_mode != SceneMode::Scene3D)
		{
            if (m_remove_update_height_callback != nullptr)
            {
                m_remove_update_height_callback();
                m_remove_update_height_callback = nullptr;
            }

            disable_orientation_updates();
            disable_location_updates();
			return;
		}

        std::unique_lock<std::mutex> location_lock(m_location_mutex);
        std::unique_lock<std::mutex> altitude_lock(m_altitude_mutex);
        std::unique_lock<std::mutex> orientation_lock(m_orientation_mutex);

        if (m_has_location_provider_height)
        {
            m_has_location_provider_height = false;
        }
        else
        {
            m_ground_height = m_terrain->get_point_height(m_position);
            m_ground_height += HEIGHT_FROM_GROUND_3D;
        }

        Geodetic3D geodetic(m_position, m_ground_height);

        if (m_location_changed)
        {
            m_update_height_function = [&, camera, geodetic](vec3d& position)
            {
                Ellipsoid ellipsoid = m_terrain->get_ellipsoid();
                Geodetic3D geodetic_position = ellipsoid.to_geodetic3D(position);
                geodetic_position = Geodetic3D(
                        geodetic_position.get_longitude(),
                        geodetic_position.get_latitude(),
                        geodetic_position.get_height() + geodetic.get_height());
                camera->set_position(ellipsoid.to_geocentric3D(geodetic_position).to_vec3d());
                /*
                camera->set_view(
                        position,
                        m_yaw,
                        m_pitch,
                        m_roll);
                */
            };

            if (m_remove_update_height_callback != nullptr)
            {
                m_remove_update_height_callback();
            }

            m_remove_update_height_callback = m_terrain->get_raster_source()->add_listener(
                    m_update_height_function,
                    geodetic);


            vec3d position = m_terrain->get_ellipsoid().to_geocentric3D(geodetic).to_vec3d();
            m_update_height_function(position);
        }

        if (m_orientation_changed)
        {
            // Get the current camera euler angles.
            const double camera_yaw   = camera->get_yaw();
            const double camera_pitch = camera->get_pitch();
            const double camera_roll  = camera->get_roll();

            // Get the deltas respect the last frame.
            const double delta_yaw   = m_current_yaw   - m_yaw;
            const double delta_pitch = m_current_pitch - m_pitch;
            const double delta_roll  = m_current_roll  - m_roll;

            // Compute the rotations in enu local space using quaternions.
            const quaternion yaw_quat    = quaternion::from_axis_angle(vec3d::unit_z(), -(camera_yaw - delta_yaw));// camera_yaw - delta_yaw);
            const quaternion pitch_quat  = quaternion::from_axis_angle(vec3d::unit_x(), camera_pitch - delta_pitch);//camera_pitch - delta_pitch);
            const quaternion orientation = quat_normalize(yaw_quat * pitch_quat);

            // Apply rotation in the ENU coordinate frame
            const vec3d direction_enu = orientation * vec3d::unit_y();
            const vec3d right_enu     = orientation * vec3d::unit_x();

            // Transform from ENU to ECEF coordinate frame.
            const vec3d direction_ecef = Transforms::enu_to_ecef(geodetic, direction_enu);
            const vec3d right_ecef     = Transforms::enu_to_ecef(geodetic, right_enu);
            const vec3d up_ecef		   = vec_cross(right_ecef, direction_ecef);

            camera->set_direction(direction_ecef);
            camera->set_right(right_ecef);
            camera->set_up(up_ecef);
        }

		m_current_yaw   = m_yaw;
		m_current_pitch = m_pitch;
		m_current_roll  = m_roll;

		m_orientation_changed = m_location_changed = false;
	}
}
