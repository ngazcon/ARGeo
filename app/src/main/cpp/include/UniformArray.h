#pragma once

#include "ICleanableObserver.h"
#include "IUniform.h"

#include <string>
#include <vector>

namespace argeo
{
	template<class T>
	class UniformArray : public IUniform
	{
	public:

		int get_size() const;
		
		std::vector<T> get_values();
		void set_values(std::vector<T> values);

		void copy_value(IUniform* to_copy);

		std::string get_name() const;

		UniformType get_type() const;

		int get_location() const;

	protected:

		UniformArray(std::string name, UniformType type, int location, int resource_size, ICleanableObserver* observer);

		std::vector< T > m_values;

		// For active uniforms that are arrays, the size is the number of active elements in the array; for all other uniforms, the size is one.
		int m_size;

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
	UniformArray<T>::UniformArray(std::string name, UniformType type, int location, int resource_size, ICleanableObserver* observer)
		: m_name(name),
		m_type(type),
		m_location(location),
		m_dirty(false),
		m_observer(observer),
		m_size(resource_size)
	{
		m_values.reserve(m_size);
	}

	template<class T>
	int UniformArray<T>::get_size() const
	{
		return m_size;
	}

	template<class T>
	std::vector<T> UniformArray<T>::get_values()
	{
		return m_values;
	}

	template<class T>
	void UniformArray<T>::copy_value(IUniform* to_copy)
	{
		set_values(dynamic_cast<UniformArray<T>*>(to_copy)->get_values());
	}

	template<class T>
	void UniformArray<T>::set_values(std::vector<T> values)
	{
		bool equals = true;

		if (m_values.size() == m_size)
		{
			for (int i = 0; i < m_size && equals; i++)
			{
				equals = (m_values[i] == values[i]);
			}
		}
		else
		{
			equals = false;
		}


		if (!equals)
		{
			m_values.clear();

			for (int i = 0; i < m_size; i++)
			{
				m_values.push_back(values[i]);
			}

			if (!m_dirty)
			{
				m_observer->notify_dirty(this);
				m_dirty = true;
			}
		}
	}

	template<class T>
	std::string UniformArray<T>::get_name() const
	{
		return m_name;
	}

	template<class T>
	UniformType UniformArray<T>::get_type() const
	{
		return m_type;
	}

	template<class T>
	int UniformArray<T>::get_location() const
	{
		return m_location;
	}
}
