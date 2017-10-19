#include "Transforms.h"
#include "SceneTransitioner.h"
#include "Scene.h"

namespace argeo
{
    SceneTransitioner::SceneTransitioner(Scene *scene)
        : m_scene(scene)
        , m_scratch_orthographic_frustum(new OrthographicFrustum())
        , m_scratch_orthographic_off_center_frustum(new OrthographicOffCenterFrustum())
        , m_scratch_perspective_frustum(new PerspectiveFrustum())
        , m_complete_morph(nullptr)
        , m_previous_mode(SceneMode::Scene3D)
    {
        if (scene == nullptr)
        {
            throw std::invalid_argument("Scene is required");
        }
    }

    SceneTransitioner::~SceneTransitioner()
    {
        destroy_morph_handler();
        delete m_scratch_to_2D_camera.frustum;
    }

    void SceneTransitioner::destroy_morph_handler()
    {
        //var tweens = transitioner._currentTweens;
        //for (unsigned int i = 0; i < tweens.length; ++i)
        //{
        //     tweens[i].cancelTween();
        //}

        //transitioner._currentTweens.length = 0;
        //transitioner._morphHandler = transitioner._morphHandler && transitioner._morphHandler.destroy();
    }

    void SceneTransitioner::complete_morph()
    {
        if (m_complete_morph != nullptr)
        {
            m_complete_morph();
        }
    }

    void SceneTransitioner::morph_to_2D(
            const double& duration,
            const Ellipsoid &ellipsoid)
    {
        complete_morph();

        m_previous_mode = m_scene->get_mode();
        m_morph_to_orthographic =
                dynamic_cast<OrthographicFrustum*>(m_scene->get_camera()->get_frustum()) != nullptr;

        if (m_previous_mode == SceneMode::Scene2D || m_previous_mode == SceneMode::Morphing)
        {
            return;
        }

        m_scene->morph_start(
                this,
                m_previous_mode,
                SceneMode ::Scene2D,
                true);

        m_scene->m_mode = SceneMode::Morphing;
        m_scene->get_camera()->set_transform(mat4d::identity());


        if (m_previous_mode == SceneMode::SceneCV)
        {
            morph_from_CV_to_2D(duration);
        }
        else
        {
            morph_from_3D_to_2D(duration, ellipsoid);
        }

        if (duration == 0.0)
        {
            complete_morph();
        }
    }

    void SceneTransitioner::morph_to_3D(
            const double& duration,
            const Ellipsoid &ellipsoid)
    {
        complete_morph();

        m_previous_mode = m_scene->get_mode();

        if (m_previous_mode == SceneMode::Scene3D || m_previous_mode == SceneMode::Morphing)
        {
            return;
        }

        m_scene->morph_start(
                this,
                m_previous_mode,
                SceneMode ::Scene3D,
                true);


        m_scene->m_mode = SceneMode::Morphing;
        m_scene->get_camera()->set_transform(mat4d::identity());

        if (m_previous_mode == SceneMode::Scene2D)
        {
            morph_from_2D_to_3D(duration, ellipsoid);
        }
        else
        {
            EndCamera& camera_3D =
                    duration > 0.0 ?
                    m_scratch_end_camera :
                    get_CV_to_3D_camera(ellipsoid);

            if (duration > 0.0)
            {
                camera_3D.position =
                        Geocentric3D::from_degrees(0.0, 0.0, 5.0 * ellipsoid.get_maximum_radius(), ellipsoid).to_vec3d();
                camera_3D.direction =
                        vec_normalize(-camera_3D.position);
                camera_3D.up = vec3d::unit_z();
            }

            std::function<void()> complete = complete_3D_callback(camera_3D);
            create_morph_handler(complete);
            morph_from_CV_to_3D(duration, camera_3D, complete);
        }

        if (duration == 0.0)
        {
            complete_morph();
        }
    }

