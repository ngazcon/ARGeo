#pragma once

#include "ICleanable.h"

namespace argeo
{
	class ICleanableObserver
	{
	public:

		virtual void notify_dirty(ICleanable* value) = 0;
	};
}
