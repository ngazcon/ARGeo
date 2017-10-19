#pragma once

#include "KeyboardKey.h"
#include "KeyboardAction.h"

namespace argeo
{
	struct KeyboardKeyEventArgs
	{
		KeyboardKeyEventArgs(
			const KeyboardKey& key,
			const KeyboardAction& action)
			: key(key),
			  action(action)
		{ }

		const KeyboardKey key;
		const KeyboardAction action;
	};
}