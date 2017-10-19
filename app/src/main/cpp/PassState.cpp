#include "PassState.h"


namespace argeo
{
	PassState::PassState(
		Context*	 context,
		FrameBuffer* frame_buffer,
		Rectangle*   viewport,
		Blending *   blending,
		ScissorTest* scissor_test)
		: m_context(context)
		, m_viewport(viewport)
		, m_framebufer(frame_buffer)
		, m_scissor_test(scissor_test)
		, m_blending(blending)
	{ }

	PassState::~PassState()
	{ }

	Context* PassState::get_context() const
	{
		return m_context;
	}

	void PassState::set_context(Context* value)
	{
		m_context = value;
	}

	FrameBuffer* PassState::get_framebuffer() const
	{
		return m_framebufer;
	}

	void PassState::set_framebuffer(FrameBuffer* value)
	{
		m_framebufer = value;
	}

	Rectangle* PassState::get_viewport() const
	{
		return m_viewport;
	}

	void PassState::set_viewport(Rectangle* value)
	{
		m_viewport = value;
	}

	Blending* PassState::get_blending() const
	{
		return m_blending;
	}

	void PassState::set_blending(Blending* value)
	{
		m_blending = value;
	}

	ScissorTest* PassState::get_scissor_test() const
	{
		return m_scissor_test;
	}

	void PassState::set_scissor_test(ScissorTest* value)
	{
		m_scissor_test = value;
	}
}