#include "Entity.h"

#include "android/include/JNIHelper.h"
#include "include/guid.h"

namespace argeo
{
	Entity::Entity(
		const std::string& id,
		const std::string& name,
		IGraphics* graphics)
		: m_name(name)
		, m_id(id)
		, m_graphics(graphics)
		, m_orientation(quaternion::identity())
		, m_position(vec3d::zero())
	{ 
		m_graphics->add_property_changed_listener(this);
	}

	Entity::Entity(
		const std::string& name,
		IGraphics* graphics)
		: m_name(name)
		, m_graphics(graphics)
		, m_orientation(quaternion::identity())
		, m_position(vec3d::zero())
	{ 
		std::ostringstream stream;

#ifdef __ANDROID__
		JNIEnv* jni;
		jint status = JNIHelper::get_instance().attach_current_thread(&jni);
		GuidGenerator generator(jni);
		stream << generator.newGuid();
		JNIHelper::get_instance().detach_current_thread(status);
#else
		GuidGenerator generator;
		stream << generator.newGuid();
#endif

		m_id = stream.str();
		m_graphics->add_property_changed_listener(this);
	}

	Entity::~Entity()
	{ 
		delete m_graphics;
	}

	void Entity::on_property_changed(const PropertyChangedEvent& event)
	{
		raise_on_property_changed(
			event.name,
			event.old_value,
			event.new_value);
	}

	std::string Entity::get_id() const
	{
		return m_id;
	}

	std::string Entity::get_name() const
	{
		return m_name;
	}

	IGraphics* Entity::get_graphics() const
	{
		return m_graphics;
	}

	Geocentric3D Entity::get_position() const
	{
		return m_position;
	}

	void Entity::set_position(const Geocentric3D& value)
	{
		if (m_position != value)
		{
			raise_on_property_changed(
				"position",
				&m_position,
				&value);
			m_position = value;
		}
	}

	quaternion Entity::get_orientation() const
	{
		return m_orientation;
	}

	void Entity::set_orientation(const quaternion& value)
	{
		if (m_orientation != value)
		{
			raise_on_property_changed(
				"orientation",
				&m_orientation,
				&value);
			m_orientation = value;
		}
	}

	void Entity::set_collection(EntityCollection* value)
	{
		m_collection = value;
	}

	EntityCollection* Entity::get_collection() const
	{
		return m_collection;
	}
}