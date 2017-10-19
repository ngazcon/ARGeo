#include "OrthographicOffCenterFrustum.h"


namespace argeo
{
    OrthographicOffCenterFrustum::OrthographicOffCenterFrustum(
            const double& left,
            const double& right,
            const double& bottom,
            const double& top,
            const double& near,
            const double& far)
            : m_near(near)
            , m_far(far)
            , m_left(left)
            , m_right(right)
            , m_bottom(bottom)
            , m_top(top)
            , m_dirty(true)
    { }


    OrthographicOffCenterFrustum::~OrthographicOffCenterFrustum()
    { }

    double OrthographicOffCenterFrustum::get_depth()
    {
        return std::abs(m_far - m_near);
    }

    void OrthographicOffCenterFrustum::set_left(const double& value)
    {
        if (m_left != value)
        {
            m_left = value;
            m_dirty = true;
        }
    }

    double OrthographicOffCenterFrustum::get_left()
    {
        return m_left;
    }

    void OrthographicOffCenterFrustum::set_right(const double& value)
    {
        if (m_right != value)
        {
            m_right = value;
            m_dirty = true;
        }
    }

    double OrthographicOffCenterFrustum::get_right()
    {
        return m_right;
    }

    void OrthographicOffCenterFrustum::set_top(const double& value)
    {
        if (m_top != value)
        {
            m_top = value;
            m_dirty = true;
        }
    }

    double OrthographicOffCenterFrustum::get_top()
    {
        return m_top;
    }

    void OrthographicOffCenterFrustum::set_bottom(const double& value)
    {
        if (m_bottom != value)
        {
            m_bottom = value;
            m_dirty = true;
        }
    }

    double OrthographicOffCenterFrustum::get_bottom()
    {
        return m_bottom;
    }

    void OrthographicOffCenterFrustum::set_near(const double& value)
    {
        if (m_near != value)
        {
            m_near  = value;
            m_dirty = true;
        }
    }

    double OrthographicOffCenterFrustum::get_near()
    {
        return m_near;
    }

    void OrthographicOffCenterFrustum::set_far(const double& value)
    {
        if (m_far != value)
        {
            m_far  = value;
            m_dirty = true;
        }
    }

    double OrthographicOffCenterFrustum::get_far()
    {
        return m_far;
    }

    mat4d OrthographicOffCenterFrustum::get_projection()
    {
        clean();
        return m_orthographic_matrix;
    }

    void OrthographicOffCenterFrustum::set_field_of_view_y(const double &value)
    {
        throw std::runtime_error("Cannot set the field of view in a orthographic offcenter frustum.");
    }

    double OrthographicOffCenterFrustum::get_field_of_view_y()
    {
        return 2.0 * std::atan(m_top / m_near);
    }

    void OrthographicOffCenterFrustum::set_field_of_view(const double &value)
    {
        throw std::runtime_error("Cannot set the field of view in a orthographic offcenter frustum.");
    }

    double OrthographicOffCenterFrustum::get_field_of_view()
    {
        return (2.0 * std::atan(get_aspect_ratio() * std::tan(get_field_of_view_y() * 0.5)));
    }

    double OrthographicOffCenterFrustum::get_aspect_ratio()
    {
        return (m_right - m_left) / (m_top - m_bottom);
    }

    std::unique_ptr<CullingVolume> OrthographicOffCenterFrustum::get_culling_volume(
            const vec3d& position,
            const vec3d& direction,
            const vec3d& up)
    {
        double t = m_top;
        double b = m_bottom;
        double r = m_right;
        double l = m_left;
        double n = m_near;
        double f = m_far;

        vec3d right = vec_cross(direction, up);
        vec3d near_center = (direction * n) + position;

        std::vector<vec4d> planes;
        vec3d point;

        //Left plane computation
        point = (right * l) + near_center;
        planes.push_back(vec4d(
                right,
                -vec_dot(right, point)));

        //Right plane computation
        point = (right * r) + near_center;
        planes.push_back(vec4d(
                -right,
                -vec_dot(-right, point)));

        //Bottom plane computation
        point = (up * b) + near_center;
        planes.push_back(vec4d(
                up,
                -vec_dot(up, point)));

        //Top plane computationS
        point = (up * t) + near_center;
        planes.push_back(vec4d(
                -up,
                -vec_dot(-up, point)));

        //Near plane computation
        planes.push_back(vec4d(
                direction,
                -vec_dot(direction, near_center)));

        //Far plane computation
        point = (direction * f) + position;
        planes.push_back(vec4d(
                -direction,
                -vec_dot(-direction, point)));

        return std::unique_ptr<CullingVolume>(new CullingVolume(planes));
    }

    void OrthographicOffCenterFrustum::clean()
    {
        if (m_dirty)
        {
            m_orthographic_matrix = mat_orthographic_off_center(
                    m_left,
                    m_right,
                    m_top,
                    m_bottom,
                    m_near,
                    m_far);

            m_dirty = false;
        }
    }

    vec2d OrthographicOffCenterFrustum::get_pixel_dimension(
            const int& surface_width,
            const int& surface_height,
            const double& distance)
    {
        clean();

        if (surface_width <= 0)
        {
            throw std::invalid_argument(
                    "surface_width must be greater than zero.");
        }

        if (surface_height <= 0)
        {
            throw std::invalid_argument(
                    "surface_height must be greater than zero.");
        }

        double frustum_width  = m_right - m_left;
        double frustum_height = m_top   - m_bottom;
        double pixel_width    = frustum_width / surface_width;
        double pixel_height   = frustum_height / surface_height;


        return vec2d(pixel_width, pixel_height);
    }

    Frustum* OrthographicOffCenterFrustum::clone(Frustum* result)
    {
        if (result == nullptr)
        {
            result = new OrthographicOffCenterFrustum();
        }

        OrthographicOffCenterFrustum* casted_result = static_cast<OrthographicOffCenterFrustum*>(result);
        casted_result->set_bottom(m_bottom);
        casted_result->set_far(m_far);
        casted_result->set_left(m_left);
        casted_result->set_near(m_near);
        casted_result->set_right(m_right);
        casted_result->set_top(m_top);

        return casted_result;
    }
}