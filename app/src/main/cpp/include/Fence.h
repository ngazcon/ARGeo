#pragma once

#include "ClientWaitResult.h"
#include "SynchronizationStatus.h"

namespace argeo
{
	class Fence
	{
		///
		/// Represent a fence object for GPU context synchronization.
		///

	public:

		///
		/// Prevent the driver from adding any commands to the GPUs command queue until this sync object is signaled.
		/// This function does not wait for this to happen.
		///
		virtual void server_wait() = 0;

		///
		/// Block all CPU operations until a sync object is signaled.
		/// @result {ClientWaitResult} The result of waiting.
		///
		virtual ClientWaitResult client_wait() = 0;

		///
		/// Block all CPU operations until a sync object is signaled.
		/// @param {unsigned long long} The maximum waiting time.
		/// @result {ClientWaitResult} The result of waiting.
		///
		virtual ClientWaitResult client_wait(const unsigned long long& timeout_in_nanoseconds) = 0;

		///
		/// Get the current status of the synchronization.
		/// @result {SynchronizationStatus} The current sync status.
		///
		virtual SynchronizationStatus status() = 0;
	};
}