    void SceneTransitioner::morph_to_CV(
            const double& duration,
            const Ellipsoid &ellipsoid)
    {
        complete_morph();

        m_previous_mode = m_scene->get_mode();
        m_morph_to_orthographic =
                dynamic_cast<OrthographicFrustum*>(m_scene->get_camera()->get_frustum()) != nullptr;

        if (m_previous_mode == SceneMode::SceneCV || m_previous_mode == SceneMode::Morphing)
        {
            return;
        }

        m_scene->morph_start(
                this,
                m_previous_mode,
                SceneMode ::SceneCV,
                true);

        vec3d position;
        vec3d direction;
        vec3d up;

        if (duration > 0.0)
        {
            position.x =  0.0;
            position.y = -1.0;
            position.z =  1.0;
            position *=   vec_normalize(position) * 5.0 * ellipsoid.get_maximum_radius();
            direction = - vec_normalize(position);
            up        =   vec_cross(vec3d::unit_x(), direction);
        }
        else
        {
            Camera* camera = m_scene->get_camera();

            if (m_previous_mode == SceneMode::Scene2D)
            {
                position   = camera->get_position();

                position.z =
                        camera->get_frustum()->get_right() -
                        camera->get_frustum()->get_left();

                direction  = -vec3d::unit_z();
                up         =  vec3d::unit_y();
            }
            else
            {
                position  = camera->get_position_wc();
                direction = camera->get_direction_wc();
                up        = camera->get_up_wc();

                const Geocentric3D surface_point = ellipsoid.scale_to_geodetic_surface(position);
                mat4d to_enu = Transforms::enu_to_fixed_frame(surface_point.to_vec3d(), ellipsoid);
                to_enu = mat_inverse_transformation(to_enu);

                position  = m_scene->get_projection()->project(ellipsoid.to_geodetic3D(position)).to_vec3d();
                direction = mat_multiply_by_point_as_vector(to_enu, direction);
                up        = mat_multiply_by_point_as_vector(to_enu, up);
            }
        }

        Frustum* frustum;
        if (m_morph_to_orthographic)
        {
            m_scratch_orthographic_frustum->set_width(
                    m_scene->get_camera()->get_frustum()->get_right() -
                    m_scene->get_camera()->get_frustum()->get_left());

            m_scratch_orthographic_frustum->set_aspect_ratio(
                    (double) m_scene->get_drawing_surface_width() /
                    (double) m_scene->get_drawing_surface_height());

            frustum = m_scratch_orthographic_frustum.get();
        }
        else
        {
            m_scratch_perspective_frustum->set_field_of_view(ArgeoMath::to_radians(60.0));
            m_scratch_perspective_frustum->set_aspect_ratio(
                    (double) m_scene->get_drawing_surface_width() /
                    (double) m_scene->get_drawing_surface_height());

            frustum = m_scratch_perspective_frustum.get();
        }

        ToCVCamera& camera_CV = m_scratch_to_CV_camera;

        camera_CV.position  = position;
        camera_CV.direction = direction;
        camera_CV.up        = up;
        camera_CV.frustum   = frustum;

        std::function<void()> complete = complete_CV_callback(camera_CV);
        create_morph_handler(complete);

        if (m_previous_mode == SceneMode ::Scene2D)
        {
            morph_from_2D_to_CV(duration, camera_CV, complete);
        }
        else
        {
            camera_CV.position_2D  = mat_multiply_by_point(Camera::TRANSFORM_2D, position);
            camera_CV.direction_2D = mat_multiply_by_point_as_vector(Camera::TRANSFORM_2D, direction);
            camera_CV.up_2D        = mat_multiply_by_point_as_vector(Camera::TRANSFORM_2D, up);

            m_scene->m_mode      = SceneMode::Morphing;
            morph_from_3D_to_CV(duration, camera_CV, complete);
        }

        if (duration == 0.0)
        {
            complete_morph();
        }
    }


    vec3d SceneTransitioner::columbus_view_morph(
            const vec3d &start_position,
            const vec3d &end_position,
            const double &time)
    {
        return vec_lerp(start_position, end_position, time);
    }

