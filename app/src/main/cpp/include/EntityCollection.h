#pragma once

#include "Entity.h"
#include "EntityCollectionChangedListener.h"
#include "IPropertyChangedListener.h"

#include <string>
#include <map>
#include <unordered_set>
#include <vector>

namespace argeo
{
	class EntityCollection
		: IPropertyChangedListener
	{
	public:
		EntityCollection();
		~EntityCollection();

		bool add(Entity* value);
		bool remove(Entity* value);
		Entity* get_by_id(const std::string& id);

		bool add_listener(EntityCollectionChangedListener* listener);
		bool remove_listener(EntityCollectionChangedListener* listener);

		void on_property_changed(const PropertyChangedEvent& event);

	private:
		void on_collection_changed(
			const std::vector<const Entity*>& added,
			const std::vector<const Entity*>& removed,
			const std::vector<const Entity*>& changed);

	private:
		std::unordered_set<EntityCollectionChangedListener*> m_listeners;
		std::map<std::string, Entity*> m_collection;
	};
}
