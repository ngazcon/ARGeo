#include "FenceGL.h"

#include <stdexcept>

#include "GL.h"

namespace argeo
{
	FenceGL::FenceGL()
	{
		m_handle = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
	}

	FenceGL::~FenceGL()
	{
		glDeleteSync(m_handle);
	}

	void FenceGL::server_wait()
	{
		glWaitSync(m_handle, 0, GL_TIMEOUT_EXPIRED);
	}

	ClientWaitResult FenceGL::client_wait()
	{
		return client_wait(GL_TIMEOUT_IGNORED);
	}


	ClientWaitResult FenceGL::client_wait(const unsigned long long& timeout_in_nanoseconds)
	{
		GLenum result = glClientWaitSync(m_handle, GL_SYNC_FLUSH_COMMANDS_BIT, timeout_in_nanoseconds);

		switch (result)
		{
		case GL_ALREADY_SIGNALED:
			return ClientWaitResult::AlreadySignaled;
		case GL_CONDITION_SATISFIED:
			return ClientWaitResult::Signaled;
		case GL_TIMEOUT_EXPIRED:
			return ClientWaitResult::TimeoutExpired;
		}

		return ClientWaitResult::Failed; 
	}

	SynchronizationStatus FenceGL::status()
	{
		int length;
		int status;

		glGetSynciv(m_handle, GL_SYNC_STATUS, 1, &length, &status);

		if (status == GL_UNSIGNALED)
		{
			return SynchronizationStatus::Unsignaled;
		}
		else
		{
			return SynchronizationStatus::Signaled;
		}
	}
}