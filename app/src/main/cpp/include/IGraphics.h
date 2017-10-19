#pragma once

#include "NotifyPropertyChanged.h"

namespace argeo
{
	class IGraphics
		: public NotifyPropertyChanged
	{
	public:
		virtual bool get_show() const = 0;
		virtual void set_show(const bool& value) = 0;
	};
}
