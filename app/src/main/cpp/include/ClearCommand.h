#pragma once

#include "Color.h"
#include "ClearBuffers.h"
#include "RenderState.h"
#include "ICommand.h"
#include "PassState.h"
#include "Context.h"


namespace argeo
{
	class ClearCommand : public ICommand
	{
	public:

		ClearCommand(
			RenderState* render_state = nullptr,
			ClearBuffers buffers	  = ClearBuffers::ColorAndDepthBuffer,
			Color		 color		  = Color(255, 255, 255, 255),
			float		 depth		  = 1,
			int          stencil	  = 0,
			FrameBuffer* frame_buffer = nullptr);

		~ClearCommand();

		void execute(
			Context*   context,
			PassState* pass_state = nullptr);

		RenderState* render_state;
		FrameBuffer* frame_buffer;

		ClearBuffers buffers;

		Color color;
		float depth;
		int   stencil;
	};
}
