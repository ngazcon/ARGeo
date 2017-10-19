#pragma once

#include "Context.h"
#include "Surface.h"

#include <unordered_set>

namespace argeo
{
	struct ResizeEventArgs
	{
		ResizeEventArgs(
			const unsigned int& width, 
			const unsigned int& height)
			: width(width)
			, height(height)
		{ }

		const unsigned int width;
		const unsigned int height;
	};

	struct CloseEventArgs
	{ };

	struct RenderFrameEventArgs
	{ };

	class SurfaceStateListener
	{
		public:
			virtual void on_close(CloseEventArgs args)
			{ };

			virtual void on_resize(ResizeEventArgs args)
			{ };

			virtual void on_render_frame(RenderFrameEventArgs args)
			{ };
	};

	class SurfaceWindow : public Surface
	{
	public:
		
		bool add_surface_state_listener(SurfaceStateListener* listener)
		{
			return m_listeners.insert(listener).second;
		};

		bool remove_surface_state_listener(SurfaceStateListener* listener)
		{
			return m_listeners.erase(listener) > 0;
		};

		virtual void on_close()
		{
			for (auto listener : m_listeners)
			{
				listener->on_close(CloseEventArgs());
			}
		}

		virtual void on_resize()
		{
			for (auto listener : m_listeners)
			{
				listener->on_resize(ResizeEventArgs(
					get_width(),
					get_height()));
			}
		}

		virtual void on_render_frame()
		{
			for (auto listener : m_listeners)
			{
				listener->on_render_frame(RenderFrameEventArgs());
			}
		}

		virtual void update() = 0;
		virtual void stop() = 0;

		virtual Context* get_context() = 0;
		virtual unsigned int get_width() const = 0;
		virtual unsigned int get_height() const = 0;

	protected:
		std::unordered_set<SurfaceStateListener*> m_listeners;
	};
}