    void SceneTransitioner::morph_from_CV_to_2D(const double &duration)
    {
        const double half_duration = duration * 0.5;
        Camera* camera = m_scene->get_camera();

        const vec3d start_pos = camera->get_position();
        const vec3d start_dir = camera->get_direction();
        const vec3d start_up  = camera->get_up();

        const vec3d end_dir =  vec3d::unit_y();
        const vec3d end_up  = -vec3d::unit_z();
        vec3d end_pos;

        if (duration > 0.0)
        {
            end_pos   = vec3d::zero();
            end_pos.z = 5.0 * m_scene->get_projection()->get_ellipsoid().get_maximum_radius();
        }
        else
        {
            end_pos = start_pos;

            Ray ray;
            ray.origin = mat_multiply_by_point(Camera::TRANSFORM_2D, start_pos);
            ray.direction = mat_multiply_by_point_as_vector(Camera::TRANSFORM_2D, start_dir);

            ITerrain* terrain = m_scene->get_terrain();
            if (terrain != nullptr)
            {
                vec3d pick_pos = terrain->pick(
                        ray,
                        m_scene);

                if (!vec_undefined(pick_pos))
                {
                    end_pos = mat_multiply_by_point(Camera::TRANSFORM_2D_INVERSE, pick_pos);
                    end_pos.z += vec_distance(start_pos, end_pos);
                }
            }
        }

        OrthographicOffCenterFrustum* frustum = m_scratch_orthographic_off_center_frustum.get();
        frustum->set_right(end_pos.z * 0.5);
        frustum->set_left(-frustum->get_right());
        frustum->set_top(frustum->get_right() * ((double) m_scene->get_drawing_surface_height() / (double) m_scene->get_drawing_surface_width()));
        frustum->set_bottom(-frustum->get_top());

        EndCamera& camera2D = m_scratch_end_camera;
        camera2D.position  = end_pos;
        camera2D.direction = end_dir;
        camera2D.up        = end_up;
        camera2D.frustum   = frustum;

        std::function<void()> complete = complete_2D_callback(camera2D);
        create_morph_handler(complete);

        std::function<void(const double&)> update_cv =
                [&, camera, start_pos, start_dir, start_up, end_pos, end_dir, end_up] (const double& value)
        {
            camera->set_position(columbus_view_morph(start_pos, end_pos, value));
            camera->set_direction(columbus_view_morph(start_dir, end_dir, value));
            camera->set_up(columbus_view_morph(start_up, end_up,  value));

            camera->set_right(vec_normalize(
                    vec_cross(camera->get_direction(), camera->get_up())));

            camera->adjust_orthographic_frustum(true);
        };

        std::function<void(Camera*, const double&)> update_height = [&] (
                Camera* camera,
                const double& height)
        {
            camera->set_position(vec3d(
                    camera->get_position().x,
                    camera->get_position().y,
                    height));
        };


        const double start_object = 0.0;
        const double stop_object  = 1.0;
        Tween* tween = m_scene->get_tween_collection()->add(
                start_object,
                stop_object,
                half_duration,
                0.0,
                EasingFunction::SmoothStep,
                update_cv,
                [&, complete, half_duration, update_height, camera2D] ()
                {
                    morph_perspective_to_orthographic(half_duration, camera2D, update_height, complete);
                }
        );
        m_tweens.insert(tween);
    }

    void SceneTransitioner::morph_from_CV_to_3D(
            const double &duration,
            const EndCamera &end_camera,
            std::function<void()> complete)
    {
        const double half_duration = duration * 0.5;
        Camera* camera = m_scene->get_camera();

        const vec3d start_pos = camera->get_position();
        const vec3d start_dir = camera->get_direction();
        const vec3d start_up  = camera->get_up();

        const vec3d end_pos = mat_multiply_by_point(Camera::TRANSFORM_2D_INVERSE, end_camera.position);
        const vec3d end_dir = mat_multiply_by_point_as_vector(Camera::TRANSFORM_2D_INVERSE, end_camera.direction);
        const vec3d end_up  = mat_multiply_by_point_as_vector(Camera::TRANSFORM_2D_INVERSE, end_camera.up);

        std::function<void(const double&)> update =
                [&, camera, start_pos, start_dir, start_up, end_pos, end_dir, end_up] (const double& value)
        {
            camera->set_position(columbus_view_morph(start_pos, end_pos, value));
            camera->set_direction(columbus_view_morph(start_dir, end_dir, value));
            camera->set_up(columbus_view_morph(start_up, end_up,  value));

            camera->set_right(vec_normalize(
                    vec_cross(camera->get_direction(), camera->get_up())));
        };

        const double start_object = 0.0;
        const double stop_object  = 1.0;
        Tween* tween = m_scene->get_tween_collection()->add(
                start_object,
                stop_object,
                half_duration,
                0.0,
                EasingFunction::SmoothStep,
                update,
                [&, complete, half_duration] ()
                {
                    add_morph_time_animations(0.0, 1.0, half_duration, complete);
                }
        );
        m_tweens.insert(tween);
    }

