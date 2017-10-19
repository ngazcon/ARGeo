#include "ClearCommand.h"

namespace argeo
{
		ClearCommand::ClearCommand(
			RenderState* render_state, 
			ClearBuffers buffers,
			Color		 color,	  
			float		 depth,	  
			int          stencil,
			FrameBuffer* frame_buffer)
			: render_state(render_state)
			, buffers(buffers)
			, color(color)
			, depth(depth)
			, stencil(stencil)
			, frame_buffer(frame_buffer)
		{ }

		ClearCommand::~ClearCommand()
		{ }

		void ClearCommand::execute(
			Context*   context,
			PassState* pass_state)
		{
			context->clear(
				this,
				pass_state);
		}
}
