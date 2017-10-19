#pragma once

#include "Context.h"
#include "PickListener.h"

#include <unordered_set>

namespace argeo
{
	class IPickable
	{
	public:
		bool add_pick_listener(PickListener* listener)
		{
			return m_listeners.insert(listener).second;
		}

		bool remove_pick_listener(PickListener* listener)
		{
			return m_listeners.erase(listener);
		}

		bool get_is_picked() const
		{
			return m_is_picked;
		}

		void set_is_picked(const bool& value)
		{
			m_is_picked = value;
		}

		void* get_owner()
		{
			return m_owner;
		}

		void set_owner(void* owner)
		{
			m_owner = owner;
		}

		virtual void on_picked()
		{
			m_is_picked = true;
			for (PickListener* listener : m_listeners)
			{
				listener->on_picked(this);
			}
		}

	protected:
		
		void* m_owner;
		bool  m_is_picked;
		std::unordered_set<PickListener*> m_listeners;
	};
}