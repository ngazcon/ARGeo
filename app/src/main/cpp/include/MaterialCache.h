#pragma once

#include <map>
#include <string>

namespace argeo
{
	class Material;

	class MaterialCache
	{
	public:
		MaterialCache();
		~MaterialCache();

		void add_material(
			const std::string& name,
			Material* material);

		Material* get_material(const std::string& name);

	private:
		std::map<std::string, Material*> m_materials;
	};
}
