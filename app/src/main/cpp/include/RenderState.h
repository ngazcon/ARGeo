#pragma once

#include "PrimitiveRestart.h"
#include "FacetCulling.h"
#include "ScissorTest.h"
#include "StencilTest.h"
#include "DepthTest.h"
#include "DepthRange.h"
#include "Blending.h"
#include "ColorMask.h"
#include "ProgramPointSize.h"
#include "RasterizationMode.h"
#include "PassState.h"
#include "EnableCap.h"
#include "StencilFace.h"
#include "PolygonOffset.h"

#include <iomanip>
#include <sstream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <map>

namespace argeo
{
	class RenderState;

	struct RenderStateParameters
	{
		RenderStateParameters()
			: program_point_size(ProgramPointSize::Disabled)
			, rasterization_mode(RasterizationMode::Fill)
			, depth_mask(true)
			, stencil_mask(0xFF)
		{ }

		PolygonOffset	  polygon_offset;
		ProgramPointSize  program_point_size;
		RasterizationMode rasterization_mode;
		PrimitiveRestart  primitive_restart;
		FacetCulling	  facet_culling;
		StencilTest       stencil_test;
		DepthTest		  depth_test;
		DepthRange		  depth_range;
		ColorMask		  color_mask;
		Rectangle		  viewport;
		ScissorTest		  scissor_test;
		Blending		  blending;
		bool			  depth_mask;
		unsigned int      stencil_mask;

		std::string to_string() const
		{
			// Compute individual hash values for two data members and combine them using XOR and bit shifting

			std::stringstream sbuf;

			sbuf <<
				// Facet culling
				facet_culling.enabled <<
				static_cast<typename std::underlying_type<CullFace>::type>(facet_culling.face) <<
				static_cast<typename std::underlying_type<WindingOrder>::type>(facet_culling.front_face_winding_order);

			sbuf <<
				// Depth range
				std::setfill('0') << std::setw(2) << depth_range.far_value <<
				std::setfill('0') << std::setw(2) << depth_range.near_value;

			sbuf <<
				// Blending
				blending.enabled <<
				static_cast<typename std::underlying_type<BlendEquation>::type>(blending.alpha_equation) <<
				static_cast<typename std::underlying_type<BlendEquation>::type>(blending.rgb_equation) <<
				std::setfill('0') << std::setw(2) << static_cast<typename std::underlying_type<DestinationBlendingFactor>::type>(blending.destination_alpha_factor) <<
				std::setfill('0') << std::setw(2) << static_cast<typename std::underlying_type<DestinationBlendingFactor>::type>(blending.destination_rgb_factor) <<
				std::setfill('0') << std::setw(2) << static_cast<typename std::underlying_type<SourceBlendingFactor>::type>(blending.source_alpha_factor) <<
				std::setfill('0') << std::setw(2) << static_cast<typename std::underlying_type<SourceBlendingFactor>::type>(blending.source_rgb_factor);

			sbuf <<
				// Viewport
				std::setfill('0') << std::setw(4) << viewport.get_bottom() <<
				std::setfill('0') << std::setw(4) << viewport.get_top() <<
				std::setfill('0') << std::setw(4) << viewport.get_left() <<
				std::setfill('0') << std::setw(4) << viewport.get_right();

			sbuf <<
				primitive_restart.enabled <<
				std::setfill('0') << std::setw(5) << primitive_restart.index;

			sbuf <<
				// Depth test
				depth_test.enabled <<
				static_cast<typename std::underlying_type<DepthTestFunction>::type>(depth_test.function);

			sbuf <<
				stencil_test.enabled <<
				static_cast<typename std::underlying_type<StencilOperation>::type>(stencil_test.back_face.stencil_fail_operation) <<
				static_cast<typename std::underlying_type<StencilOperation>::type>(stencil_test.back_face.depth_fail_stencil_pass_operation) <<
				static_cast<typename std::underlying_type<StencilOperation>::type>(stencil_test.back_face.depth_pass_stencil_pass_operation) <<
				static_cast<typename std::underlying_type<StencilTestFunction>::type>(stencil_test.back_face.function) <<
				stencil_test.back_face.reference_value <<
				stencil_test.back_face.mask <<

				static_cast<typename std::underlying_type<StencilOperation>::type>(stencil_test.front_face.stencil_fail_operation) <<
				static_cast<typename std::underlying_type<StencilOperation>::type>(stencil_test.front_face.depth_fail_stencil_pass_operation) <<
				static_cast<typename std::underlying_type<StencilOperation>::type>(stencil_test.front_face.depth_pass_stencil_pass_operation) <<
				static_cast<typename std::underlying_type<StencilTestFunction>::type>(stencil_test.front_face.function) <<
				stencil_test.front_face.reference_value <<
				stencil_test.front_face.mask;

			sbuf <<
				polygon_offset.enabled <<
				polygon_offset.factor <<
				polygon_offset.units;

			sbuf <<
				// Depth mask
				depth_mask <<
				stencil_mask <<

				// Color mask
				color_mask.alpha <<
				color_mask.red <<
				color_mask.green <<
				color_mask.blue;

			return sbuf.str();
		}

