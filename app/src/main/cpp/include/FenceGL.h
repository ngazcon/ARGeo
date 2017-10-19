#pragma once

#include "Fence.h"
#include "GL.h"

namespace argeo
{
	class FenceGL : public Fence
	{
	public:
		FenceGL();

		~FenceGL();

		void server_wait();

		ClientWaitResult client_wait();

		ClientWaitResult client_wait(const unsigned long long& timeout_in_nanoseconds);

		SynchronizationStatus status();


	private:
		GLsync m_handle;
	};
}
