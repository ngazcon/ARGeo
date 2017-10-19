#pragma once

namespace argeo
{
	class Context;
	class PassState;

	class ICommand
	{
	public:
		virtual void execute(
			Context*   context, 
			PassState* pass_state = nullptr) = 0;
	};
}
