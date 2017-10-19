#include "PolylineVisualizer.h"

#include "PolylineGraphics.h"
#include "Device.h"

namespace argeo
{
	PolylineVisualizer::PolylineVisualizer(
		EntityCollection* collection,
		Scene* scene)
		: m_collection(collection)
		, m_scene(scene)
		, m_polyline_collection(nullptr)
	{
		m_collection->add_listener(this);
	}


	PolylineVisualizer::~PolylineVisualizer()
	{
		m_collection->remove_listener(this);
	}

	void PolylineVisualizer::update(const EngineClockTick& time)
	{
		for (const Entity* entity : m_entities_added)
		{
			std::string id = entity->get_id();
			if (m_items.find(id) == m_items.end())
			{
				if (m_polyline_collection == nullptr)
				{
					m_polyline_collection = std::unique_ptr<PolylineCollection>(new PolylineCollection());
					m_scene->add_primitive(m_polyline_collection.get());
				}

				PolylineGraphics* graphics =
					dynamic_cast<PolylineGraphics*>(entity->get_graphics());

				Polyline* item = m_polyline_collection->add(
					graphics->get_points(),
					graphics->get_loop(),
					graphics->get_width());

				item->set_show(graphics->get_show());

				m_items.emplace(
					id,
					new PolylineEntity(
						entity,
						item));
			}
		}
		m_entities_added.clear();

		for (const Entity* entity : m_entities_removed)
		{
			std::string id = entity->get_id();
			if (m_items.find(id) != m_items.end())
			{
				PolylineEntity* entry = m_items[id];

				Polyline* item = entry->polyline;
				PolylineGraphics* graphics = dynamic_cast<PolylineGraphics*>(entity->get_graphics());

				m_polyline_collection->remove(item);
				m_items.erase(id);

				delete entry;
			}
		}
		m_entities_removed.clear();

		for (const Entity* entity : m_entities_changed)
		{
			std::string id = entity->get_id();
			if (m_items.find(id) != m_items.end())
			{
				PolylineEntity* entry = m_items[id];

				Polyline* item = entry->polyline;
				PolylineGraphics* graphics = static_cast<PolylineGraphics*>(entity->get_graphics());

				item->set_points(graphics->get_points());
				item->set_loop(graphics->get_loop());
				item->set_show(graphics->get_show());
				item->set_width(graphics->get_width());
			}
		}
		m_entities_changed.clear();
	}

	void PolylineVisualizer::on_collection_changed(const EntityCollectionChangedEvent& event)
	{
		for (const Entity* entity : event.added)
		{
			if (dynamic_cast<PolylineGraphics*>(entity->get_graphics()) != nullptr)
			{
				m_entities_added.emplace(entity);
			}
		}

		for (const Entity* entity : event.removed)
		{
			if (dynamic_cast<PolylineGraphics*>(entity->get_graphics()) != nullptr)
			{
				m_entities_removed.emplace(entity);
			}
		}

		for (const Entity* entity : event.changed)
		{
			if (dynamic_cast<PolylineGraphics*>(entity->get_graphics()) != nullptr)
			{
				m_entities_changed.emplace(entity);
			}
		}
	}
}
