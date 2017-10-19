#pragma once

#include "UniformMap.h"

#include <unordered_set>
#include <string>

namespace argeo
{
	struct MaterialTemplate
	{
		typedef std::map<std::string, std::string>      Components;
		typedef std::map<std::string, MaterialTemplate> Materials;
		typedef std::map<std::string, IUniformValue*>   Values;

		MaterialTemplate(
			std::string source,
			Values values = Values())
			: source(source)
			, values(values)
		{ }

		MaterialTemplate(
			Components components = Components(),
			Values values         = Values(),
			Materials materials   = Materials())
			: components(components)
			, values(values)
			, materials(materials)
		{ }

		std::string source;
		Components  components;
		Values		values;
		Materials   materials;
	};
}