#pragma once

#include "Geocentric3D.h"
#include "Quaternion.h"
#include "NotifyPropertyChanged.h"
#include "IGraphics.h"

#include <string>

namespace argeo
{
	class EntityCollection;

	class Entity 
		: public NotifyPropertyChanged
		, public IPropertyChangedListener
	{
	public:
		
		Entity(
			const std::string& name,
			IGraphics* graphics);

		Entity(
			const std::string& id,
			const std::string& name,
			IGraphics* graphics);

		void set_collection(EntityCollection* value);
		EntityCollection* get_collection() const;

		std::string get_id()      const;
		std::string get_name()    const;
		IGraphics*  get_graphics() const;

		Geocentric3D get_position() const;
		void set_position(const Geocentric3D& value);

		quaternion get_orientation() const;
		void set_orientation(const quaternion& value);

		void on_property_changed(const PropertyChangedEvent& event);

		~Entity();

	private:
		Geocentric3D m_position;
		quaternion m_orientation;

		std::string m_id;
		std::string m_name;

		EntityCollection* m_collection;
		IGraphics* m_graphics;
	};
}
