#pragma once

#include "IPropertyChangedListener.h"

#include <unordered_set>

namespace argeo
{
	class NotifyPropertyChanged
	{
	public:

		inline bool add_property_changed_listener(IPropertyChangedListener* listener)
		{
			return m_listeners.emplace(listener).second;
		}

		inline bool remove_property_changed_listener(IPropertyChangedListener* listener)
		{
			return m_listeners.erase(listener);
		}

		inline void raise_on_property_changed(
			const std::string& name, 
			const void* old_value,
			const void* new_value)
		{
			PropertyChangedEvent event(
				this,
				name,
				old_value,
				new_value);

			for (IPropertyChangedListener* listener : m_listeners)
			{
				listener->on_property_changed(event);
			}
		}

	private:
		std::unordered_set<IPropertyChangedListener*> m_listeners;
	};
}