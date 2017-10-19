#pragma once

#include "IPrimitive.h"
#include "Texture2D.h"
#include "Rectangle.h"

namespace argeo
{
	class IVideoCapturePreview : public IPrimitive
	{
	public:
		virtual bool is_running() const = 0;

		virtual void   set_current_aspect_ratio(const double& value) = 0;
		virtual double get_current_aspect_ratio() const = 0;
		virtual void   set_current_vertical_view_angle(const double& value) = 0;
		virtual double get_current_vertical_view_angle() const = 0;

		virtual void set_orientation(float value)    = 0;

		virtual void set_viewport_size(Rectangle value) = 0;
		virtual Rectangle get_viewport_size() const = 0;

		virtual void set_current_preview_size(Rectangle value) = 0;
		virtual Rectangle get_current_preview_size() const = 0;

		virtual void update(FrameState* frame_state) = 0;
	};

}
