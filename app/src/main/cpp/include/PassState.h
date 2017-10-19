#pragma once

#include "Rectangle.h"
#include "FrameBuffer.h"
#include "Blending.h"
#include "ScissorTest.h"
#include "Context.h"

#include <memory>

namespace argeo
{
	
	class PassState
	{
		///
		/// The state for a particular rendering pass.  This is used to supplement the state
		/// in a command being executed.
		///

	public:

		PassState(
			Context* context,
			FrameBuffer* frame_buffer = nullptr,
			Rectangle*   viewport     = nullptr,
			Blending*    blending	  = nullptr,
			ScissorTest* scissor_test = nullptr);

		~PassState();

		///
		/// Get the context used to execute commands for this pass.
		/// @return {Context*} The context of this pass.
		///
		Context* get_context() const;

		///
		///
		///
		void set_context(Context* value);

		///
		/// The framebuffer to render to.  This framebuffer is used unless a {@link DrawCommand}
		/// or {@link ClearCommand} explicitly define a framebuffer, which is used for off-screen
		/// rendering.
		///
		/// @type {Framebuffer}
		/// @default nullptr
		///
		FrameBuffer* get_framebuffer() const;


		void set_framebuffer(FrameBuffer* value);


		///
		/// The viewport used when one is not defined by a {@link DrawCommand}'s render state.
		/// @type {Rectangle}
		/// @default nullptr
		///
		Rectangle* get_viewport() const;


		void set_viewport(Rectangle* value);

		/**
		* When defined, this overrides the blending property of a {@link DrawCommand}'s render state.
		* This is used to, for example, to allow the renderer to turn off blending during the picking pass.
		* When this is <code>undefined</code>, the {@link DrawCommand}'s property is used.
		* @type {Blending}
		* @default nullptr
		*/
		Blending* get_blending() const;
		void set_blending(Blending* value);

		/**
		* When defined, this overrides the scissor test property of a {@link DrawCommand}'s render state.
		* This is used to, for example, to allow the renderer to scissor out the pick region during the picking pass.
		* <p>
		* When this is <code>undefined</code>, the {@link DrawCommand}'s property is used.
		* </p>
		*
		* @type {ScissorTest}
		* @default undefined
		*/
		ScissorTest* get_scissor_test() const;
		void set_scissor_test(ScissorTest* value);

	private:

		Rectangle*    m_viewport;
		FrameBuffer*  m_framebufer;
		Blending*     m_blending;
		ScissorTest*  m_scissor_test;
		Context*      m_context;
	};
}