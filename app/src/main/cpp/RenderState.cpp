#include "RenderState.h"
#include "TypeConverterGL.h"

#include "GL.h"

namespace argeo
{
	unsigned int RenderState::g_next_id = 0;
	std::map<std::string, CachedRenderState*> RenderState::g_cached_states;

	RenderState::RenderState(const RenderStateParameters& params)
		: m_id(g_next_id++)
		, m_params(params)
	{ 
		validate();
	}

	RenderState::~RenderState()
	{ }

	PrimitiveRestart RenderState::get_primitive_restart() const
	{
		return m_params.primitive_restart;
	}

	PolygonOffset RenderState::get_polygon_offset() const
	{
		return m_params.polygon_offset;
	}

	FacetCulling RenderState::get_facet_culling() const
	{
		return m_params.facet_culling;
	}

	ProgramPointSize RenderState::get_program_point_size() const
	{
		return m_params.program_point_size;
	}

	RasterizationMode RenderState::get_rasterization_mode() const
	{
		return m_params.rasterization_mode;
	}

	ScissorTest RenderState::get_scissor_test() const
	{
		return m_params.scissor_test;
	}

	StencilTest RenderState::get_stencil_test() const
	{
		return m_params.stencil_test;
	}

	DepthTest RenderState::get_depth_test() const
	{
		return m_params.depth_test;
	}

	DepthRange RenderState::get_depth_range() const
	{
		return m_params.depth_range;
	}

	Blending RenderState::get_blending() const
	{
		return m_params.blending;
	}

	ColorMask RenderState::get_color_mask() const
	{
		return m_params.color_mask;
	}

	Rectangle RenderState::get_viewport() const
	{
		return m_params.viewport;
	}

	bool RenderState::get_depth_mask() const
	{
		return m_params.depth_mask;
	}

	unsigned int RenderState::get_stencil_mask() const
	{
		return m_params.stencil_mask;
	}

	unsigned int RenderState::get_id() const
	{
		return m_id;
	}

	RenderStateParameters RenderState::get_params() const
	{
		return m_params;
	}

	void RenderState::apply(PassState* pass_state)
	{
		apply_primitive_restart();
		apply_facet_culling();
		apply_polygon_offset();
		apply_program_point_size();
		apply_rasterization_mode();
		apply_stencil_mask();
		apply_stencil_test();
		apply_depth_test();
		apply_depth_range();
		apply_color_mask();
		apply_depth_mask();

		apply_viewport(pass_state);
		apply_scissor_test(pass_state);
		apply_blending(pass_state);
	}

	void RenderState::apply_viewport(PassState* pass_state)
	{
		Context* context = pass_state->get_context();

		const Rectangle* viewport = pass_state->get_viewport() != nullptr ?
			pass_state->get_viewport() :
			&m_params.viewport;

		// TODO: fix this!!
		// If the viewport is all zeros then set the size acording the inner surface
		bool created = false;
		if(viewport->get_left()   == 0 && viewport->get_right() == 0 &&
		   viewport->get_height() == 0 && viewport->get_width() == 0)
		{
			viewport = new Rectangle(
				0,
				0, 
				context->get_drawing_surface_width(),
				context->get_drawing_surface_height());
			created = true;
		}

		glViewport(
			viewport->get_left(),
			viewport->get_bottom(),
			viewport->get_width(),
			viewport->get_height());

		context->get_uniform_state()->set_viewport(*viewport);

		if (created)
		{
			delete viewport;
		}
	}

	void RenderState::apply_scissor_test(PassState* pass_state)
	{
		const ScissorTest* scissor_test = &m_params.scissor_test;
		bool enabled				    = pass_state->get_scissor_test() != nullptr ? 
			pass_state->get_scissor_test()->enabled :
			scissor_test->enabled;

		enable(EnableCap::ScissorTest, enabled);

		if (enabled)
		{
			Rectangle rectangle = pass_state->get_scissor_test() != nullptr ?
				pass_state->get_scissor_test()->rectangle :
				scissor_test->rectangle;

			glScissor(
				rectangle.get_left(),
				rectangle.get_bottom(),
				rectangle.get_width(),
				rectangle.get_height());
		}
	}

