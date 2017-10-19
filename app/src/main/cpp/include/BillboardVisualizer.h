#pragma once

#include "EntityCollection.h"
#include "Scene.h"

#include "Billboard.h"
#include "BillboardCollection.h"

#include "IVisualizer.h"

#include <vector>
#include <map>

namespace argeo
{
	struct BillboardEntity
	{
		BillboardEntity(
			const Entity* entity,
			Billboard*  billboard)
			: entity(entity)
			, billboard(billboard)
		{ }

		const Entity* entity;
		Billboard* billboard;
	};

	class BillboardVisualizer :
		public IVisualizer,
		public EntityCollectionChangedListener
	{
	public:
		BillboardVisualizer(
			EntityCollection* collection,
			Scene* scene);

		~BillboardVisualizer();

		void update(const EngineClockTick& time);
		void on_collection_changed(const EntityCollectionChangedEvent& event);

	private:
		EntityCollection* m_collection;
		std::unique_ptr<BillboardCollection> m_billboard_collection;

		Scene* m_scene;

		std::unordered_set<const Entity*> m_entities_added;
		std::unordered_set<const Entity*> m_entities_removed;
		std::unordered_set<const Entity*> m_entities_changed;
		std::map<std::string, BillboardEntity*> m_items;
	};
}