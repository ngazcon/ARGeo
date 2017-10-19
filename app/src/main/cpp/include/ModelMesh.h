#pragma once

#include "ModelMaterial.h"

#include <string>	
#include <map>

namespace argeo
{
	struct ModelMesh
	{
		ModelMesh(
			std::string id,
			std::string name,
			std::string material)
			: id(id)
			, name(name)
			, material(material)
		{ }

		std::string id;
		std::string material;
		std::string name;
	};
}

