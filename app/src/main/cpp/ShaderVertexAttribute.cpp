#include "ShaderVertexAttribute.h"


namespace argeo
{
	ShaderVertexAttribute::ShaderVertexAttribute(
		std::string name,
		int location, 
		int size,
		ShaderVertexAttributeType type)
		: m_name(name),
		  m_location(location),
		  m_size(size),
		  m_type(type)
	{ }


	ShaderVertexAttribute::~ShaderVertexAttribute()
	{ }

	std::string ShaderVertexAttribute::get_name() const
	{
		return m_name;
	}

	int ShaderVertexAttribute::get_size() const
	{
		return m_size;
	}

	int ShaderVertexAttribute::get_location() const
	{
		return m_location;
	}

	ShaderVertexAttributeType ShaderVertexAttribute::get_type() const
	{
		return m_type;
	}
}