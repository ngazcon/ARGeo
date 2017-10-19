#include "PerspectiveOffCenterFrustum.h"

#include <stdexcept>

namespace argeo
{
	PerspectiveOffCenterFrustum::PerspectiveOffCenterFrustum(
		const double& left,
		const double& right,
		const double& bottom,
		const double& top,
		const double& near,
		const double& far)
		: m_left(left)
		, m_right(right)
		, m_bottom(bottom)
		, m_top(top)
		, m_near(near)
		, m_far(far)
		, m_dirty(true)
	{ }

	PerspectiveOffCenterFrustum::~PerspectiveOffCenterFrustum()
	{ }

	
	void PerspectiveOffCenterFrustum::set_near(const double& value)
	{
		if (m_near != value)
        {
            m_near = value;
            m_dirty = true;
        }
	}

	double PerspectiveOffCenterFrustum::get_near()
	{
		return m_near;
	}

	void PerspectiveOffCenterFrustum::set_far(const double& value)
	{
        if (m_far != value)
        {
            m_far = value;
            m_dirty = true;
        }
	}

	double PerspectiveOffCenterFrustum::get_far()
	{
		return m_far;
	}

	void PerspectiveOffCenterFrustum::set_left(const double& value)
	{
        if (m_left != value)
        {
            m_left = value;
            m_dirty = true;
        }
	}

	double PerspectiveOffCenterFrustum::get_left()
	{
		return m_left;
	}

	void PerspectiveOffCenterFrustum::set_right(const double& value)
	{
        if (m_right != value)
        {
            m_right = value;
            m_dirty = true;
        }
	}

	double PerspectiveOffCenterFrustum::get_right()
	{
		return m_right;
	}

	void PerspectiveOffCenterFrustum::set_top(const double& value)
	{
        if (m_top != value)
        {
            m_top = value;
            m_dirty = true;
        }
	}

	double PerspectiveOffCenterFrustum::get_top()
	{
		return m_top;
	}

	void PerspectiveOffCenterFrustum::set_bottom(const double& value)
	{
        if (m_bottom != value)
        {
            m_bottom = value;
            m_dirty = true;
        }
	}

	double PerspectiveOffCenterFrustum::get_bottom()
	{
		return m_bottom;
	}

	mat4d PerspectiveOffCenterFrustum::get_projection()
	{
		clean();
		return m_perspective_matrix;
	}


	double PerspectiveOffCenterFrustum::get_aspect_ratio()
	{
		return (m_right - m_left) / (m_top - m_bottom);
	}

    void PerspectiveOffCenterFrustum::set_field_of_view_y(const double &value)
    {
        throw std::runtime_error("Cannot set the field of view in a perspective offcenter frustum.");
    }

	double PerspectiveOffCenterFrustum::get_field_of_view_y()
	{
		return 2.0 * std::atan(m_top / m_near);
	}

    void PerspectiveOffCenterFrustum::set_field_of_view(const double &value)
    {
        throw std::runtime_error("Cannot set the field of view in a perspective offcenter frustum.");
    }

    double PerspectiveOffCenterFrustum::get_field_of_view()
    {
        return (2.0 * std::atan(get_aspect_ratio() * std::tan(get_field_of_view_y() * 0.5)));
    }

	std::unique_ptr<CullingVolume> PerspectiveOffCenterFrustum::get_culling_volume(
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

		vec3d right       = vec_cross(direction, up);
		vec3d near_center = (direction * n);
		vec3d far_center  = (direction * f);

		std::vector<vec4d> planes;
		vec3d normal;
		
		//Left plane computation
		normal  = (right * l) + near_center;
		normal  = vec_normalize(normal);
		normal  = vec_cross(normal, up);

		planes.push_back(vec4d(
			 normal,
			-vec_dot(normal, position)));

		//Right plane computation
		normal  = (right * r) + near_center;
		normal  = vec_normalize(normal);
		normal  = vec_cross(up, normal);

		planes.push_back(vec4d(
			 normal,
			-vec_dot(normal, position)));

		//Bottom plane computation
		normal  = (up * b) + near_center;
		normal  = vec_normalize(normal);
		normal  = vec_cross(right, normal);

		planes.push_back(vec4d(
			normal,
			-vec_dot(normal, position)));

		//Top plane computation
		normal  = (up * t) + near_center;
		normal  = vec_normalize(normal);
		normal  = vec_cross(normal, right);

		planes.push_back(vec4d(
			 normal,
			-vec_dot(normal, position)));

		//Near plane computation
		planes.push_back(vec4d(
			 direction,
			-vec_dot(direction, near_center + position)));

		//Far plane computation
		normal = -direction;
		planes.push_back(vec4d(
			 normal,
			-vec_dot(normal, far_center + position)));

		return std::unique_ptr<CullingVolume>(new CullingVolume(planes));
	}

	void PerspectiveOffCenterFrustum::clean()
	{
		if (m_dirty) 
		{
			if (m_near <= 0 || m_near > m_far)
			{
				throw std::invalid_argument(
					"near must be greater than zero and less than far.");
			}

			m_perspective_matrix = mat_perspective_off_center(
				m_left,
				m_right,
				m_bottom,
				m_top,
				m_near,
				m_far);

			m_dirty = false;
		}
	}

	vec2d PerspectiveOffCenterFrustum::get_pixel_dimension(
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

		double inverse_near = 1.0 / m_near;
		double tan_theta    = m_top * inverse_near;
		double pixel_height = 2.0 * distance * tan_theta / surface_height;
		tan_theta = m_right * inverse_near;
		double pixel_width  = 2.0 * distance * tan_theta / surface_width;

		return vec2d(pixel_width, pixel_height);
	}

	Frustum* PerspectiveOffCenterFrustum::clone(Frustum* result)
	{
		if (result == nullptr)
		{
			result = new PerspectiveOffCenterFrustum();
		}

		PerspectiveOffCenterFrustum* casted_result = static_cast<PerspectiveOffCenterFrustum*>(result);
		casted_result->set_bottom(m_bottom);
		casted_result->set_far(m_far);
		casted_result->set_left(m_left);
		casted_result->set_near(m_near);
		casted_result->set_right(m_right);
		casted_result->set_top(m_top);

		return casted_result;
	}
}