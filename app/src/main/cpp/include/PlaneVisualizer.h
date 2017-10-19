#pragma once

#include "EntityCollection.h"
#include "Scene.h"
#include "PlanePrimitive.h"

#include "IVisualizer.h"

#include <vector>
#include <map>

namespace argeo
{
	struct PlaneEntity
	{
		PlaneEntity(
			const Entity* entity,
			PlanePrimitive*  plane)
			: entity(entity)
			, plane(plane)
		{ }

		const Entity* entity;
		PlanePrimitive*  plane;
	};

	class PlaneVisualizer :
		public IVisualizer,
		public EntityCollectionChangedListener
	{
	public:
		PlaneVisualizer(
			EntityCollection* collection,
			Scene* scene);

		~PlaneVisualizer();

		void update(const EngineClockTick& time);
		void on_collection_changed(const EntityCollectionChangedEvent& event);

	private:
		EntityCollection* m_collection;
		Scene* m_scene;

		std::unordered_set<const Entity*> m_entities_added;
		std::unordered_set<const Entity*> m_entities_removed;
		std::unordered_set<const Entity*> m_entities_changed;
		std::map<std::string, PlaneEntity*> m_items;
	};
}