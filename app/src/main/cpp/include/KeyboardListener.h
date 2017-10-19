#pragma once

#include "KeyboardKeyEventArgs.h"

namespace argeo
{
	class KeyboardListener
	{
	public:

		virtual bool on_key_event(const KeyboardKeyEventArgs& args)
		{ 
			return true;
		};
	};
}