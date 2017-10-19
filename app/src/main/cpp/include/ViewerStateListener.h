#pragma once

namespace argeo
{
	class ViewerStateListener
	{
	public:
		virtual void on_start_render_loop() = 0;
		virtual void on_stop_render_loop() = 0;
	};
}
