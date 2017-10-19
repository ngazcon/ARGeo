#pragma once

#include "IVertexAttribute.h"

#include <vector>

namespace argeo
{
	template<class T>
	class VertexAttribute : public IVertexAttribute
	{
	protected:

		VertexAttribute(
			const std::string& name,
			const VertexAttributeType& type)
			: m_name(name)
			, m_type(type)
		{ }

		VertexAttribute(
			const std::string& name,
			const VertexAttributeType& type,
			const int& capacity)
			: m_name(name)
			, m_type(type)
		{
			m_values.reserve(capacity);
		}

	public:

		std::vector<T> get_values() const
		{
			return m_values;
		}

		void set_values(const std::vector<T>& value)
		{
			m_values = value;
		}

		void set_values(
			const std::vector<T>& value,
			const T& max,
			const T& min)
		{
			m_values = value;
			m_max = max;
			m_min = min;
		}

		T get_max() const
		{
			return m_max;
		}

		void set_max(const T& max)
		{
			m_max = max;
		}

		T get_min() const
		{
			return m_min;
		}

		void set_min(const T& min)
		{
			m_min = min;
		}

		std::string get_name() const
		{
			return m_name;
		}

		VertexAttributeType get_type() const
		{
			return m_type;
		}

	protected:
		std::vector<T> m_values;
		T m_min;
		T m_max;
		std::string m_name;
		VertexAttributeType m_type;
	};
}