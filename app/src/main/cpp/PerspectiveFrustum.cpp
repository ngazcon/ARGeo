#include "PerspectiveFrustum.h"

#include <cmath>
#include <stdexcept>

namespace argeo
{
	PerspectiveFrustum::PerspectiveFrustum(
		const double& field_of_view_y,
		const double& aspect_ratio,    
		const double& near,
		const double& far,
		const double& x_offset,
		const double& y_offset)
		: m_field_of_view_y(field_of_view_y)
		, m_aspect_ratio(aspect_ratio)
		, m_near(near)
		, m_far(far)
		, m_x_offset(x_offset)
		, m_y_offset(y_offset)
		, m_dirty(true)
        , m_offcenter_frustum(new PerspectiveOffCenterFrustum())
	{ }


	PerspectiveFrustum::~PerspectiveFrustum()
	{ }

	void PerspectiveFrustum::set_near(const double& value)
	{
		if (m_near != value)
		{
            m_near  = value;
            m_dirty = true;
		}
	}

	double PerspectiveFrustum::get_near()
	{
        clean();
		return m_near;
	}

	void PerspectiveFrustum::set_far(const double& value)
	{
        if (m_far != value)
        {
            m_far = value;
            m_dirty = true;
        }
	}

	double PerspectiveFrustum::get_far()
	{
        clean();
		return m_far;
	}

	double PerspectiveFrustum::get_field_of_view_y()
	{
        clean();
		return m_field_of_view_y;
	}

	void PerspectiveFrustum::set_field_of_view_y(const double& value)
	{
        if (m_field_of_view_y != value)
        {
            m_field_of_view_y = value;
            m_dirty = true;
        }
	}

    void PerspectiveFrustum::set_field_of_view(const double& value)
    {
        set_field_of_view_y(::atan(::tan(value * 0.5) / m_aspect_ratio) * 2.0);
    }

	double PerspectiveFrustum::get_field_of_view()
	{
        clean();
		return (2.0 * std::atan(m_aspect_ratio * std::tan(m_field_of_view_y * 0.5)));
	}

	void PerspectiveFrustum::set_aspect_ratio(const double& value)
	{
        if (m_aspect_ratio != value)
        {
            m_aspect_ratio = value;
            m_dirty = true;
        }
	}

    void PerspectiveFrustum::set_y_offset(const double &value)
    {
        if (m_y_offset != value)
        {
            m_y_offset = value;
            m_dirty = true;
        }
    }

    double PerspectiveFrustum::get_y_offset()
    {
        return m_y_offset;
    }

	void PerspectiveFrustum::set_x_offset(const double &value)
	{
		if (m_x_offset != value)
		{
			m_x_offset = value;
			m_dirty = true;
		}
	}

	double PerspectiveFrustum::get_x_offset()
	{
		return m_x_offset;
	}

	double PerspectiveFrustum::get_aspect_ratio()
	{
		return m_aspect_ratio;
	}

	mat4d PerspectiveFrustum::get_projection()
	{
		clean();
		return m_offcenter_frustum->get_projection();
	}

    double PerspectiveFrustum::get_bottom()
    {
        clean();
        return m_offcenter_frustum->get_bottom();
    }

    void PerspectiveFrustum::set_bottom(const double& value)
    {
        return m_offcenter_frustum->set_bottom(value);
    }

    double PerspectiveFrustum::get_top()
    {
        clean();
        return m_offcenter_frustum->get_top();
    }

    void PerspectiveFrustum::set_top(const double& value)
    {
        return m_offcenter_frustum->set_top(value);
    }

    double PerspectiveFrustum::get_left()
    {
        clean();
        return m_offcenter_frustum->get_left();
    }

    void PerspectiveFrustum::set_left(const double& value)
    {
        return m_offcenter_frustum->set_left(value);
    }

    double PerspectiveFrustum::get_right()
    {
        clean();
        return m_offcenter_frustum->get_right();
    }

    void PerspectiveFrustum::set_right(const double& value)
    {
        return m_offcenter_frustum->set_right(value);
    }

	std::unique_ptr<CullingVolume> PerspectiveFrustum::get_culling_volume(
		const vec3d& position,
		const vec3d& direction,
		const vec3d& up)
	{
        clean();
		return m_offcenter_frustum->get_culling_volume(position, direction, up);
	}

	void PerspectiveFrustum::clean()
	{
		if (m_dirty)
		{
			if (m_field_of_view_y < 0 || m_field_of_view_y >= PI) 
			{
				throw std::invalid_argument("fov must be in the range [0, PI).");
			}

			if (m_aspect_ratio < 0) 
			{
				throw std::invalid_argument("aspect ratio must be positive.");
			}

			if (m_near < 0 || m_near > m_far) 
			{
				throw std::invalid_argument("near must be greater than zero and less than far.");
			}

			double top    =  m_near * tan(0.5 * m_field_of_view_y);
			double bottom = -top;
			double right  =  m_aspect_ratio * top;
			double left   = -right;
			double near   = m_near;
			double far    = m_far;

			right  += m_x_offset;
			left   += m_x_offset;
			top    += m_y_offset;
			bottom += m_y_offset;

			m_offcenter_frustum->set_bottom(bottom);
			m_offcenter_frustum->set_top(top);
			m_offcenter_frustum->set_far(far);
			m_offcenter_frustum->set_near(near);
			m_offcenter_frustum->set_left(left);
			m_offcenter_frustum->set_right(right);

			m_dirty = false;
		}
	}

	vec2d PerspectiveFrustum::get_pixel_dimension(
		const int& surface_width,
		const int& surface_height,
		const double& distance)
	{
        clean();
		return m_offcenter_frustum->get_pixel_dimension(surface_width, surface_height, distance);
	}

    PerspectiveOffCenterFrustum* PerspectiveFrustum::get_offcenter_frustum()
    {
        return m_offcenter_frustum.get();
    }

	Frustum* PerspectiveFrustum::clone(Frustum* result)
	{
		if (result == nullptr)
		{
			result = new PerspectiveFrustum();
		}

		PerspectiveFrustum* casted_result = static_cast<PerspectiveFrustum*>(result);
		casted_result->set_aspect_ratio(m_aspect_ratio);
		casted_result->set_far(m_far);
		casted_result->set_field_of_view_y(m_field_of_view_y);
		casted_result->set_near(m_near);

		return casted_result;
	}
}