#pragma once

#include "ShaderProgram.h"
#include "ShaderVertexAttributeLocations.h"

#include <string>
#include <map>

namespace argeo
{
	struct CachedShaderProgram
	{
		CachedShaderProgram(
			ShaderProgram* shader_program,
			const std::string& keyword,
			const unsigned int& count = 0)
			: shader_program(shader_program)
			, keyword(keyword)
			, count(count)
		{ }
		
		ShaderProgram* shader_program;
		std::string keyword;
		unsigned int count;
	};

	class ShaderCache
	{
	public:
		ShaderCache();
		~ShaderCache();

		void destroy_released_shader_programs();
		void release_shader_program(ShaderProgram* shader_program);

		ShaderProgram* get_shader_program(
			const std::string& vertex_shader_source,
			const std::string& fragment_shader_source,
			const ShaderVertexAttributeLocations& vertex_attribute_locations = ShaderVertexAttributeLocations());

	private:
		std::map<std::string, CachedShaderProgram*> m_cached_shader_programs;
		std::map<std::string, CachedShaderProgram*> m_shader_programs_to_release;
	};
}
