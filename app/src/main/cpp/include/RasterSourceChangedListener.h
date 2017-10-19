#pragma once

#include "RasterSource.h"

namespace argeo
{
	class RasterSourceChangedListener
	{
	public:
		virtual void on_raster_source_changed(RasterSource* value) = 0;
	};
}
