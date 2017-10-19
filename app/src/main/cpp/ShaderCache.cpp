#include "ShaderCache.h"

#include "Device.h"

#include <sstream>

namespace argeo
{
	ShaderCache::ShaderCache()
	{ }

	ShaderCache::~ShaderCache()
	{
		for (auto entry : m_cached_shader_programs)
		{
			release_shader_program(entry.second->shader_program);
		}

		destroy_released_shader_programs();
	}

	ShaderProgram* ShaderCache::get_shader_program(
		const std::string& vertex_shader_source,
		const std::string& fragment_shader_source,
		const ShaderVertexAttributeLocations& vertex_attribute_locations)
	{
		std::stringstream attribute_locations_stream;
		for (auto entry : vertex_attribute_locations)
		{
			attribute_locations_stream << entry.first << entry.second;
		}
		std::string keyword = vertex_shader_source + fragment_shader_source + attribute_locations_stream.str();

		CachedShaderProgram* cached_shader;

		if (m_cached_shader_programs.find(keyword) != m_cached_shader_programs.end())
		{
			cached_shader = m_cached_shader_programs[keyword];

			// No longer want to release this if it was previously released.
			m_shader_programs_to_release.erase(keyword);
		}
		else 
		{
			ShaderProgram* shader_program = Device::create_shader_program(
				vertex_shader_source,
				fragment_shader_source,
				vertex_attribute_locations).release();

			cached_shader = new CachedShaderProgram(
				shader_program,
				keyword);

			shader_program->set_cached_shader_program(cached_shader);

			// A shader can't be in more than one cache.
			m_cached_shader_programs[keyword] = cached_shader;
		}
		
		cached_shader->count++;
		return cached_shader->shader_program;
	}

	void ShaderCache::release_shader_program(ShaderProgram* shader_program)
	{
		CachedShaderProgram* cached_shader = shader_program->get_cached_shader_program();
		
		if (cached_shader && (--cached_shader->count == 0))
		{
			m_shader_programs_to_release[cached_shader->keyword] = cached_shader;
		}
	}

	void ShaderCache::destroy_released_shader_programs()
	{
		for (auto entry : m_shader_programs_to_release)
		{
			std::string keyword = entry.first;
			CachedShaderProgram* cached_shader = m_shader_programs_to_release[keyword];

			delete m_cached_shader_programs[keyword];
			m_cached_shader_programs.erase(keyword);

			delete cached_shader->shader_program;
		}

		m_shader_programs_to_release.clear();
	}
}