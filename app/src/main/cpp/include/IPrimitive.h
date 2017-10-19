#pragma once

#include "FrameState.h"
#include "IPickable.h"

namespace argeo
{
	class IPrimitive : public IPickable
	{
	public:
		virtual void update(FrameState* frame_state) = 0;

	};
}
