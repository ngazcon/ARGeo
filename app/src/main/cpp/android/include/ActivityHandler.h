#pragma once

#include "ActivityStatus.h"

namespace argeo
{
	class ActivityHandler
	{
	public:
		virtual status on_activate() = 0;
		virtual void on_deactivate() = 0;
		virtual status on_step() = 0;

		virtual void on_start() { };
		virtual void on_resume() { };
		virtual void on_pause() { };
		virtual void on_stop() { };
		virtual void on_destroy() { };

		virtual void on_save_instace_state(void** data, std::size_t* resource_size) { };
		virtual void on_configuration_changed() { };
		virtual void on_low_memory() { };

		virtual void on_create_window() { };
		virtual void on_destroy_window() { };
		virtual void on_gain_focus() { };
		virtual void on_lost_focus() { };

	};
}


