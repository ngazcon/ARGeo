#pragma once


#include "Ellipsoid.h"
#include "Camera.h"
#include "OrthographicFrustum.h"
#include "PerspectiveFrustum.h"
#include "OrthographicOffCenterFrustum.h"
#include "PerspectiveOffCenterFrustum.h"
#include "TweenCollection.h"

#include <memory>
#include <functional>

namespace argeo
{
    class Scene;

    struct ToCVCamera
    {
        ToCVCamera(
                const vec3d& position  = vec3d::zero(),
                const vec3d& direction = vec3d::zero(),
                const vec3d& up  = vec3d::zero(),
                const vec3d& position_2D  = vec3d::zero(),
                const vec3d& direction_2D = vec3d::zero(),
                const vec3d& up_2D  = vec3d::zero(),
                Frustum* frustum = nullptr)
                : position(position)
                , direction(direction)
                , up(up)
                , position_2D(position_2D)
                , direction_2D(direction_2D)
                , up_2D(up_2D)
                , frustum(frustum)
        { }

        vec3d position;
        vec3d direction;
        vec3d up;

        vec3d position_2D;
        vec3d direction_2D;
        vec3d up_2D;

        Frustum* frustum;
    };

    struct To2DCamera
    {
        To2DCamera(
                const vec3d& position  = vec3d::zero(),
                const vec3d& direction = vec3d::zero(),
                const vec3d& up  = vec3d::zero(),
                const vec3d& position_2D  = vec3d::zero(),
                const vec3d& direction_2D = vec3d::zero(),
                const vec3d& up_2D  = vec3d::zero(),
                OrthographicOffCenterFrustum* frustum = new OrthographicOffCenterFrustum())
                : position(position)
                , direction(direction)
                , up(up)
                , position_2D(position_2D)
                , direction_2D(direction_2D)
                , up_2D(up_2D)
                , frustum(frustum)
        { }

        vec3d position;
        vec3d direction;
        vec3d up;

        vec3d position_2D;
        vec3d direction_2D;
        vec3d up_2D;

        OrthographicOffCenterFrustum* frustum;
    };

    struct EndCamera
    {
        EndCamera(
                const vec3d& position  = vec3d::zero(),
                const vec3d& direction = vec3d::zero(),
                const vec3d& up  = vec3d::zero(),
                Frustum* frustum = nullptr)
                : position(position)
                , direction(direction)
                , up(up)
                , frustum(frustum)
        { }

        vec3d position;
        vec3d direction;
        vec3d up;
        Frustum* frustum;
    };

    class SceneTransitioner
    {
    public:
        SceneTransitioner(Scene* scene);

        ~SceneTransitioner();

        void complete_morph();

        void morph_to_2D(
                const double& duration,
                const Ellipsoid& ellipsoid = Ellipsoid::wgs84());

        void morph_to_3D(
                const double& duration,
                const Ellipsoid& ellipsoid = Ellipsoid::wgs84());


        void morph_to_CV(
                const double& duration,
                const Ellipsoid& ellipsoid = Ellipsoid::wgs84());

        void morph_from_CV_to_3D(
                const double& duration,
                const EndCamera& end_camera,
                std::function<void()> complete);

        void morph_from_2D_to_CV(
                const double& duration,
                const ToCVCamera& end_camera,
                std::function<void()> complete);

        void morph_from_3D_to_CV(
                const double& duration,
                const ToCVCamera& end_camera,
                std::function<void()> complete);

        void morph_from_CV_to_2D(
                const double& duration);

        void morph_from_2D_to_3D(
                const double& duration,
                const Ellipsoid& ellipsoid = Ellipsoid::wgs84());


        void morph_from_3D_to_2D(
                const double& duration,
                const Ellipsoid& ellipsoid = Ellipsoid::wgs84());

        void morph_perspective_to_orthographic(
                const double& duration,
                const EndCamera& end_camera,
                const std::function<void(Camera*, const double&)>& update_height,
                const std::function<void()>& complete);

        void morph_orthographic_to_perspective(
                const double& duration,
                const EndCamera& end_camera,
                const  std::function<void()>& complete);

        vec3d columbus_view_morph(
                const vec3d& start_position,
                const vec3d& end_position,
                const double& time);

        void destroy_morph_handler();

        void add_morph_time_animations(
                const double& start,
                const double& stop,
                const double& duration,
                std::function<void()> complete);

        EndCamera& get_CV_to_3D_camera(const Ellipsoid& ellipsoid);

        void create_morph_handler(std::function<void()>& complete);

        std::function<void()> complete_3D_callback(const EndCamera&  camera_3D);
        std::function<void()> complete_2D_callback(const EndCamera&  camera_2D);
        std::function<void()> complete_CV_callback(const ToCVCamera& camera_CV);

    private:
        Scene* m_scene;
        SceneMode m_previous_mode;

        ToCVCamera m_scratch_to_CV_camera;
        To2DCamera m_scratch_to_2D_camera;
        EndCamera  m_scratch_end_camera;

        std::unique_ptr<OrthographicFrustum> m_scratch_orthographic_frustum;
        std::unique_ptr<OrthographicOffCenterFrustum> m_scratch_orthographic_off_center_frustum;
        std::unique_ptr<PerspectiveFrustum>  m_scratch_perspective_frustum;

        bool m_morph_to_orthographic;
        bool m_morph_canceled;

        std::function<void()> m_complete_morph;

        std::unordered_set<Tween*> m_tweens;

    };
}