	void RenderState::apply_blending(PassState* pass_state)
	{
		const Blending* blending = &m_params.blending;
		bool enabled			 = pass_state->get_blending() != nullptr ? 
			pass_state->get_blending()->enabled :
			blending->enabled;
		
		enable(EnableCap::Blend, enabled);

		if (enabled)
		{
			glBlendFuncSeparate(
				TypeConverterGL::ToEnum(blending->source_rgb_factor),
				TypeConverterGL::ToEnum(blending->destination_rgb_factor),
				TypeConverterGL::ToEnum(blending->source_alpha_factor),
				TypeConverterGL::ToEnum(blending->destination_alpha_factor));

			glBlendEquationSeparate(
				TypeConverterGL::ToEnum(blending->rgb_equation),
				TypeConverterGL::ToEnum(blending->alpha_equation));

			glBlendColor(
				blending->color.red / 255.0,
				blending->color.green / 255.0,
				blending->color.blue / 255.0,
				blending->color.alpha / 255.0);
		}
	}

	void RenderState::apply_polygon_offset()
	{
		enable(EnableCap::PolygonOffsetFill, m_params.polygon_offset.enabled);
		if (m_params.facet_culling.enabled)
		{
			glPolygonOffset(
				m_params.polygon_offset.factor,
				m_params.polygon_offset.units);
		}
	}

	void RenderState::apply_facet_culling()
	{
		enable(EnableCap::CullFace, m_params.facet_culling.enabled);
		if (m_params.facet_culling.enabled)
		{
			glCullFace(TypeConverterGL::ToEnum(m_params.facet_culling.face));
			glFrontFace(TypeConverterGL::ToEnum(m_params.facet_culling.front_face_winding_order));
		}
	}

	void RenderState::apply_primitive_restart()
	{
#ifdef WIN32
		enable(EnableCap::PrimitiveRestart, m_primitive_restart.enabled);
		if (m_primitive_restart.enabled)
		{
			glPrimitiveRestartIndex(m_primitive_restart.index);
		}
#endif
	}

	void RenderState::apply_program_point_size()
	{
#ifdef WIN32
		enable(EnableCap::ProgramPointSize, m_program_point_size == ProgramPointSize::Enabled);
#endif
	}

	void RenderState::apply_rasterization_mode()
	{
#ifdef WIN32
		glPolygonMode(GL_FRONT_AND_BACK, TypeConverterGL::ToEnum(m_rasterization_mode));
#endif
	}

	void RenderState::apply_stencil_test()
	{
		enable(EnableCap::StencilTest, m_params.stencil_test.enabled);
		if (m_params.stencil_test.enabled)
		{
			apply_stencil(StencilFace::Front, m_params.stencil_test.front_face);
			apply_stencil(StencilFace::Back, m_params.stencil_test.back_face);
		}
	}

	void RenderState::apply_stencil_mask()
	{
		glStencilMask(m_params.stencil_mask);
	}

	void RenderState::apply_stencil(
		const StencilFace& face,
		const StencilTestFace& test)
	{
		glStencilOpSeparate(
			TypeConverterGL::ToEnum(face),
			TypeConverterGL::ToEnum(test.stencil_fail_operation),
			TypeConverterGL::ToEnum(test.depth_fail_stencil_pass_operation),
			TypeConverterGL::ToEnum(test.depth_pass_stencil_pass_operation));

		glStencilFuncSeparate(
			TypeConverterGL::ToEnum(face),
			TypeConverterGL::ToEnum(test.function),
			test.reference_value,
			test.mask);
	}

	void RenderState::apply_depth_test()
	{
		enable(EnableCap::DepthTest, m_params.depth_test.enabled);
		if (m_params.depth_test.enabled)
		{
			glDepthFunc(TypeConverterGL::ToEnum(m_params.depth_test.function));
		}
	}

	void RenderState::apply_depth_range()
	{
		glDepthRangef(m_params.depth_range.near_value, m_params.depth_range.far_value);
	}

	void RenderState::apply_color_mask()
	{
		glColorMask(
			m_params.color_mask.red,
			m_params.color_mask.green,
			m_params.color_mask.blue,
			m_params.color_mask.alpha);
	}

	void RenderState::apply_depth_mask()
	{
		glDepthMask(m_params.depth_mask);
	}

	void RenderState::enable(const EnableCap& enable_cap, bool enable)
	{
		if (enable)
		{
			glEnable(TypeConverterGL::ToEnum(enable_cap));
		}
		else
		{
			glDisable(TypeConverterGL::ToEnum(enable_cap));
		}
	}

