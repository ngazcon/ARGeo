#pragma once

#include <string>

namespace argeo
{
	class ShadowMap;

	class ShadowMapShader
	{
	public:
		static std::string create_shadow_cast_vertex_shader(
			const std::string& shader_source,
			const bool& is_point_light,
			const bool& is_terrain);


		static std::string create_shadow_cast_fragment_shader(
			const std::string& shader_source,
			const bool& is_point_light,
			const bool&	use_depth_texture,
			const bool& opaque);
		
		static std::string create_shadow_receive_vertex_shader(
			const std::string& shader_source,
			const bool& is_terrain,
			const bool& has_terrain_normal);


		static std::string create_shadow_receive_fragment_shader(
			const std::string& shader_source,
			ShadowMap* shadow_map,
			const bool&	cast_shadows,
			const bool& is_terrain,
			const bool& has_terrain_normal);

	};
}
