#include "OrthographicFrustum.h"


namespace argeo
{
	OrthographicFrustum::OrthographicFrustum(
            const double& aspect_ratio,
            const double& width,
            const double& near,
            const double& far)
		: m_near(near)
		, m_far(far)
        , m_width(width)
        , m_aspect_ratio(aspect_ratio)
		, m_dirty(true)
        , m_offcenter_frustum(new OrthographicOffCenterFrustum())
	{ }

	OrthographicFrustum::~OrthographicFrustum()
	{ }

    void OrthographicFrustum::set_aspect_ratio(const double& value)
    {
        if (m_aspect_ratio != value)
        {
            m_aspect_ratio = value;
            m_dirty = true;
        }
    }

    double OrthographicFrustum::get_width()
    {
        clean();
        return m_width;
    }

    void OrthographicFrustum::set_width(const double& value)
    {
        if (m_width != value)
        {
            m_width = value;
            m_dirty = true;
        }
    }

	double OrthographicFrustum::get_depth()
	{
        clean();
		return std::abs(m_far - m_near);
	}

	void OrthographicFrustum::set_left(const double& value)
	{
		m_offcenter_frustum->set_left(value);
		m_dirty = true;
	}

	double OrthographicFrustum::get_left()
	{
		return m_offcenter_frustum->get_left();
	}

	void OrthographicFrustum::set_right(const double& value)
	{
		m_offcenter_frustum->set_right(value);
		m_dirty = true;
	}

	double OrthographicFrustum::get_right()
	{
        return m_offcenter_frustum->get_right();
	}

	void OrthographicFrustum::set_top(const double& value)
	{
        m_offcenter_frustum->set_top(value);
		m_dirty = true;
	}

	double OrthographicFrustum::get_top()
	{
        return m_offcenter_frustum->get_top();
	}

	void OrthographicFrustum::set_bottom(const double& value)
	{
        m_offcenter_frustum->set_bottom(value);
		m_dirty = true;
	}


	double OrthographicFrustum::get_bottom()
	{
        return m_offcenter_frustum->get_bottom();
	}

	void OrthographicFrustum::set_near(const double& value)
	{
		m_near  = value;
		m_dirty = true;
	}

	double OrthographicFrustum::get_near()
	{
		return m_near;
	}

	void OrthographicFrustum::set_far(const double& value)
	{
        if (m_far != value)
        {
            m_far = value;
            m_dirty = true;
        }
	}

	double OrthographicFrustum::get_far()
	{
		return m_far;
	}

	mat4d OrthographicFrustum::get_projection()
	{
		clean();
		return m_offcenter_frustum->get_projection();
	}

	double OrthographicFrustum::get_aspect_ratio()
	{
		return m_aspect_ratio;
	}

    double OrthographicFrustum::get_field_of_view_y()
    {
        clean();
        return m_offcenter_frustum->get_field_of_view_y();
    }

    void OrthographicFrustum::set_field_of_view_y(const double& value)
    {
        m_offcenter_frustum->set_field_of_view_y(value);
    }

    void OrthographicFrustum::set_field_of_view(const double& value)
    {
        set_field_of_view_y(::atan(::tan(value * 0.5) / m_aspect_ratio) * 2.0);
    }

    double OrthographicFrustum::get_field_of_view()
    {
        clean();
        return m_offcenter_frustum->get_field_of_view();
    }

	std::unique_ptr<CullingVolume> OrthographicFrustum::get_culling_volume(
		const vec3d& position,
		const vec3d& direction,
		const vec3d& up)
	{
		clean();
		return m_offcenter_frustum->get_culling_volume(position, direction, up);
	}

	void OrthographicFrustum::clean() 
	{
		if (m_dirty)
        {
            if (m_aspect_ratio < 0)
            {
                throw std::runtime_error("aspectRatio must be positive.");
            }

            if (m_near < 0 || m_near > m_far)
            {
                throw std::runtime_error("near must be greater than zero and less than far.");
            }

            double ratio = 1.0 / m_aspect_ratio;

            m_offcenter_frustum->set_right(m_width * 0.5);
            m_offcenter_frustum->set_left(-m_offcenter_frustum->get_right());
            m_offcenter_frustum->set_top(ratio * m_offcenter_frustum->get_right());
            m_offcenter_frustum->set_bottom(-m_offcenter_frustum->get_top());
            m_offcenter_frustum->set_near(m_near);
            m_offcenter_frustum->set_far(m_far);

            m_dirty = false;
        }
	}

	vec2d OrthographicFrustum::get_pixel_dimension(
		const int& surface_width,
		const int& surface_height,
		const double& distance)
	{
		clean();
		return m_offcenter_frustum->get_pixel_dimension(surface_width, surface_height, distance);
	}

    OrthographicOffCenterFrustum* OrthographicFrustum::get_offcenter_frustum()
    {
        return m_offcenter_frustum.get();
    }

	Frustum* OrthographicFrustum::clone(Frustum* result)
	{
		if (result == nullptr)
		{
			result = new OrthographicFrustum();
		}

		OrthographicFrustum* casted_result = static_cast<OrthographicFrustum*>(result);
		casted_result->set_far(m_far);
		casted_result->set_near(m_near);
        casted_result->set_aspect_ratio(m_aspect_ratio);
        casted_result->set_width(m_width);

		return casted_result;
	}
}