	std::vector<RenderState::TransitionMethod> RenderState::create_transition_methods(RenderState* previous_state)
	{
		std::vector<TransitionMethod> methods;

		if (previous_state->get_depth_mask() != m_params.depth_mask)
		{
			methods.push_back(&RenderState::apply_depth_mask);
		}

		if (previous_state->get_stencil_mask() != m_params.stencil_mask)
		{
			methods.push_back(&RenderState::apply_stencil_mask);
		}

		if (previous_state->get_color_mask() != m_params.color_mask)
		{
			methods.push_back(&RenderState::apply_color_mask);
		}

		if ((previous_state->get_depth_range().near_value != m_params.depth_range.near_value) ||
			(previous_state->get_depth_range().far_value  != m_params.depth_range.far_value))
		{
			methods.push_back(&RenderState::apply_depth_range);
		}

		if ((previous_state->get_polygon_offset().enabled != m_params.polygon_offset.enabled) ||
			(previous_state->get_polygon_offset().factor  != m_params.polygon_offset.factor) ||
			(previous_state->get_polygon_offset().units   != m_params.polygon_offset.units)) 
		{
			methods.push_back(&RenderState::apply_polygon_offset);
		}

		if (previous_state->get_depth_test().enabled  != m_params.depth_test.enabled ||
			previous_state->get_depth_test().function != m_params.depth_test.function)
		{
			methods.push_back(&RenderState::apply_depth_test);
		}

		if (previous_state->get_stencil_test().enabled     != m_params.stencil_test.enabled ||
			
			// Back face
			previous_state->get_stencil_test().back_face.stencil_fail_operation			   != m_params.stencil_test.back_face.stencil_fail_operation ||
			previous_state->get_stencil_test().back_face.depth_fail_stencil_pass_operation != m_params.stencil_test.back_face.depth_fail_stencil_pass_operation ||
			previous_state->get_stencil_test().back_face.depth_pass_stencil_pass_operation != m_params.stencil_test.back_face.depth_pass_stencil_pass_operation ||
			previous_state->get_stencil_test().back_face.function						   != m_params.stencil_test.back_face.function ||
			previous_state->get_stencil_test().back_face.reference_value				   != m_params.stencil_test.back_face.reference_value ||
			previous_state->get_stencil_test().back_face.mask							   != m_params.stencil_test.back_face.mask ||
			
			// Front face
			previous_state->get_stencil_test().front_face.stencil_fail_operation			!= m_params.stencil_test.front_face.stencil_fail_operation ||
			previous_state->get_stencil_test().front_face.depth_fail_stencil_pass_operation != m_params.stencil_test.front_face.depth_fail_stencil_pass_operation ||
			previous_state->get_stencil_test().front_face.depth_pass_stencil_pass_operation != m_params.stencil_test.front_face.depth_pass_stencil_pass_operation ||
			previous_state->get_stencil_test().front_face.function						    != m_params.stencil_test.front_face.function ||
			previous_state->get_stencil_test().front_face.reference_value				    != m_params.stencil_test.front_face.reference_value ||
			previous_state->get_stencil_test().front_face.mask							    != m_params.stencil_test.front_face.mask)
		{
			methods.push_back(&RenderState::apply_stencil_test);
		}

		if (previous_state->get_rasterization_mode() != m_params.rasterization_mode)
		{
			methods.push_back(&RenderState::apply_rasterization_mode);
		}

		if (previous_state->get_program_point_size() != m_params.program_point_size)
		{
			methods.push_back(&RenderState::apply_program_point_size);
		}

		if (previous_state->get_facet_culling().enabled				     != m_params.facet_culling.enabled ||
			previous_state->get_facet_culling().face					 != m_params.facet_culling.face ||
			previous_state->get_facet_culling().front_face_winding_order != m_params.facet_culling.front_face_winding_order)
		{
			methods.push_back(&RenderState::apply_facet_culling);
		}

		if (previous_state->get_primitive_restart().enabled != m_params.primitive_restart.enabled ||
			previous_state->get_primitive_restart().index   != m_params.primitive_restart.index)
		{
			methods.push_back(&RenderState::apply_primitive_restart);
		}

		return methods;
	}