    void SceneTransitioner::morph_from_2D_to_CV(
            const double &duration,
            const ToCVCamera &end_camera,
            std::function<void()> complete)
    {
        double half_duration = duration * 0.5;
        Camera* camera = m_scene->get_camera();

        const vec3d end_pos = mat_multiply_by_point(Camera::TRANSFORM_2D_INVERSE, end_camera.position);
        const vec3d end_dir = mat_multiply_by_point_as_vector(Camera::TRANSFORM_2D_INVERSE, end_camera.direction);
        const vec3d end_up  = mat_multiply_by_point_as_vector(Camera::TRANSFORM_2D_INVERSE, end_camera.up);

        m_scene->m_mode = SceneMode::Morphing;

        std::function<void()> morph = [&]()
        {
            camera->set_frustum(end_camera.frustum->clone());

            vec3d start_pos = camera->get_position();
            const vec3d start_dir = camera->get_direction();
            const vec3d start_up  = camera->get_up();

            start_pos.z = end_pos.z;

            std::function<void(const double&)> update =
                    [&, start_pos, start_dir, start_up, end_pos, end_dir, end_up] (const double& value)
            {
                camera->set_position(columbus_view_morph(start_pos, end_pos, value));
                camera->set_direction(columbus_view_morph(start_dir, end_dir, value));
                camera->set_up(columbus_view_morph(start_up, end_up,  value));

                camera->set_right(vec_normalize(
                        vec_cross(camera->get_direction(), camera->get_up())));
            };

            const double start_object = 0.0;
            const double stop_object  = 1.0;
            Tween* tween = m_scene->get_tween_collection()->add(
                    start_object,
                    stop_object,
                    half_duration,
                    0.0,
                    EasingFunction::SmoothStep,
                    update,
                    complete
            );
            m_tweens.insert(tween);
        };

        if (m_morph_to_orthographic)
        {
            morph();
        }
        else
        {
            morph_orthographic_to_perspective(
                    0.0,
                    EndCamera(
                            end_camera.position,
                            end_camera.direction,
                            end_camera.up,
                            end_camera.frustum),
                    morph);
        }
    }

    void SceneTransitioner::morph_from_3D_to_CV(
            const double& duration,
            const ToCVCamera& end_camera,
            std::function<void()> complete)
    {
        const double half_duration = duration * 0.5;
        Camera* camera = m_scene->get_camera();

        const vec3d start_pos = camera->get_position();
        const vec3d start_dir = camera->get_direction();
        const vec3d start_up  = camera->get_up();

        const vec3d end_pos = mat_multiply_by_point(Camera::TRANSFORM_2D_INVERSE, end_camera.position);
        const vec3d end_dir = mat_multiply_by_point_as_vector(Camera::TRANSFORM_2D_INVERSE, end_camera.direction);
        const vec3d end_up  = mat_multiply_by_point_as_vector(Camera::TRANSFORM_2D_INVERSE, end_camera.up);

        std::function<void(const double&)> update =
                [&, camera, start_pos, start_dir, start_up, end_pos, end_up, end_dir] (const double& value)
        {
            camera->set_position(columbus_view_morph(start_pos, end_pos, value));
            camera->set_direction(columbus_view_morph(start_dir, end_dir, value));
            camera->set_up(columbus_view_morph(start_up, end_up,  value));

            camera->set_right(vec_normalize(
                    vec_cross(camera->get_direction(), camera->get_up())));
        };

        const double start_object = 0.0;
        const double stop_object  = 1.0;
        Tween* tween = m_scene->get_tween_collection()->add(
                start_object,
                stop_object,
                half_duration,
                0.0,
                EasingFunction::SmoothStep,
                update,
                [&, complete, half_duration] ()
                {
                    add_morph_time_animations(0.0, 1.0, half_duration, complete);
                }
        );
        m_tweens.insert(tween);
    }


