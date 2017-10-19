#pragma once

#include <assimp/scene.h>
#include <string>


namespace argeo
{
	
	struct ModelMaterial
	{
		ModelMaterial(
			std::string id,
			const aiMaterial* material)
			: material(material)
			, id(id)
		{ }

		std::string id;
		const aiMaterial* material;
	};
}