#pragma once

#include "EntityCollection.h"
#include "Scene.h"
#include "Model.h"

#include "IVisualizer.h"

#include <vector>
#include <map>

namespace argeo
{
	struct ModelEntity
	{
		ModelEntity(
			const Entity* entity,
			Model*  model)
			: entity(entity)
			, model(model)
		{ }

		const Entity* entity;
		Model*  model;
	};

	class ModelVisualizer : 
		public IVisualizer,
		public EntityCollectionChangedListener
	{
	public:
		ModelVisualizer(
			EntityCollection* collection,
			Scene* scene);

		~ModelVisualizer();

		void update(const EngineClockTick& time);		
		void on_collection_changed(const EntityCollectionChangedEvent& event);

	private:
		EntityCollection* m_collection;
		Scene* m_scene;

		std::unordered_set<const Entity*> m_entities_added;
		std::unordered_set<const Entity*> m_entities_removed;
		std::unordered_set<const Entity*> m_entities_changed;
		std::map<std::string, ModelEntity*> m_items;
	};
}