    void SceneTransitioner::morph_from_3D_to_2D(
            const double &duration,
            const Ellipsoid &ellipsoid)
    {
        const double half_duration = duration * 0.5;

        Camera* camera = m_scene->get_camera();
        To2DCamera& camera_2D = m_scratch_to_2D_camera;

        if (half_duration > 0.0)
        {
            camera_2D.position   =  vec3d::zero();
            camera_2D.position.z =  5.0 * ellipsoid.get_maximum_radius();
            camera_2D.direction  = -vec3d::unit_z();
            camera_2D.up         =  vec3d::unit_y();
        }
        else
        {
            // TODO: ANALYSE THIS!!
            Geodetic3D geodetic  = ellipsoid.to_geodetic3D(camera->get_position_wc());
            camera_2D.position   =  m_scene->get_projection()->project(geodetic).to_vec3d();
            camera_2D.position.z =  5.0 * ellipsoid.get_maximum_radius();

            camera_2D.direction = -vec3d::unit_z();
            camera_2D.up        =  vec3d::unit_y();

            Ray ray;
            ray.origin    = camera_2D.position_2D;
            ray.direction = camera->get_direction_wc();

            const Geocentric3D surface_point = ellipsoid.scale_to_geodetic_surface(camera->get_position_wc());
            const mat4d to_enu = mat_inverse_transformation(
                    Transforms::enu_to_fixed_frame(surface_point.to_vec3d(), ellipsoid));


            vec3d ray_direction = mat_multiply_by_point_as_vector(to_enu, ray.direction);
            ray_direction = mat_multiply_by_point_as_vector(Camera::TRANSFORM_2D, ray_direction);
            ray.direction = ray_direction;

            ITerrain* terrain = m_scene->get_terrain();
            if (terrain != nullptr)
            {
                vec3d picked_pos = terrain->pick(
                        ray,
                        m_scene);

                if (!vec_undefined(picked_pos))
                {
                    const double height   = vec_distance(camera_2D.position_2D, picked_pos);
                    picked_pos.x         += height;
                    camera_2D.position_2D = picked_pos;
                }
            }
        }

        camera_2D.position_2D  = mat_multiply_by_point(Camera::TRANSFORM_2D, camera_2D.position);
        camera_2D.direction_2D = mat_multiply_by_point_as_vector(Camera::TRANSFORM_2D, camera_2D.direction);
        camera_2D.up_2D        = mat_multiply_by_point_as_vector(Camera::TRANSFORM_2D, camera_2D.up);

        Frustum* frustum = camera_2D.frustum;
        frustum->set_right(camera_2D.position.z * 0.5);
        frustum->set_left(-frustum->get_right());
        frustum->set_top(frustum->get_right() * ((double) m_scene->get_drawing_surface_height() / (double) m_scene->get_drawing_surface_width()));
        frustum->set_bottom(-frustum->get_top());

        EndCamera& end_camera = m_scratch_end_camera;

        end_camera.position = mat_multiply_by_point(Camera::TRANSFORM_2D_INVERSE, camera_2D.position_2D);
        end_camera.direction = camera_2D.direction;
        end_camera.up = camera_2D.up;
        end_camera.frustum = frustum;

        std::function<void()> complete = complete_2D_callback(end_camera);
        create_morph_handler(complete);

        std::function<void(Camera*, const double&)> update_height = [&](
                Camera* camera,
                const double& height)
        {
            camera->set_position(vec3d(
                    height,
                    camera->get_position().y,
                    camera->get_position().z));
        };

        std::function<void()> complete_callback = [&, camera_2D, update_height, complete, half_duration]()
        {
            morph_perspective_to_orthographic(
                    half_duration,
                    EndCamera(
                            camera_2D.position,
                            camera_2D.direction,
                            camera_2D.up,
                            camera_2D.frustum),
                    update_height,
                    complete);
        };

        morph_from_3D_to_CV(
                half_duration,
                ToCVCamera(
                        camera_2D.position,
                        camera_2D.direction,
                        camera_2D.up,
                        camera_2D.position_2D,
                        camera_2D.direction_2D,
                        camera_2D.up_2D,
                        camera_2D.frustum),
                complete_callback);
    }

