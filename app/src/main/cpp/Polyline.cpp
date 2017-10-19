#include "Polyline.h"

#include "PolylineCollection.h"

#include <algorithm>

namespace argeo
{
	unsigned int Polyline::show_index		    = 0;
	unsigned int Polyline::width_index		    = 1;
	unsigned int Polyline::position_index	    = 2;
	unsigned int Polyline::material_index	    = 3;
	unsigned int Polyline::position_size_index  = 4;
	unsigned int Polyline::number_of_properties = 5;

	Polyline::Polyline(
		const std::vector<vec3d>& points,
		const bool& loop,
		const double& width)
		: m_vertex_buffer_offset(0)
		, m_collection(nullptr)
		, m_pick_id(nullptr)
		, m_loop(loop)
		, m_width(width)
		, m_points(points)
		, m_show(true)
	{ 
		m_properties.resize(number_of_properties, false);

		m_outline_width = std::unique_ptr<UniformValue<float>>(
			new UniformValue<float>(0.0f, UniformType::Float));

		m_material = Material::from_name(
			Material::polyline_outline_material_name,
			UniformValues
		{
			{ "outlineWidth", m_outline_width.get() }
		});

		m_actual_points = points;
		if (m_loop && points.size() > 2)
		{
			m_actual_points.push_back(points[0]);
		}
		m_length = m_actual_points.size();
	}


	Polyline::~Polyline()
	{ }

	void Polyline::update()
	{
		bool positions_changed = m_properties[position_size_index] || m_properties[position_index];

		m_outline_width->set_data(
			get_is_picked() ? 6.0f : 0.0f);
		make_dirty(Polyline::material_index);
	}

	void Polyline::clean()
	{ 
		std::replace(
			m_properties.begin(),
			m_properties.end(),
			true,
			false);
	}

	void Polyline::make_dirty(const unsigned int& index)
	{
		if (!m_properties[index])
		{
			m_properties[index] = true;
			if (m_collection != nullptr)
			{
				m_collection->notify_dirty(
					this,
					index);
			}
		}
	}

	std::vector<bool> Polyline::get_properties() const
	{
		return m_properties;
	}

	PickId* Polyline::get_pick_id(Context* context)
	{
		if (m_pick_id == nullptr)
		{
			m_pick_id = context->create_pick_id(this);
		}
		return m_pick_id.get();
	}

	void Polyline::set_material(Material* value)
	{
		if (m_material != value)
		{
			m_material = value;
			make_dirty(Polyline::material_index);
		}
	}

	Material* Polyline::get_material()
	{
		return m_material;
	}

	void Polyline::set_vertex_buffer_offset(int value)
	{
		m_vertex_buffer_offset = value;
	}

	int Polyline::get_vertex_buffer_offset()
	{
		return m_vertex_buffer_offset;
	}

	void Polyline::add_locator_bucket(const LocatorBucket& locator_bucket)
	{
		m_locator_buckets.push_back(locator_bucket);
	}
	
	std::vector<LocatorBucket> Polyline::get_locator_buckets()
	{
		return m_locator_buckets;
	}

	std::vector<vec3d> Polyline::get_actual_points() const
	{
		return m_actual_points;
	}

	std::vector<vec3d> Polyline::get_points() const
	{
		return m_points;
	}

	void Polyline::set_points(const std::vector<vec3d>& value)
	{
		if (m_points != value)
		{
			std::vector<vec3d> points = value;

			if (m_loop && points.size() > 2)
			{
				points.push_back(points[0]);
			}

			if (m_actual_points.size() != points.size() || m_actual_points.size() != m_length)
			{
				make_dirty(Polyline::position_size_index);
			}

			m_points = value;
			m_actual_points = points;
			m_length = m_actual_points.size();

			make_dirty(Polyline::position_index);
			update();
		}
	}

	void Polyline::set_loop(bool value)
	{
		if (m_loop != value)
		{
			std::vector<vec3d> points = m_actual_points;
			
			if (value)
			{
				if (points.size() > 2 && points[0] == points[points.size() - 1])
				{
					if (points.size() == m_points.size())
					{
						m_actual_points = points = m_points;
					}
					points.push_back(points[0]);
				}
			}
			else
			{
				if (points.size() > 2 && points[0] == points[points.size() - 1])
				{
					if (points.size() - 1 == m_points.size())
					{
						m_actual_points = m_points;
					}
					else
					{
						points.resize(points.size() - 1);
					}
				}	
			}

			m_loop = value;
			make_dirty(Polyline::position_size_index);
		}
	}

	bool Polyline::get_loop() const
	{
		return m_loop;
	}

	void Polyline::set_show(bool value)
	{
		if (m_show != value)
		{
			m_show = value;
			make_dirty(Polyline::show_index);
		}
	}

	bool Polyline::get_show() const
	{
		return m_show;
	}

	void Polyline::set_width(double value)
	{
		if (m_width != value)
		{
			m_width = value;
			make_dirty(Polyline::width_index);
		}
	}

	double Polyline::get_width() const
	{
		return m_width;
	}

	void Polyline::set_group(PolylineCollection* value)
	{
		m_collection = value;
	}

	PolylineCollection* Polyline::get_group()
	{
		return m_collection;
	}

	unsigned int Polyline::get_length() const
	{
		return m_length;
	}
}