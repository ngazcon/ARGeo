#include "PlaneVisualizer.h"

#include "PlaneGraphics.h"

namespace argeo
{
	PlaneVisualizer::PlaneVisualizer(
		EntityCollection* collection,
		Scene* scene)
		: m_collection(collection)
		, m_scene(scene)
	{
		m_collection->add_listener(this);
	}


	PlaneVisualizer::~PlaneVisualizer()
	{
		m_collection->remove_listener(this);
	}

	void PlaneVisualizer::update(const EngineClockTick& time)
	{
		for (const Entity* entity : m_entities_added)
		{
			std::string id = entity->get_id();
			if (m_items.find(id) == m_items.end())
			{
				PlaneGraphics* graphics = static_cast<PlaneGraphics*>(entity->get_graphics());

				PlanePrimitive* item = new PlanePrimitive(
						m_scene,
					graphics->get_origin(),
					graphics->get_x_axis(),
					graphics->get_y_axis(),
					graphics->get_normal(),
					graphics->get_flag());

				item->set_translation(entity->get_position().to_vec3d());
				item->set_rotation(entity->get_orientation());
				item->set_width(graphics->get_width());
				item->set_show(graphics->get_show());
                item->set_height(graphics->get_height());

				m_items.emplace(
					id,
					new PlaneEntity(
						entity,
						item));

				m_scene->add_primitive(item);
			}
		}
		m_entities_added.clear();

		for (const Entity* entity : m_entities_removed)
		{
			std::string id = entity->get_id();
			if (m_items.find(id) != m_items.end())
			{
				PlaneEntity* entry = m_items[id];

				PlanePrimitive* model = entry->plane;
				PlaneGraphics* graphics = static_cast<PlaneGraphics*>(entity->get_graphics());

				m_scene->remove_primitive(model);

				delete entry;
				delete model;
			}
		}
		m_entities_removed.clear();

		for (const Entity* entity : m_entities_changed)
		{
			std::string id = entity->get_id();
			if (m_items.find(id) != m_items.end())
			{
				PlaneEntity* entry = m_items[id];

				PlanePrimitive* item = entry->plane;
				PlaneGraphics* graphics = static_cast<PlaneGraphics*>(entity->get_graphics());

				item->set_translation(entity->get_position().to_vec3d());
				item->set_rotation(entity->get_orientation());
				item->set_width(graphics->get_width());
				item->set_show(graphics->get_show());
                item->set_height(graphics->get_height());
			}
		}
		m_entities_changed.clear();
	}

	void PlaneVisualizer::on_collection_changed(const EntityCollectionChangedEvent& event)
	{
		for (const Entity* entity : event.added)
		{
			if (dynamic_cast<PlaneGraphics*>(entity->get_graphics()) != nullptr)
			{
				m_entities_added.emplace(entity);
			}
		}

		for (const Entity* entity : event.removed)
		{
			if (dynamic_cast<PlaneGraphics*>(entity->get_graphics()) != nullptr)
			{
				m_entities_removed.emplace(entity);
			}
		}

		for (const Entity* entity : event.changed)
		{
			if (dynamic_cast<PlaneGraphics*>(entity->get_graphics()) != nullptr)
			{
				m_entities_changed.emplace(entity);
			}
		}
	}
}