    void SceneTransitioner::morph_from_2D_to_3D(
            const double &duration,
            const Ellipsoid &ellipsoid)
    {
        double third_duration = duration / 3.0;
        Camera* camera = m_scene->get_camera();

        EndCamera& camera_3D =
                third_duration > 0.0 ?
                m_scratch_end_camera :
                get_CV_to_3D_camera(ellipsoid);

        if (third_duration > 0.0)
        {
            camera_3D.position =
                    Geocentric3D::from_degrees(0.0, 0.0, 5.0 * ellipsoid.get_maximum_radius(), ellipsoid).to_vec3d();
            camera_3D.direction =
                    vec_normalize(-camera_3D.position);
            camera_3D.up = vec3d::unit_z();
        }
        else
        {
            camera->set_position(vec3d(
                    camera->get_position().x,
                    camera->get_position().y,
                    camera->get_frustum()->get_right() - camera->get_frustum()->get_left()));
        }

        std::function<void()> complete = complete_3D_callback(camera_3D);
        create_morph_handler(complete);

        const vec3d start_pos = camera->get_position();
        const vec3d start_dir = camera->get_direction();
        const vec3d start_up  = camera->get_up();

        const vec3d end_pos =  vec3d(0.0, 0.0, 5.0 * ellipsoid.get_maximum_radius());
        const vec3d end_dir = -vec3d::unit_z();
        const vec3d end_up  =  vec3d::unit_y();

        const double start_right = camera->get_frustum()->get_right();
        const double end_right   = end_pos.z * 0.5;

        std::function<void(const double&)> update =
                [&, camera, start_pos, start_dir, start_right, start_up, end_pos, end_up, end_dir, end_right] (const double& value)
        {
            camera->set_position(columbus_view_morph(start_pos, end_pos, value));
            camera->set_direction(columbus_view_morph(start_dir, end_dir, value));
            camera->set_up(columbus_view_morph(start_up, end_up,  value));

            camera->set_right(vec_normalize(
                    vec_cross(camera->get_direction(), camera->get_up())));

            Frustum* frustum = camera->get_frustum();
            frustum->set_right(ArgeoMath::lerp(start_right, end_right, value));
            frustum->set_left(-frustum->get_right());
            frustum->set_top(frustum->get_right() * ((double) m_scene->get_drawing_surface_height() / (double) m_scene->get_drawing_surface_width()));
            frustum->set_bottom(-frustum->get_top());

            camera->set_position(vec3d(
                    camera->get_position().x,
                    camera->get_position().y,
                    2.0 * m_scene->get_projection()->get_ellipsoid().get_maximum_radius()));
        };

        std::function<void()> morph;
        Frustum* frustum;

        if (m_morph_to_orthographic)
        {
            morph = [&, frustum, camera, camera_3D, complete] () mutable
            {
                frustum = m_scratch_orthographic_frustum.get();

                m_scratch_orthographic_frustum->set_aspect_ratio((double) m_scene->get_drawing_surface_width() / (double) m_scene->get_drawing_surface_height());
                m_scratch_orthographic_frustum->set_width(camera->get_frustum()->get_right() - camera->get_frustum()->get_left());

                camera->set_frustum(frustum);
                morph_from_CV_to_3D(third_duration, camera_3D, complete);
            };
        }
        else
        {
            morph = [&] ()
            {
                frustum = m_scratch_perspective_frustum.get();

                m_scratch_perspective_frustum->set_aspect_ratio((double) m_scene->get_drawing_surface_width() / (double) m_scene->get_drawing_surface_height());
                m_scratch_perspective_frustum->set_field_of_view(ArgeoMath::to_radians(60.0));

                camera_3D.frustum = frustum;

                std::function<void()> complete_orthographic_to_perspective = [&, third_duration, camera_3D, complete]()
                {
                    morph_from_CV_to_3D(third_duration, camera_3D, complete);
                };

                morph_orthographic_to_perspective(
                        third_duration,
                        camera_3D,
                        complete_orthographic_to_perspective);
            };
        }


        if (third_duration > 0.0)
        {
            const double start_object = 0.0;
            const double stop_object  = 1.0;
            Tween* tween = m_scene->get_tween_collection()->add(
                    start_object,
                    stop_object,
                    third_duration,
                    0.0,
                    EasingFunction::SmoothStep,
                    update,
                    [&, morph] ()
                    {
                        m_scene->m_mode = SceneMode::Morphing;
                        morph();
                    }
            );
            m_tweens.insert(tween);
        }
        else
        {
            morph();
        }
    }


