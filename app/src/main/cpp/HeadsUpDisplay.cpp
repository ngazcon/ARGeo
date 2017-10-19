#include "HeadsUpDisplay.h"
#include "HeadsUpDisplayShaders.h"
#include "Device.h"

namespace argeo
{
	float HeadsUpDisplay::s_origin_scale[] = { 0.0f, 1.0f, -1.0f };

	HeadsUpDisplay::HeadsUpDisplay()
	{
		RenderStateParameters parameters;
		parameters.facet_culling = FacetCulling(false);
		parameters.depth_test    = DepthTest(false);
		parameters.blending      = Blending(
			true,
			SourceBlendingFactor::SourceAlpha,
			SourceBlendingFactor::SourceAlpha,
			DestinationBlendingFactor::OneMinusSourceAlpha,
			DestinationBlendingFactor::OneMinusSourceAlpha);

		RenderState* render_state = RenderState::from_cache(parameters);

		m_shader_program = Device::create_shader_program(
			heads_up_display_vs,
			heads_up_display_fs);

		m_color_uniform = (Uniform<vec3>*)m_shader_program->get_uniform("u_color");
		m_origin_scale_uniform = (Uniform<vec2>*)m_shader_program->get_uniform("u_originScale");
		m_center_position_uniform = (Uniform<vec3>*)m_shader_program->get_uniform("u_centerPosition");

		m_draw_command = std::unique_ptr<DrawCommand>(new DrawCommand(
			PrimitiveType::Triangles,
			nullptr,
			m_shader_program.get(), 
			render_state));

		m_color = ColorHelper::white_color();

		set_horizontal_origin(HorizontalOrigin::Left);
		set_vertical_origin(VerticalOrigin::Bottom);

		m_position_dirty = true;
	}


	HeadsUpDisplay::~HeadsUpDisplay()
	{ }

	void HeadsUpDisplay::create_vertex_array(Context* context)
	{
		// TODO:  Buffer hint.
		m_position_buffer = Device::create_vertex_buffer(
			BufferHint::StaticDraw,
			sizeof(vec2) * 4);

		VertexBufferAttribute* position_attribute = new VertexBufferAttribute(
			m_position_buffer.get(),
			ComponentDataType::Float,
			2);

		m_vertex_array = context->create_vertex_array();

		m_vertex_array->get_attributes()->set_attribute(
			position_attribute,
			(*m_shader_program->get_attributes())["position"]->get_location());

		m_draw_command->set_vertex_array(m_vertex_array.get());
	}

	void HeadsUpDisplay::dispose_vertex_array()
	{
		m_position_buffer.release();
		m_vertex_array.release();
	}


	void HeadsUpDisplay::update(Context* context)
	{
		if (m_position_dirty)
		{
			dispose_vertex_array();
			create_vertex_array(context);

			std::vector<vec2> positions =
			{ 
				vec2(-1.0, 1.0),
				vec2( 1.0, 1.0),
				vec2(-1.0,-1.0),
				vec2( 1.0,-1.0),
			};

			m_position_buffer->copy_from_system(positions);

			m_position_dirty = false;
		}
	}

	void HeadsUpDisplay::render(Context* context, const UniformState& uniform_state)
	{
		update(context);

		if (m_vertex_array.get() != nullptr)
		{
			//context->get_texture_units()->get_texture_unit(0)->set_texture(m_texture);
			//context->get_texture_units()->get_texture_unit(0)->set_texture_sampler(Device::get_texture_samplers()->get_linear_clamp());

			m_draw_command->execute(
				context,
				nullptr);
		}
	}

	Texture2D* HeadsUpDisplay::get_texture()
	{
		return m_texture;
	}

	void HeadsUpDisplay::set_texture(Texture2D* value)
	{
		m_texture = value;
	}

	Color HeadsUpDisplay::get_color()
	{
		return m_color; 
	}


	void HeadsUpDisplay::set_color(Color value)
	{
		m_color = value;
		m_color_uniform->set_value(
			vec3(m_color.red / 255.0f, m_color.green / 255.0f, m_color.blue / 255.0f));
	}

	HorizontalOrigin HeadsUpDisplay::get_horizontal_origin()
	{
		return m_horizontal_origin; 
	}

	void HeadsUpDisplay::set_horizontal_origin(HorizontalOrigin value)
	{
		m_horizontal_origin = value;

		m_origin_scale_uniform->set_value(vec2(
			s_origin_scale[(int)value],
			m_origin_scale_uniform->get_value().y));
	}

	VerticalOrigin HeadsUpDisplay::get_vertical_origin()
	{
		return m_vertical_origin; 
	}

	void HeadsUpDisplay::set_vertical_origin(VerticalOrigin value)
	{
		m_vertical_origin = value;

		m_origin_scale_uniform->set_value(vec2(
			m_origin_scale_uniform->get_value().x,
			s_origin_scale[(int)value]));
	}

	vec2 HeadsUpDisplay::get_position()
	{
		return m_position;
	}

	void HeadsUpDisplay::set_position(vec2 value)
	{
		if (m_position != value)
		{
			m_position = value;

			m_center_position_uniform->set_value(vec3(value, 0.0));

			m_position_dirty = true;
		}
	}
}
	