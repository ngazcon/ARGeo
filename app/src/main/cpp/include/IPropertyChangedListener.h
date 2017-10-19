#pragma once

#include "PropertyChangedEvent.h"

namespace argeo
{
	class IPropertyChangedListener
	{
	public:
		virtual void on_property_changed(const PropertyChangedEvent& event) = 0;
	};
}
