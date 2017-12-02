#include "PlaneGraphics.h"

namespace argeo
{
	PlaneGraphics::PlaneGraphics(
		vec3d origin,
		vec3d x_axis,
		vec3d y_axis,
		vec3d normal,
		bool flag)
		: m_origin(origin)
		, m_x_axis(x_axis)
		, m_y_axis(y_axis)
		, m_normal(normal)
		, m_has_outline(true)
		, m_allow_picking(true)
		, m_outline_stroke(1.1)
		, m_show(true)
		, m_width(1.0)
	    , m_height(1.0)
		, m_flag(flag)
	{  }

	PlaneGraphics::PlaneGraphics(EllipsoidTangentPlane tangent_plane, bool flag)
		: PlaneGraphics(
			tangent_plane.get_origin().to_vec3d(),
			tangent_plane.get_x_axis().to_vec3d(),
			tangent_plane.get_y_axis().to_vec3d(),
			tangent_plane.get_normal().to_vec3d(),
			flag)
	{ }


	PlaneGraphics::~PlaneGraphics()
	{ }

	double PlaneGraphics::get_outline_stroke() const
	{
		return m_outline_stroke;
	}

	bool PlaneGraphics::get_flag() const
	{
		return m_flag;
	}

	void PlaneGraphics::set_outline_stroke(const double& value)
	{
		if (m_outline_stroke != value)
		{
			raise_on_property_changed(
				"m_outline_stroke",
				&m_outline_stroke,
				&value);
			m_allow_picking = value;
		}
	}

	bool PlaneGraphics::get_allow_picking() const
	{
		return m_allow_picking;
	}

	void PlaneGraphics::set_allow_picking(bool value)
	{
		if (m_allow_picking != value)
		{
			raise_on_property_changed(
				"allow_picking",
				&m_allow_picking,
				&value);
			m_allow_picking = value;
		}
	}

	void PlaneGraphics::set_show(const bool& value)
	{
		if (m_show != value)
		{
			raise_on_property_changed(
				"show",
				&m_show,
				&value);
			m_show = value;
		}
	}

	bool PlaneGraphics::get_show() const
	{
		return m_show;
	}

	void PlaneGraphics::set_width(double value)
	{
		if (m_width != value)
		{
			raise_on_property_changed(
				"width",
				&m_width,
				&value);
			m_width = value;
		}
	}

	double PlaneGraphics::get_width() const
	{
		return m_width;
	}


    void PlaneGraphics::set_height(const double& value)
    {
        if (m_height != value)
        {
            raise_on_property_changed(
                    "height",
                    &m_height,
                    &value);
            m_height = value;
        }
    }

    double PlaneGraphics::get_height() const
    {
        return m_height;
    }

	bool PlaneGraphics::get_depth_test_enabled()
	{
		return m_depth_test;
	}

	void PlaneGraphics::set_depth_test_enabled(bool value)
	{
		if (m_depth_test != value)
		{
			raise_on_property_changed(
				"depth_test",
				&m_depth_test,
				&value);
			m_depth_test = value;
		}
	}

	bool PlaneGraphics::get_depth_write()
	{
		return m_depth_write;
	}

	void PlaneGraphics::set_depth_write(bool value)
	{
		if (m_depth_write != value)
		{
			raise_on_property_changed(
				"depth_write",
				&m_depth_write,
				&value);
			m_depth_write = value;
		}
	}

	vec3d PlaneGraphics::get_origin() const
	{
		return m_origin;
	}

	vec3d PlaneGraphics::get_x_axis() const
	{
		return m_x_axis;
	}

	vec3d PlaneGraphics::get_y_axis() const
	{
		return m_y_axis;
	}

	vec3d PlaneGraphics::get_normal() const
	{
		return m_normal;
	}
}