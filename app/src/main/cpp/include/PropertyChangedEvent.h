#pragma once

#include <string>

namespace argeo
{
	class NotifyPropertyChanged;

	struct PropertyChangedEvent
	{
		PropertyChangedEvent(
			const NotifyPropertyChanged* source,
			const std::string& name,
			const void* old_value,
			const void* new_value)
			: source(source)
			, name(name)
			, old_value(old_value)
			, new_value(new_value)
		{ }

		const std::string name;
		const void* old_value;
		const void* new_value;
		const NotifyPropertyChanged* source;
	};
}