		bool operator==(const RenderStateParameters &other) const
		{
			return strcmp(
				other.to_string().c_str(),
				to_string().c_str()) == 0;
		}
	};

	struct CachedRenderState
	{
		CachedRenderState(RenderState* state)
			: state(state)
			, reference_count(1)
		{ }

		RenderState* state;
		unsigned int reference_count;		
	};

	class RenderState
	{
	public:

		RenderState(const RenderStateParameters& params);
		~RenderState();

		ProgramPointSize  get_program_point_size() const;
		RasterizationMode get_rasterization_mode() const;
		PolygonOffset     get_polygon_offset() const;
		FacetCulling      get_facet_culling() const;
		DepthTest		  get_depth_test() const;
		DepthRange		  get_depth_range() const;
		ScissorTest	      get_scissor_test() const;
		StencilTest	      get_stencil_test() const;
		PrimitiveRestart  get_primitive_restart() const;
		Blending		  get_blending() const;
		ColorMask		  get_color_mask() const;
		Rectangle		  get_viewport() const;
		RenderStateParameters get_params() const;

		unsigned int get_stencil_mask() const;
		bool         get_depth_mask() const;
		unsigned int get_id() const;

		void apply(PassState* pass_state);
		void validate();
	
		static RenderState* from_cache(RenderStateParameters parameters = RenderStateParameters());
		static void remove_from_cache(RenderStateParameters parameters);

		static void apply_transition(
			RenderState* previous_render_state,
			RenderState* render_state,
			PassState*   previous_pass_state,
			PassState*   pass_state,
			bool clear);
	private:

		typedef void(RenderState::*TransitionMethod)(void);
		std::vector<TransitionMethod> create_transition_methods(RenderState* previous_state);
		std::map<unsigned int, std::vector<TransitionMethod>> m_transition_methods;

		void apply_viewport(PassState* pass_state);
		void apply_blending(PassState* pass_state);
		void apply_scissor_test(PassState* pass_state);
		
		void apply_stencil_mask();
		void apply_primitive_restart();
		void apply_facet_culling();
		void apply_program_point_size();
		void apply_rasterization_mode();
		void apply_stencil_test();
		void apply_depth_test();
		void apply_depth_range();
		void apply_color_mask();
		void apply_depth_mask();
		void apply_polygon_offset();
		
		static void enable(
			const EnableCap& enable_cap,
			bool enable);

		static void apply_stencil(
			const StencilFace& face,
			const StencilTestFace& test);

	private:
		
		static std::map<std::string, CachedRenderState*> g_cached_states;
		static unsigned int g_next_id;

		const unsigned int m_id;
		const RenderStateParameters m_params;
	};

}

