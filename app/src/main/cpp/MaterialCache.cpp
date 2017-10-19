#include "MaterialCache.h"


namespace argeo
{
	MaterialCache::MaterialCache()
	{ }


	MaterialCache::~MaterialCache()
	{ }

	void MaterialCache::add_material(
		const std::string& name,
		Material* material)
	{
		m_materials[name] = material;
	}

	Material* MaterialCache::get_material(const std::string& name)
	{
		if (m_materials.find(name) == m_materials.end())
		{
			return nullptr;
		}

		return m_materials.at(name);
	}
}