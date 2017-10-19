#include "PolylineGraphics.h"

namespace argeo
{
	PolylineGraphics::PolylineGraphics(
		const std::vector<vec3d>& points,
		const bool& loop,
		const double& width)
		: m_points(points)
		, m_loop(loop)
		, m_show(true)
		, m_width(width)
	{ }

	PolylineGraphics::~PolylineGraphics()
	{ }

	std::vector<vec3d> PolylineGraphics::get_points() const
	{
		return m_points;
	}

	void PolylineGraphics::set_points(const std::vector<vec3d>& value)
	{
		if (m_points != value)
		{
			raise_on_property_changed(
				"points",
				&m_points,
				&value);
			m_points = value;
		}
	}

	void PolylineGraphics::add_point(const vec3d& value)
	{
		std::vector<vec3d> new_values = m_points;
		new_values.push_back(value);

		raise_on_property_changed(
			"points",
			&m_points,
			&new_values);
		m_points = new_values;
	}

	void PolylineGraphics::set_loop(const bool& value)
	{
		if (m_loop != value)
		{
			raise_on_property_changed(
				"loop",
				&m_loop,
				&value);
			m_loop = value;
		}
	}

	bool PolylineGraphics::get_loop() const
	{
		return m_loop;
	}

	void PolylineGraphics::set_width(const double& value)
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

	double PolylineGraphics::get_width() const
	{
		return m_width;
	}

	void PolylineGraphics::set_show(const bool& value)
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

	bool PolylineGraphics::get_show() const
	{
		return m_show;
	}
}