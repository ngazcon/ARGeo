#include "ViewportQuad.h"


namespace argeo
{
	ViewportQuad::ViewportQuad(
		const Rectangle& rectangle,
		Material*  material)
		: m_rectangle(rectangle)
		, m_material(material)
		, m_render_state(nullptr)
		, m_draw_command(nullptr)
	{ }


	ViewportQuad::~ViewportQuad()
	{ }

	void ViewportQuad::update(FrameState* frame_state)
	{
		if (m_render_state == nullptr || 
			m_render_state->get_viewport() != m_rectangle)
		{
			RenderStateParameters parameters;

			parameters.blending.enabled = true;
			parameters.viewport         = m_rectangle;
			m_render_state = RenderState::from_cache(parameters);
		}

		if (m_draw_command == nullptr)
		{
			Context* context = frame_state->get_context();
			m_draw_command = std::unique_ptr<DrawCommand>(context->create_viewport_quad_command(
				R"(
					in  vec2 fsTextureCoordinates;
					out vec4 fragmentColor;

					uniform sampler2D og_texture0;
					uniform float     og_alpha;

					void main()
					{
						fragmentColor   = texture(og_texture0, fsTextureCoordinates);

						if(fragmentColor.a == 0.0f)
						{
							discard;
						}

						fragmentColor.a = og_alpha;
					})",
				m_render_state,
				&m_material->get_uniform_map()));
			m_draw_command->set_pass(CommandPass::OverlayCommand);
		}
		
		frame_state->get_command_queue().push_back(m_draw_command.get());
	}

	void ViewportQuad::set_material(Material* value)
	{
		m_material = value;
	}

	Material* ViewportQuad::get_material()
	{
		return m_material;
	}

	void ViewportQuad::set_rectangle(Rectangle value)
	{
		m_rectangle = value;
	}

	Rectangle& ViewportQuad::get_rectangle()
	{
		return m_rectangle;
	}
}