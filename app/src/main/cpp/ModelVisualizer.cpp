#include "ModelVisualizer.h"

#include "ModelGraphics.h"

namespace argeo
{
	ModelVisualizer::ModelVisualizer(
		EntityCollection* collection,
		Scene* scene)
		: m_collection(collection)
		, m_scene(scene)
	{
		m_collection->add_listener(this);
	}


	ModelVisualizer::~ModelVisualizer()
	{
		m_collection->remove_listener(this);
	}

	void ModelVisualizer::update(const EngineClockTick& time)
	{
		for (const Entity* entity : m_entities_added)
		{
			std::string id = entity->get_id();
			if (m_items.find(id) == m_items.end())
			{
				ModelGraphics* graphics = static_cast<ModelGraphics*>(entity->get_graphics());

				Model* item = new Model(
					graphics->get_path(), 
					entity->get_name(),
					m_scene);
				
				item->set_translation(entity->get_position().to_vec3d());
				item->set_rotation(entity->get_orientation());
				item->set_height_clamping(graphics->get_height_clamping());
				item->set_orientation_clamping(graphics->get_orientation_clamping());
				item->set_owner(const_cast<void*>(static_cast<const void*>(entity)));
				
				m_items.emplace(
					id, 
					new ModelEntity(
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
				ModelEntity* entry = m_items[id];

				Model* model = entry->model;
				ModelGraphics* graphics = static_cast<ModelGraphics*>(entity->get_graphics());

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
				ModelEntity* entry = m_items[id];

				Model* item = entry->model;
				ModelGraphics* graphics = static_cast<ModelGraphics*>(entity->get_graphics());

				item->set_translation(entity->get_position().to_vec3d());
				item->set_rotation(entity->get_orientation());
				item->set_height_clamping(graphics->get_height_clamping());
				item->set_orientation_clamping(graphics->get_orientation_clamping());
			}
		}
		m_entities_changed.clear();
	}

	void ModelVisualizer::on_collection_changed(const EntityCollectionChangedEvent& event)
	{
		for (const Entity* entity : event.added)
		{
			if (dynamic_cast<ModelGraphics*>(entity->get_graphics()) != nullptr)
			{
				m_entities_added.emplace(entity);
			}
		}

		for (const Entity* entity : event.removed)
		{
			if (dynamic_cast<ModelGraphics*>(entity->get_graphics()) != nullptr)
			{
				m_entities_removed.emplace(entity);
			}
		}

		for (const Entity* entity : event.changed)
		{
			if (dynamic_cast<ModelGraphics*>(entity->get_graphics()) != nullptr)
			{
				m_entities_changed.emplace(entity);
			}
		}
	}
}
