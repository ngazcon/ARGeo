#pragma once

#include "EntityCollectionChangedEvent.h"

#include <vector>

namespace argeo
{
	class EntityCollection;
	class Entity;

	class EntityCollectionChangedListener
	{
	public:
		virtual void on_collection_changed(const EntityCollectionChangedEvent& event) = 0;
	};
}
