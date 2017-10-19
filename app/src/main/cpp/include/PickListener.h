#pragma once

namespace argeo
{
	class IPickable;
	class PickListener
	{
	public:
		virtual void on_picked(IPickable* picked) = 0;
	};
}