	void RenderState::apply_transition(
		RenderState* previous_render_state,
		RenderState* render_state,
		PassState*   previous_pass_state,
		PassState*   pass_state,
		bool clear)
	{
		if (previous_render_state != render_state)
		{
			// When a new render state is applied, instead of making OpenGL calls for all the states or first
			// comparing the states one-by-one with the previous state (basically a linear search), we take
			// advantage of RenderState's immutability, and store a dynamically populated sparse data structure
			// containing functions that make the minimum number of OpenGL calls when transitioning from one state
			// to the other.  In practice, this works well since state-to-state transitions generally only require a
			// few OpenGL calls, especially if commands are stored by state.

			std::vector<TransitionMethod> transition_methods;
			if (render_state->m_transition_methods.find(previous_render_state->get_id()) ==
				render_state->m_transition_methods.end())
			{
				transition_methods = render_state->create_transition_methods(previous_render_state);
				render_state->m_transition_methods[previous_render_state->get_id()] = transition_methods;
			}
			else
			{
				transition_methods = render_state->m_transition_methods[previous_render_state->get_id()];
			}

			for (TransitionMethod method : transition_methods)
			{
				(render_state->*method)();
			}
		}

		const ScissorTest previous_scissor_test = previous_pass_state->get_scissor_test() != nullptr ?
			*previous_pass_state->get_scissor_test() :
			previous_render_state->get_scissor_test();

		const ScissorTest scissor_test = pass_state->get_scissor_test() != nullptr ?
			*pass_state->get_scissor_test() :
			render_state->get_scissor_test();

		// Our scissor rectangle can get out of sync with the GL scissor rectangle on clears.
		// Seems to be a problem only on ANGLE. See https://github.com/AnalyticalGraphicsInc/cesium/issues/2994
		if (previous_scissor_test.enabled   != scissor_test.enabled ||
			previous_scissor_test.rectangle != scissor_test.rectangle ||
			clear)
		{
			render_state->apply_scissor_test(pass_state);
		}

		bool previous_blending_enabled = previous_pass_state->get_blending() != nullptr ?
			previous_pass_state->get_blending()->enabled :
			previous_render_state->get_blending().enabled;

		bool blending_enabled = pass_state->get_blending() != nullptr ?
			pass_state->get_blending()->enabled :
			render_state->get_blending().enabled;

		if ((previous_blending_enabled != blending_enabled) ||
			(blending_enabled && (previous_render_state->get_blending() != render_state->get_blending())))
		{
			render_state->apply_blending(pass_state);
		}

		// Pointer comparation
		if (previous_render_state			   != render_state ||
			previous_pass_state				   != pass_state   ||
			previous_pass_state->get_context() != pass_state->get_context())
		{
			render_state->apply_viewport(pass_state);
		}
	}

	RenderState* RenderState::from_cache(RenderStateParameters parameters)
	{
		std::string str_state = parameters.to_string();

		if (g_cached_states.find(str_state) != g_cached_states.end())
		{
			CachedRenderState* cached_state = g_cached_states[str_state];
			cached_state->reference_count++;
			return cached_state->state;
		}
		// Cache miss. 
		// Fully define render state and try again.
		else
		{	
			RenderState* new_render_state = new RenderState(parameters);

			CachedRenderState* cached_state = new CachedRenderState(new_render_state);
			g_cached_states[str_state]      = cached_state;
			return cached_state->state;
		}
	}

	void RenderState::remove_from_cache(RenderStateParameters parameters)
	{ }

	void RenderState::validate()
	{
		if ((m_params.scissor_test.rectangle.get_width() < 0) ||
			(m_params.scissor_test.rectangle.get_height() < 0))
		{
			throw std::invalid_argument(
				"renderState.scissorTest.rectangle.width and renderState.scissorTest.rectangle.height must be greater than or equal to zero.");
		}

		if (m_params.depth_range.near_value > m_params.depth_range.near_value)
		{
			throw std::invalid_argument(
				"renderState.depthRange.near can not be greater than renderState.depthRange.far.");
		}

		if (m_params.depth_range.near_value < 0.0 || m_params.depth_range.near_value > 1.0)
		{
			throw std::invalid_argument(
				"DepthRange Near must be between zero and one.");
		}

		if (m_params.depth_range.far_value < 0.0 || m_params.depth_range.far_value > 1.0)
		{
			throw std::invalid_argument(
				"DepthRange Far must be between zero and one.");
		}
	}


}