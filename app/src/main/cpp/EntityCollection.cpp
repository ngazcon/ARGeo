#include "EntityCollection.h"
#include "Viewer.h"

namespace argeo
{
	EntityCollection::EntityCollection()
	{ }

	EntityCollection::~EntityCollection()
	{ }

	bool EntityCollection::add(Entity* value)
	{
		bool success = m_collection.emplace(
			value->get_id(), 
			value).second;

		if (success)
		{
			value->set_collection(this);
			value->add_property_changed_listener(this);

			on_collection_changed(
				std::vector<const Entity*> { value },
				std::vector<const Entity*>(),
				std::vector<const Entity*>());
		}

		return success;
	}

	bool EntityCollection::remove(Entity* value)
	{
		bool success = m_collection.erase(value->get_id());

		if (success)
		{
			value->remove_property_changed_listener(this);
			on_collection_changed(
				std::vector<const Entity*>(),
				std::vector<const Entity*> { value },
				std::vector<const Entity*>());
		}

		return success;
	}

	void EntityCollection::on_property_changed(const PropertyChangedEvent& event)
	{
		const Entity* entity = static_cast<const Entity*>(event.source);
		if (m_collection.find(entity->get_id()) != m_collection.end())
		{
			on_collection_changed(
				std::vector<const Entity*>(),
				std::vector<const Entity*>(),
				std::vector<const Entity*>{ entity });
		}
	}

	bool EntityCollection::add_listener(EntityCollectionChangedListener* listener)
	{
		return m_listeners.emplace(listener).second;
	}

	bool EntityCollection::remove_listener(EntityCollectionChangedListener* listener)
	{
		return m_listeners.erase(listener);
	}

	Entity* EntityCollection::get_by_id(const std::string& id)
	{
		if (m_collection.find(id) != m_collection.end())
		{
			return m_collection.at(id);
		}
		return nullptr;
	}

	void EntityCollection::on_collection_changed(
		const std::vector<const Entity*>& added,
		const std::vector<const Entity*>& removed,
		const std::vector<const Entity*>& changed)
	{
		EntityCollectionChangedEvent event(
			this,
			added,
			removed,
			changed);
		for (EntityCollectionChangedListener* listener : m_listeners)
		{
			listener->on_collection_changed(event);
		}
	}
}