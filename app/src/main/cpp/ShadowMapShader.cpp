#include "ShadowMapShader.h"
#include "ShadowMap.h"
#include "ShaderObject.h"
#include "ShaderObjectGL.h"

namespace argeo
{
	std::string ShadowMapShader::create_shadow_cast_vertex_shader(
		const std::string& shader_source,
		const bool& is_point_light,
		const bool& is_terrain)
	{
		std::string modified_source;

		if (is_terrain) 
		{
			modified_source += "#define GENERATE_POSITION 1";
		}

		std::string position_output_name = ShaderObjectGL::find_position_output(shader_source);

		if (is_point_light && !position_output_name.empty())
		{			
			modified_source = ShaderObjectGL::replace_main(shader_source, "og_shadow_cast_main");

			std::string shadow_vs =
				R"(
				out vec3 v_positionEC;
				void main()
				{
				    og_shadow_cast_main();
				    v_positionEC = (og_inverseProjectionMatrix * gl_Position).xyz;
				})";

			modified_source += shadow_vs;
		}
		else
		{
			modified_source += shader_source;
		}

		return modified_source;
	}


	std::string ShadowMapShader::create_shadow_cast_fragment_shader(
		const std::string& shader_source,
		const bool& is_point_light,
		const bool&	use_depth_texture,
		const bool& opaque)
	{
		std::string position_output_name = ShaderObjectGL::find_position_output(shader_source);
		bool has_position_output  = !position_output_name.empty();

		if (!has_position_output)
		{
			position_output_name = 'fsPositionEC';
		}

		std::string modified_source = ShaderObjectGL::replace_main(
			shader_source, 
			"og_shadow_cast_main");

		if (is_point_light) 
		{
			if (!has_position_output)
			{
				modified_source += "in vec3 v_positionEC;\n";
			}

			modified_source += "uniform vec4 shadowMap_lightPositionEC;\n";
		}

		if (opaque) 
		{
			modified_source +=
				R"(
				void main() +
				{)";
		}
		else 
		{
			modified_source +=
				R"(
				void main() +
				{
				    og_shadow_cast_main();
				    if (gl_FragColor.a == 0.0)
				    {
				       discard;
				    }
				)";
		}

		if (is_point_light) 
		{
			modified_source +=
				"float distance = length(" + position_output_name + "); \n" +
				"distance /= shadowMap_lightPositionEC.w; // radius \n" +
				"gl_FragColor = czm_packDepth(distance); \n";
		}
		else if (use_depth_texture)
		{
			modified_source += "gl_FragColor = vec4(1.0); \n";
		}
		else 
		{
			modified_source += "gl_FragColor = czm_packDepth(gl_FragCoord.z); \n";
		}

		modified_source += "} \n";

		return modified_source;
	}

	std::string ShadowMapShader::create_shadow_receive_vertex_shader(
		const std::string& shader_source,
		const bool& is_terrain,
		const bool& has_terrain_normal)
	{
		std::string modified_source;

		if (is_terrain)
		{
			if (has_terrain_normal)
			{
				modified_source += "#define GENERATE_POSITION_AND_NORMAL 1";
			}
			else 
			{
				modified_source += "#define GENERATE_POSITION 1";
			}
		}

		modified_source += shader_source;

		return modified_source;
	}


	std::string ShadowMapShader::create_shadow_receive_fragment_shader(
		const std::string& shader_source,
		ShadowMap* shadow_map,
		const bool&	cast_shadows,
		const bool& is_terrain,
		const bool& has_terrain_normal)
	{
		std::string modified_source;

		std::string normal_output_name = ShaderObjectGL::find_normal_output(shader_source);
		bool has_normal_output = (!is_terrain && !normal_output_name.empty()) || (is_terrain && has_terrain_normal);

		std::string position_output_name = ShaderObjectGL::find_position_output(shader_source);
		bool has_position_output = !position_output_name.empty();


		

		bool usesDepthTexture = shadow_map->get_uses_depth_texture();
		bool polygon_offset_supported = shadow_map->get_polygon_offset_supported();
		bool is_point_light = shadow_map->get_is_point_light();
		bool is_spot_light  = shadow_map->get_is_spot_light();
		bool has_cascades   = shadow_map->get_number_of_cascades() > 1;
		bool soft_shadows   = shadow_map->get_soft_shadows();
	
		Bias bias = is_point_light ? 
			shadow_map->get_point_bias() : 
			(is_terrain ? shadow_map->get_terrain_bias() : shadow_map->get_primitive_bias());

		modified_source = ShaderObjectGL::replace_main(shader_source, "czm_shadow_receive_main");
		
		if (is_point_light) 
		{
			modified_source = "#define USE_CUBE_MAP_SHADOW 1 \n" + modified_source;
		}
		else if (usesDepthTexture) 
		{
			modified_source = "#define USE_SHADOW_DEPTH_TEXTURE 1" + modified_source;
		}
		
		if (soft_shadows && !is_point_light) 
		{
			modified_source = "#define USE_SOFT_SHADOWS 1" + modified_source;
		}
		
		//// Enable day-night shading so that the globe is dark when the light is below the horizon
		if (has_cascades && cast_shadows && is_terrain)
		{
			if (has_normal_output) 
			{
				modified_source = "#define ENABLE_VERTEX_LIGHTING 1" + modified_source;
			}
			else 
			{
				modified_source = "#define ENABLE_DAYNIGHT_SHADING 1" + modified_source;
			}
		}
		
		if (cast_shadows && bias.normal_shading && has_normal_output)
		{
			modified_source = "#define USE_NORMAL_SHADING 1" + modified_source;
			if (bias.normal_shading_smooth > 0.0)
			{
				modified_source = "#define USE_NORMAL_SHADING_SMOOTH 1" + modified_source;
			}
		}

		if (is_point_light) 
		{
			modified_source += 'uniform samplerCube shadowMap_textureCube; \n';
		}
		else 
		{
			modified_source += 'uniform sampler2D shadowMap_texture; \n';
		}

		modified_source +=
			R"(
			uniform mat4 shadowMap_matrix; \n +
			uniform vec3 shadowMap_lightDirectionEC; \n +
			uniform vec4 shadowMap_lightPositionEC; \n +
			uniform vec4 shadowMap_normalOffsetScaleDistanceMaxDistanceAndDarkness; \n +
			uniform vec4 shadowMap_texelSizeDepthBiasAndNormalShadingSmooth; 
			vec4 getPositionEC()
			)"
			"{ \n" +
			(has_position_output ?
				"    return vec4(" + position_output_name + ", 1.0); \n" :
				"    return czm_windowToEyeCoordinates(gl_FragCoord); \n") +
			"} \n" +
			"vec3 getNormalEC() \n" +
			"{ \n" +
			(has_normal_output ?
				"    return normalize(" + normal_output_name + "); \n" :
				"    return vec3(1.0); \n") +
			"} \n" +
		
			// Offset the shadow position in the direction of the normal for perpendicular and back faces
			"void applyNormalOffset(inout vec4 positionEC, vec3 normalEC, float nDotL) \n" +
			"{ \n" +
			(bias.normal_offset && has_normal_output ?
				"    float normalOffset = shadowMap_normalOffsetScaleDistanceMaxDistanceAndDarkness.x; \n" 
				"    float normalOffsetScale = 1.0 - nDotL; \n"
				"    vec3 offset = normalOffset * normalOffsetScale * normalEC; \n"
				"    positionEC.xyz += offset; \n" : "") +
			"} \n";
		
		modified_source +=
			R"(
			void main()
			{
			    czm_shadow_receive_main();
			    vec4 positionEC = getPositionEC();
			    vec3 normalEC = getNormalEC();
			    float depth = -positionEC.z;

				czm_shadowParameters shadowParameters;
				shadowParameters.texelStepSize  = shadowMap_texelSizeDepthBiasAndNormalShadingSmooth.xy;
				shadowParameters.depthBias      = shadowMap_texelSizeDepthBiasAndNormalShadingSmooth.z;
				shadowParameters.normalShadingSmooth = shadowMap_texelSizeDepthBiasAndNormalShadingSmooth.w;
				shadowParameters.darkness		     = shadowMap_normalOffsetScaleDistanceMaxDistanceAndDarkness.w;
			)";
		
		if (is_terrain) 
		{
			// Scale depth bias based on view distance to reduce z-fighting in distant terrain
			modified_source += "    shadowParameters.depthBias *= max(depth * 0.01, 1.0); \n";
		}
		else if (!polygon_offset_supported) 
		{
			// If polygon offset isn't supported push the depth back based on view, however this
			// causes light leaking at further away views
			modified_source += "    shadowParameters.depthBias *= mix(1.0, 100.0, depth * 0.0015); \n";
		}

		
		if (is_point_light) 
		{
			modified_source +=
				R"(
				    vec3 directionEC = positionEC.xyz - shadowMap_lightPositionEC.xyz;
				    float distance = length(directionEC);
				    directionEC = normalize(directionEC);
				    float radius = shadowMap_lightPositionEC.w;

				    // Stop early if the fragment is beyond the point light radius
				    if (distance > radius)
				    {
				        return; 
				    }

				    vec3 directionWC  = czm_inverseViewRotation * directionEC;
		
				    shadowParameters.depth = distance / radius;
				    shadowParameters.nDotL = clamp(dot(normalEC, -directionEC), 0.0, 1.0);
		
				    shadowParameters.texCoords = directionWC;
				    float visibility = czm_shadowVisibility(shadowMap_textureCube, shadowParameters);
				)";
		}
		else if (is_spot_light)
		{
			modified_source +=
				R"(
				    vec3 directionEC = normalize(positionEC.xyz - shadowMap_lightPositionEC.xyz);
				    float nDotL = clamp(dot(normalEC, -directionEC), 0.0, 1.0);
				    applyNormalOffset(positionEC, normalEC, nDotL);
		
				    vec4 shadowPosition = shadowMap_matrix * positionEC;
				    // Spot light uses a perspective projection, so perform the perspective divide 
				    shadowPosition /= shadowPosition.w;
		
				    // Stop early if the fragment is not in the shadow bounds
				    if (any(lessThan(shadowPosition.xyz, vec3(0.0))) || any(greaterThan(shadowPosition.xyz, vec3(1.0))))
				    {
				        return;
				    }
		
				    shadowParameters.texCoords = shadowPosition.xy;
				    shadowParameters.depth = shadowPosition.z;
				    shadowParameters.nDotL = nDotL;
		
				    float visibility = czm_shadowVisibility(shadowMap_texture, shadowParameters);
				)";
		}
		else if (has_cascades)
		{
			modified_source +=
				R"(
				    float maxDepth = shadowMap_cascadeSplits[1].w;

				    // Stop early if the eye depth exceeds the last cascade
				    if (depth > maxDepth)
				    {
				        return;
				    }

				    // Get the cascade based on the eye-space depth
				    vec4 weights = czm_cascadeWeights(depth);

				    // Apply normal offset
				    float nDotL = clamp(dot(normalEC, shadowMap_lightDirectionEC), 0.0, 1.0);
				    applyNormalOffset(positionEC, normalEC, nDotL);

				    // Transform position into the cascade
				    vec4 shadowPosition = czm_cascadeMatrix(weights) * positionEC;

				    // Get visibility
				    shadowParameters.texCoords = shadowPosition.xy;
				    shadowParameters.depth = shadowPosition.z; 
				    shadowParameters.nDotL = nDotL;
				    float visibility = czm_shadowVisibility(shadowMap_texture, shadowParameters);

				    // Fade out shadows that are far away
				    float shadowMapMaximumDistance = shadowMap_normalOffsetScaleDistanceMaxDistanceAndDarkness.z;
				    float fade = max((depth - shadowMapMaximumDistance * 0.8) / (shadowMapMaximumDistance * 0.2), 0.0);
				    visibility = mix(visibility, 1.0, fade);

				)";
		}
		else 
		{
			modified_source +=
				R"(
				    float nDotL = clamp(dot(normalEC, shadowMap_lightDirectionEC), 0.0, 1.0);
				    applyNormalOffset(positionEC, normalEC, nDotL);
				    vec4 shadowPosition = shadowMap_matrix * positionEC;
		
				    // Stop early if the fragment is not in the shadow bounds \n' +
				    if (any(lessThan(shadowPosition.xyz, vec3(0.0))) || any(greaterThan(shadowPosition.xyz, vec3(1.0))))
				    {
				        return;
				    }
		
				    shadowParameters.texCoords = shadowPosition.xy;
				    shadowParameters.depth = shadowPosition.z;
				    shadowParameters.nDotL = nDotL;
				    float visibility = czm_shadowVisibility(shadowMap_texture, shadowParameters);
				)";
		}

		return modified_source;
	}
}