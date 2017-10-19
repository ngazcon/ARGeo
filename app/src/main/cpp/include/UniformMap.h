#pragma once

#include <map>
#include <string>

#include <functional>
#include <sstream>
#include <IUniform.h>

namespace argeo
{
	class IUniformValue
	{
	public:
		virtual UniformType get_type()  = 0;
		virtual std::string to_string() = 0;
	};

	template <class T>
	class UniformValue : public IUniformValue
	{
	public:
		UniformValue(
			T           data,
			UniformType type)
			: m_type(type)
			, m_data(data)
		{ }

		UniformType get_type()
		{
			return m_type;
		}

		T get_data()
		{
			return m_data;
		}

		void set_data(T data)
		{
			m_data = data;
		}

		std::string to_string()
		{
			std::ostringstream stream;
			stream << static_cast<typename std::underlying_type<UniformType>::type>(m_type);
			stream << m_data;
			return stream.str();
		}

	private:
		UniformType m_type;
		T m_data;
	};

	typedef std::function<void(IUniform*)> UniformFunction;

	typedef std::map<std::string, UniformFunction> UniformMap;
	typedef std::map<std::string, IUniformValue*>  UniformValues;
}


