#pragma once

#include "ShaderVertexAttributeType.h"

#include <string>

namespace argeo
{
	class ShaderVertexAttribute
	{
	public:

		ShaderVertexAttribute(
			std::string name,
			int location, 
			int size,
			ShaderVertexAttributeType type);

		~ShaderVertexAttribute();

		// Get the name of this attribute.
		std::string get_name() const;

		// Get the size of this attribute.
		int get_size() const;

		// Get the layout location of this location.
		int get_location() const;

		// Get the type of this shader attribute.
		ShaderVertexAttributeType get_type() const;

	private:

		int m_size;

		// The name of the attribute.
		const std::string m_name;

		// The layout location of the attribute in the shader.
		int m_location;

		// The attribute type.
		ShaderVertexAttributeType m_type;
	};

	struct ShaderVertexAttributeHasher
	{
		std::size_t operator()(const ShaderVertexAttribute* t) const
		{
			//calculate hash here.
			return std::hash<std::string>()(t->get_name());
		}
	};
}