    void SceneTransitioner::morph_orthographic_to_perspective(
            const double &duration,
            const EndCamera& end_camera,
            const std::function<void()> &complete)
    {
        Camera* camera = m_scene->get_camera();

        const double height =
                camera->get_frustum()->get_right() -
                camera->get_frustum()->get_left();

        camera->set_frustum(end_camera.frustum->clone());

        const double end_fov = camera->get_frustum()->get_field_of_view();
        double start_fov = RADIANS_PER_DEGREE * 0.5;
        double d = height * ::tan(end_fov * 0.5);

        camera->get_frustum()->set_far(d / ::tan(start_fov * 0.5) + 10000000.0);
        camera->get_frustum()->set_near(start_fov);

        std::function<void(const double&)> update = [&, camera, start_fov, end_fov] (const double& value)
        {
            camera->get_frustum()->set_field_of_view(ArgeoMath::lerp(start_fov, end_fov, value));
            camera->set_position(vec3d(
                    camera->get_position().x,
                    camera->get_position().y,
                    d / ::tan(camera->get_frustum()->get_field_of_view() * 0.5)));
        };

        const double start_object = 0.0;
        const double stop_object  = 1.0;
        Tween* tween = m_scene->get_tween_collection()->add(
                start_object,
                stop_object,
                duration,
                0.0,
                EasingFunction::SmoothStep,
                update,
                complete
        );

        m_tweens.insert(tween);
    }


    void SceneTransitioner::morph_perspective_to_orthographic(
            const double &duration,
            const EndCamera &end_camera,
            const std::function<void(Camera *, const double&)> &update_height,
            const std::function<void()> &complete)
    {
        Camera* camera = m_scene->get_camera();

        if (dynamic_cast<OrthographicFrustum*>(camera->get_frustum()) != nullptr)
        {
            return;
        }

        camera->set_frustum(end_camera.frustum->clone());

        const double start_fov = camera->get_frustum()->get_field_of_view();
        double end_fov         = RADIANS_PER_DEGREE * 0.5;
        double d = end_camera.position.z * ::tan(start_fov * 0.5);
        camera->get_frustum()->set_far(d / ::tan(end_fov * 0.5) + 10000000.0);

        std::function<void(const double&)> update = [&, camera, start_fov, end_fov, update_height] (const double& value)
        {
            camera->get_frustum()->set_field_of_view(ArgeoMath::lerp(start_fov, end_fov, value));
            const double height = d / ::tan(camera->get_frustum()->get_field_of_view() * 0.5);
            update_height(camera, height);
        };

        const double start_object = 0.0;
        const double stop_object  = 1.0;
        Tween* tween = m_scene->get_tween_collection()->add(
                start_object,
                stop_object,
                duration,
                0.0,
                EasingFunction::SmoothStep,
                update,
                [&, camera, complete, end_camera]()
                {
                    camera->set_frustum(end_camera.frustum->clone());
                    complete();
                }
        );

        m_tweens.insert(tween);
    }

    EndCamera& SceneTransitioner::get_CV_to_3D_camera(const Ellipsoid &ellipsoid)
    {
        Camera* camera       = m_scene->get_camera();
        EndCamera& camera_3D = m_scratch_end_camera;

        Geodetic3D position_geodetic = m_scene->get_projection()->unproject(camera->get_position());
        const vec3d position = ellipsoid.to_geocentric3D(position_geodetic).to_vec3d();

        Geocentric3D surface_point = ellipsoid.scale_to_geodetic_surface(position);
        mat4d from_enu = Transforms::enu_to_fixed_frame(surface_point.to_vec3d(), ellipsoid);

        const vec3d direction = mat_multiply_by_point_as_vector(from_enu, camera->get_direction());
        const vec3d up        = mat_multiply_by_point_as_vector(from_enu, camera->get_up());

        camera_3D.position  = position;
        camera_3D.direction = direction;
        camera_3D.up        = up;

        return camera_3D;
    }

    void SceneTransitioner::create_morph_handler(std::function<void()> &complete)
    {
        /*
        if (transitioner._scene.completeMorphOnUserInput)
        {
            transitioner._morphHandler = new ScreenSpaceEventHandler(transitioner._scene.canvas, false);

            var completeMorph = function()
            {
                transitioner._morphCancelled = true;
                completeMorphFunction(transitioner);
            };

            transitioner._completeMorph = completeMorph;
            transitioner._morphHandler.setInputAction(completeMorph, ScreenSpaceEventType.LEFT_DOWN);
            transitioner._morphHandler.setInputAction(completeMorph, ScreenSpaceEventType.MIDDLE_DOWN);
            transitioner._morphHandler.setInputAction(completeMorph, ScreenSpaceEventType.RIGHT_DOWN);
            transitioner._morphHandler.setInputAction(completeMorph, ScreenSpaceEventType.WHEEL);
        }
         */
    }

