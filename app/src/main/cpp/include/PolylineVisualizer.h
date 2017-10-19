#pragma once

#include "EntityCollection.h"
#include "Scene.h"

#include "Polyline.h"
#include "PolylineCollection.h"

#include "IVisualizer.h"

#include <vector>
#include <map>

namespace argeo
{
	struct PolylineEntity
	{
		PolylineEntity(
			const Entity* entity,
			Polyline*  polyline)
			: entity(entity)
			, polyline(polyline)
		{ }

		const Entity* entity;
		Polyline* polyline;
	};

	class PolylineVisualizer :
		public IVisualizer,
		public EntityCollectionChangedListener
	{
	public:
		PolylineVisualizer(
			EntityCollection* collection,
			Scene* scene);

		~PolylineVisualizer();

		void update(const EngineClockTick& time);
		void on_collection_changed(const EntityCollectionChangedEvent& event);

	private:
		EntityCollection* m_collection;
		std::unique_ptr<PolylineCollection> m_polyline_collection;

		Scene* m_scene;

		std::unordered_set<const Entity*> m_entities_added;
		std::unordered_set<const Entity*> m_entities_removed;
		std::unordered_set<const Entity*> m_entities_changed;
		std::map<std::string, PolylineEntity*> m_items;
	};
}