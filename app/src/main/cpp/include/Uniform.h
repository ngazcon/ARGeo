#pragma once

#include <string>

#include "ICleanableObserver.h"
#include "IUniform.h"

namespace argeo
{
	template<class T>
	class Uniform : public IUniform
	{
	public:

		T	 get_value() const;
		void set_value(T     value);

		void copy_value(IUniform* to_copy);

		std::string get_name() const;
		UniformType get_type() const;
		int get_location() const;
		virtual void clean() = 0;

	protected:

		Uniform(std::string name, UniformType type, int location, ICleanableObserver* observer);

		T m_value;

		// The name of the uniform.
		const std::string m_name;

		// The data type of the uniform.
		UniformType m_type;

		// The handle to gpu memory.
		int m_location;

		// The observer of this uniform clean state.
		ICleanableObserver* m_observer;

		// The clean state of this uniform.
		bool m_dirty;
	};

	template<class T>
	Uniform<T>::Uniform(std::string name, UniformType type, int location, ICleanableObserver* observer)
		: m_name(name)
		, m_type(type)
		, m_location(location)
		, m_dirty(false)
		, m_observer(observer)
	{ }

	template<class T>
	void Uniform<T>::copy_value(IUniform* to_copy)
	{
		set_value(dynamic_cast<Uniform<T>*>(to_copy)->get_value());
	}


	template<class T>
	T Uniform<T>::get_value() const
	{
		return m_value;
	}

	template<class T>
	void Uniform<T>::set_value(T value)
	{
		if (m_value != value)
		{
			m_value = value;

			if (!m_dirty)
			{
				m_observer->notify_dirty(this);
				m_dirty = true;
			}
		}
	}

	template<class T>
	std::string Uniform<T>::get_name() const
	{
		return m_name;
	}

	template<class T>
	UniformType Uniform<T>::get_type() const
	{
		return m_type;
	}

	template<class T>
	int Uniform<T>::get_location() const
	{
		return m_location;
	}
}

