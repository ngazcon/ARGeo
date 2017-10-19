#pragma once

#include <vector>

namespace argeo
{
	class Entity;
	class EntityCollection;

	struct EntityCollectionChangedEvent
	{
		EntityCollectionChangedEvent(
			const EntityCollection* source,
			const std::vector<const Entity*>& added,
			const std::vector<const Entity*>& removed,
			const std::vector<const Entity*>& changed)
			: source(source)
			, added(added)
			, removed(removed)
			, changed(changed)
		{ }

		const EntityCollection* source;
		const std::vector<const Entity*>& added;
		const std::vector<const Entity*>& removed;
		const std::vector<const Entity*>& changed;
	};
}
