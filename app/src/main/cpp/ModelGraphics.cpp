#include "ModelGraphics.h"

namespace argeo
{
	ModelGraphics::ModelGraphics(
		const std::string& path,
		const HeightClamping& height_clamping,
		const OrientationClamping& orientation_clamping)
		: m_path(path)
		, m_material(nullptr)
		, m_show(true)
		, m_height_clamping(height_clamping)
		, m_orientation_clamping(orientation_clamping)
	{ }


	ModelGraphics::~ModelGraphics()
	{ }

	std::string ModelGraphics::get_path() const
	{
		return m_path;
	}

	Material* ModelGraphics::get_material()
	{
		return m_material;
	}

	void ModelGraphics::set_material(Material* value)
	{
		m_material = value;
	}

	bool ModelGraphics::get_show() const
	{
		return m_show;
	}

	void ModelGraphics::set_show(const bool& value)
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

	void ModelGraphics::set_height_clamping(const HeightClamping& value)
	{
		if (m_height_clamping != value)
		{
			raise_on_property_changed(
				"height_clamping",
				&m_height_clamping,
				&value);
			m_height_clamping = value;
		}
	}

	HeightClamping ModelGraphics::get_height_clamping() const
	{
		return m_height_clamping;
	}

	void ModelGraphics::set_orientation_clamping(const OrientationClamping& value)
	{
		if (m_orientation_clamping != value)
		{
			raise_on_property_changed(
				"orientation_clamping",
				&m_orientation_clamping,
				&value);
			m_orientation_clamping = value;
		}
	}

	OrientationClamping ModelGraphics::get_orientation_clamping() const
	{
		return m_orientation_clamping;
	}
}