#include "BillboardVisualizer.h"

#include "BillboardGraphics.h"
#include "Device.h"

namespace argeo
{
	BillboardVisualizer::BillboardVisualizer(
		EntityCollection* collection,
		Scene* scene)
		: m_collection(collection)
		, m_scene(scene)
		, m_billboard_collection(nullptr)
	{
		m_collection->add_listener(this);
	}


	BillboardVisualizer::~BillboardVisualizer()
	{
		m_collection->remove_listener(this);
	}

	void BillboardVisualizer::update(const EngineClockTick& time)
	{
		for (const Entity* entity : m_entities_added)
		{
			std::string id = entity->get_id();
			if (m_items.find(id) == m_items.end())
			{
				if (m_billboard_collection == nullptr)
				{
					m_billboard_collection = std::unique_ptr<BillboardCollection>(new BillboardCollection(m_scene));
					m_scene->add_primitive(m_billboard_collection.get());
				}

				BillboardGraphics* graphics = 
					dynamic_cast<BillboardGraphics*>(entity->get_graphics());
				
				Billboard* item = nullptr;
				
				if (graphics->get_image() != nullptr)
				{
					item = m_billboard_collection->add(
						graphics->get_image(),
						graphics->get_image_id());
				}
				else
				{
					item = m_billboard_collection->add(
						graphics->get_image_path(),
						graphics->get_image_id());
				}

                item->set_height_clamping(graphics->get_height_clamping());
				item->set_position(entity->get_position().to_vec3d());
				item->set_pixel_offset(graphics->get_pixel_offset());
				item->set_width(graphics->get_width());
				item->set_height(graphics->get_height());

				m_items.emplace(
					id,
					new BillboardEntity(
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
				BillboardEntity* entry = m_items[id];

				Billboard* item		= entry->billboard;
				BillboardGraphics* graphics = dynamic_cast<BillboardGraphics*>(entity->get_graphics());

				m_billboard_collection->remove(item);
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
				BillboardEntity* entry = m_items[id];

				Billboard* item				= entry->billboard;
				BillboardGraphics* graphics = static_cast<BillboardGraphics*>(entity->get_graphics());

				item->set_height_clamping(graphics->get_height_clamping());
				item->set_position(entity->get_position().to_vec3d());
				item->set_pixel_offset(graphics->get_pixel_offset());
				item->set_width(graphics->get_width());
				item->set_height(graphics->get_height());
			}
		}
		m_entities_changed.clear();
	}

	void BillboardVisualizer::on_collection_changed(const EntityCollectionChangedEvent& event)
	{
		for (const Entity* entity : event.added)
		{
			if (dynamic_cast<BillboardGraphics*>(entity->get_graphics()) != nullptr)
			{
				m_entities_added.emplace(entity);
			}
		}

		for (const Entity* entity : event.removed)
		{
			if (dynamic_cast<BillboardGraphics*>(entity->get_graphics()) != nullptr)
			{
				m_entities_removed.emplace(entity);
			}
		}

		for (const Entity* entity : event.changed)
		{
			if (dynamic_cast<BillboardGraphics*>(entity->get_graphics()) != nullptr)
			{
				m_entities_changed.emplace(entity);
			}
		}
	}
}
