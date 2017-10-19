#pragma once

namespace argeo
{
	enum class ClientWaitResult
	{
		AlreadySignaled  = 0,
		Signaled		 = 1,
		TimeoutExpired	 = 2,
		Failed			 = 3,
	};
}

