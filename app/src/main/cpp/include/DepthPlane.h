#pragma once

#include "Vector.h"
#include "Ellipsoid.h"
#include "FrameState.h"
#include "DrawCommand.h"
#include "RenderState.h"
#include "Context.h"
#include "VertexArray.h"

#include <string>
#include <memory>
#include <vector>

namespace argeo
{
	class DepthPlane
	{
	public:
		DepthPlane();
		~DepthPlane();

		void update(FrameState* frame_state);
		void execute(
			Context* context,
			PassState* pass_state);

	private:
		std::vector<vec3> compute_depth_quad(
			const Ellipsoid& ellipsoid,
			FrameState* frame_state);

	private:

		std::unique_ptr<DrawCommand> m_draw_command;
		std::unique_ptr<VertexArray> m_vertex_array;

		ShaderProgram* m_shader_program;
		RenderState*   m_render_state;
	};
}