    std::function<void()> SceneTransitioner::complete_2D_callback(const EndCamera &camera_2D)
    {
        return [&] ()
        {
            m_scene->m_mode = SceneMode::Scene2D;

            m_scene->m_morph_time = get_morph_time(SceneMode::Scene2D);

            destroy_morph_handler();

            Camera* camera = m_scene->get_camera();

            camera->set_position(vec3d(
                    camera_2D.position.x,
                    camera_2D.position.y,
                    m_scene->get_projection()->get_ellipsoid().get_maximum_radius() * 2.0));

            camera->set_direction(camera_2D.direction);
            camera->set_up(camera_2D.up);
            camera->set_right(vec_normalize(vec_cross(
                    camera->get_direction(),
                    camera->get_up())));
            camera->set_frustum(camera_2D.frustum->clone());

            bool was_morphing = m_complete_morph != nullptr;
            m_complete_morph = nullptr;

            m_scene->get_camera()->update(m_scene->get_mode());

            m_scene->morph_complete(
                    this,
                    m_previous_mode,
                    SceneMode::Scene2D,
                    was_morphing);
        };
    }

    std::function<void()> SceneTransitioner::complete_CV_callback(const ToCVCamera &camera_CV)
    {
        return [&] ()
        {
            m_scene->m_mode = SceneMode::SceneCV;

            m_scene->m_morph_time = get_morph_time(SceneMode::SceneCV);

            destroy_morph_handler();

            if (m_previous_mode != SceneMode::Morphing ||
                m_morph_canceled)
            {
                m_morph_canceled = false;

                Camera* camera = m_scene->get_camera();

                camera->set_position(camera_CV.position);
                camera->set_direction(camera_CV.direction);
                camera->set_up(camera_CV.up);
                camera->set_right(vec_normalize(vec_cross(
                        camera->get_direction(),
                        camera->get_up())));
            }

            bool was_morphing = m_complete_morph != nullptr;
            m_complete_morph = nullptr;

            m_scene->get_camera()->update(m_scene->get_mode());

            m_scene->morph_complete(
                    this,
                    m_previous_mode,
                    SceneMode::SceneCV,
                    was_morphing);
        };
    }

    std::function<void()> SceneTransitioner::complete_3D_callback(const EndCamera& camera_3D)
    {
        return [&] ()
        {
            m_scene->m_mode = SceneMode::Scene3D;

            m_scene->m_morph_time = get_morph_time(SceneMode::Scene3D);

            destroy_morph_handler();

            if (m_previous_mode != SceneMode::Morphing ||
                m_morph_canceled)
            {
                m_morph_canceled = false;

                Camera* camera = m_scene->get_camera();

                camera->set_position(camera_3D.position);
                camera->set_direction(camera_3D.direction);
                camera->set_up(camera_3D.up);
                camera->set_right(vec_normalize(vec_cross(
                        camera->get_direction(),
                        camera->get_up())));
            }

            bool was_morphing = m_complete_morph != nullptr;
            m_complete_morph = nullptr;

            m_scene->get_camera()->update(m_scene->get_mode());

            m_scene->morph_complete(
                    this,
                    m_previous_mode,
                    SceneMode::Scene3D,
                    was_morphing);
        };
    }

    void SceneTransitioner::add_morph_time_animations(
            const double &start,
            const double &stop,
            const double &duration,
            std::function<void()> complete)
    {
        // Later, this will be linear and each object will adjust, if desired, in its vertex shader.
        const double start_object = start;
        const double stop_object  = stop;

        std::function<void(const double&)> update =
                [&](const double& time)
        {
            m_scene->m_morph_time = time;
        };

        Tween* tween = m_scene->get_tween_collection()->add(
                start_object,
                stop_object,
                duration,
                0.0,
                EasingFunction::SmoothStep,
                update,
                complete
        );

        m_tweens.insert(tween);
    }
}