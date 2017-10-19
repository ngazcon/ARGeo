#include "Material.h"
#include "Device.h"
#include "TextureHelper.h"
#include "Uniform.h"
#include "UniformSampler.h"

#include "PolylineMaterials.h"

#include <sstream>
#include <regex>
#include <future>

namespace argeo
{

	Material* Material::from_name(
		const std::string& name,
		const UniformValues& values)
	{
		if (g_material_cache.get_material(name) == nullptr) 
		{
			throw std::invalid_argument("material with type " + name + " does not exist.");
		}

		Material* material = new Material(name);

		for (auto entry : values)
		{
			material->get_uniform_values()[entry.first] = entry.second;
		}
		
		return material;
	}

	Material::Material(
		const std::string& name,
		const MaterialTemplate& material_template,
		const unsigned int& count)
		: m_count(count)
		, m_default_texture(nullptr)
		, m_default_cube_map_id("og_defaultCubeMap")
		, m_default_image_id("og_defaultImage")
	{
		initialize_material(
			name, 
			material_template);

		if (g_uniform_list.find(name) == g_uniform_list.end())
		{
			std::vector<std::string>& uniform_list = g_uniform_list[name];
			for (auto entry : m_uniforms)
			{
				uniform_list.push_back(entry.first);
			}
		}
	}

	Material::~Material()
	{
		for (auto entry : m_textures)
		{
			if (entry.second != m_default_texture)
			{
				delete entry.second;
			}
		}

		for (auto entry : m_materials)
		{
			delete entry.second;
		}
	}

	void Material::update(FrameState* frame_state)
	{
		Context* context = frame_state->get_context();

		for (LoadedImage loaded_image : m_loaded_images)
		{
			m_textures[loaded_image.id] = Device::create_texture2D(
				loaded_image.image,
				TextureHelper::imaging_pixel_format_to_texture_format(loaded_image.image->get_pixel_format())).release();
		}
		m_loaded_images.clear();

		for (UpdateMethod update_method : m_update_methods)
		{
			update_method(context);
		}

		for (auto entry : m_materials)
		{
			Material* sub_material = entry.second;
			sub_material->update(frame_state);
		}

	}

	void Material::validate_template()
	{
		// Make sure source and components do not exist in the same template.
		if (m_template.components.size() != 0 && !m_template.source.empty())
		{
			throw std::invalid_argument("Material template cannot have source and components in the same template.");
		}

		for (auto component : m_template.components)
		{
			if (component_properties.find(component.first) == component_properties.end())
			{
				throw std::invalid_argument("Invalid component in template.");
			}
		}
	}

	// Writes uniform declarations to the shader file and connects uniform m_values with
	// corresponding material properties through the returnUniforms function.
	void Material::create_uniform(
		std::string id,
		IUniformValue* value)
	{
		// Add uniform declaration to source coe.
		std::regex uniform_declaration_regex("uniform\\s+" + uniform_type_to_string(value->get_type()) + "\\s+" + id + "\\s*;");

		if (!std::regex_search(m_shader_source, uniform_declaration_regex))
		{
			std::string uniform_declaration = "uniform " + uniform_type_to_string(value->get_type()) + " " + id + ";\n";
			m_shader_source = uniform_declaration + m_shader_source;
		}

		std::stringstream id_stream;
		id_stream << id << "_" << m_count++;
		std::string new_id = id_stream.str();

		replace_token(this, id, new_id);

		// Set uniform value
		m_values[id] = value;

		switch (value->get_type())
		{
		case UniformType::SamplerCube:
		case UniformType::Sampler2D:
			m_uniforms[new_id] = [&, id](IUniform* uniform)
			{
				static_cast<UniformSampler*>(uniform)->set_texture(m_textures[id]);
			};
			m_update_methods.push_back(create_texture_2D_update_function(id));
			break;

		case UniformType::FloatVector3:
			m_uniforms[new_id] = [&, id](IUniform* uniform)
			{
				IUniformValue* value = m_values[id];
				static_cast<Uniform<vec3>*>(uniform)->set_value(
					static_cast<UniformValue<vec3>*>(value)->get_data());
			};
			break;

		case UniformType::FloatVector4:
			m_uniforms[new_id] = [&, id](IUniform* uniform)
			{
				IUniformValue* value = m_values[id];
				static_cast<Uniform<vec4>*>(uniform)->set_value(
					static_cast<UniformValue<vec4>*>(value)->get_data());
			};
			break;

		case UniformType::FloatMatrix33:
			m_uniforms[new_id] = [&, id](IUniform* uniform)
			{
				IUniformValue* value = m_values[id];
				static_cast<Uniform<mat3>*>(uniform)->set_value(
					static_cast<UniformValue<mat3>*>(value)->get_data());
			};
			break;

		case UniformType::FloatMatrix44:
			m_uniforms[new_id] = [&, id](IUniform* uniform)
			{
				IUniformValue* value = m_values[id];
				static_cast<Uniform<mat4>*>(uniform)->set_value(
					static_cast<UniformValue<mat4>*>(value)->get_data());
			};
			break;

		case UniformType::Float:
			m_uniforms[new_id] = [&, id](IUniform* uniform)
			{
				IUniformValue* value = m_values[id];
				static_cast<Uniform<float>*>(uniform)->set_value(
					static_cast<UniformValue<float>*>(value)->get_data());
			};
			break;
		}
	}

	void Material::create_uniforms()
	{
		m_uniforms.clear();
		for (auto entry : m_template.values)
		{
			create_uniform(
				entry.first,
				entry.second);
		}
	}

	void Material::create_method_definition()
	{
		if (!m_template.source.empty())
		{
			m_shader_source += m_template.source;
		}
		else
		{
			m_shader_source += "og_material og_getMaterial(og_materialInput materialInput)\n{\n";
			m_shader_source += "	og_material material = og_getDefaultMaterial(materialInput);\n";
			for (auto component : m_template.components)
			{
				m_shader_source += "	material." + component.first + " = " + component.second + ";\n";
			}
			m_shader_source += "	return material;\n}\n";
		}
	}

	void Material::initialize_material(
		const std::string& name,
		const MaterialTemplate& material_template)
	{
		m_name     = name;
		m_template = material_template;

		Material* cached_material = g_material_cache.get_material(m_name);

		// If the cache contains this material type, build the material template off of the stored template.
		if (cached_material != nullptr)
		{
			MaterialTemplate cached_template = cached_material->get_template();
			
			m_template.source = cached_template.source;

			m_template.components.insert(
				cached_template.components.begin(), 
				cached_template.components.end());

			m_template.materials.insert(
				cached_template.materials.begin(),
				cached_template.materials.end());

			m_template.values.insert(
				cached_template.values.begin(),
				cached_template.values.end());
		}

		validate_template();

		// If the material has a new type, add it to the cache.
		if (cached_material == nullptr)
		{
			g_material_cache.add_material(m_name, this);
		}

		create_method_definition();
		create_uniforms();
		create_sub_materials();
	}

	// Create all sub-materials by combining source and m_uniforms together.
	void Material::create_sub_materials()
	{
		for (auto entry : m_template.materials)
		{
			std::string id = entry.first;

			// Construct the sub-material.
			Material* sub_material = new Material(
				id,
				entry.second,
				m_count);

			m_count = sub_material->get_count();

			m_uniforms.insert(
				sub_material->m_uniforms.begin(),
				sub_material->m_uniforms.end());

			m_materials[id] = sub_material;

			// Make the material's czm_getMaterial unique by appending the sub-material type.

			std::string original_method_name = "og_getMaterial";

			std::stringstream new_method_name_stream;
			new_method_name_stream << original_method_name << "_" << m_count++;
			std::string new_method_name = new_method_name_stream.str();

			replace_token(sub_material, original_method_name, new_method_name);

			m_shader_source = sub_material->m_shader_source + m_shader_source;

			// Replace each material id with an czm_getMaterial method call.
			std::string material_method_call = new_method_name + "(materialInput)";

			replace_token(this, id, material_method_call);
		}
	}

	// Used for searching or replacing a token in a material's shader source with something else.
	// If excludePeriod is true, do not accept tokens that are preceded by periods.
	// http://stackoverflow.com/questions/641407/javascript-negative-lookbehind-equivalent
	unsigned int Material::replace_token(
		Material* material,
		std::string token,
		std::string new_token,
		bool exclude_period)
	{
		unsigned int count = 0;

		std::string string_to_replace;

		if (exclude_period)
		{
			string_to_replace = "(^|[^\\.]{1})" + token + "|" + token + "($|[^\\.]{1})\\1 \\2";
		}
		else
		{
			string_to_replace = "[(\\w)]?" + token + "[(\\w)]?";
		}

		std::regex regex(string_to_replace);

		material->m_shader_source = std::regex_replace(
			material->m_shader_source,
			regex,
			"$1" + new_token + "$2");


		std::ptrdiff_t const match_count(std::distance(
			std::sregex_iterator(string_to_replace.begin(), string_to_replace.end(), regex),
			std::sregex_iterator()));

		return match_count;
	}

	Material::UpdateMethod Material::create_texture_2D_update_function(std::string id)
	{
		IUniformValue* uniform_value = m_values[id];

		return [&, uniform_value, id](Context* context)
		{
			if (dynamic_cast<UniformValue<Texture*>*>(uniform_value) != nullptr)
			{
				update_texture_2D(
					dynamic_cast<UniformValue<Texture*>*>(uniform_value),
					id,
					context);
			}
			else
			{
				update_texture_2D(
					dynamic_cast<UniformValue<std::string>*>(uniform_value),
					id,
					context);
			}
		};
	}

	void Material::update_texture_2D(
		UniformValue<Texture*>* value,
		std::string id,
		Context* context)
	{
		static Texture* old_uniform_value;

		Texture* uniform_value = value->get_data();
		bool uniform_changed = old_uniform_value != uniform_value;
		old_uniform_value = uniform_value;

		if (!uniform_changed)
		{
			return;
		}

		Texture* texture = m_textures[id];

		// The uniform texture value has changed.
		if (uniform_value != texture)
		{
			m_texture_paths[id] = "";

			if (texture != m_default_texture)
			{
				delete texture;
			}

			m_textures[id] = uniform_value;
			return;
		}
	}

	void Material::update_texture_2D(
		UniformValue<std::string>* value,
		std::string id,
		Context* context)
	{
		static std::string old_uniform_value;

		std::string uniform_value = value->get_data();
		bool uniform_changed = strcmp(old_uniform_value.c_str(), uniform_value.c_str()) != 0;
		old_uniform_value = uniform_value;

		if (!uniform_changed)
		{
			return;
		}

		if (strcmp(uniform_value.c_str(), m_default_image_id.c_str()) == 0)
		{
			m_texture_paths[id] = "";

			if (m_default_texture == nullptr)
			{
				m_default_texture = context->get_default_texture2D();
			}

			m_textures[id] = m_default_texture;
			return;
		}

		if (uniform_value != m_texture_paths[id])
		{
			std::async(
				std::launch::async,
				&Material::load_image,
				this,
				id,
				uniform_value);
			m_texture_paths[id] = uniform_value;
		}
	}

	void Material::update_texture_2D(
		UniformValue<Bitmap*>* value,
		std::string id)
	{
		Bitmap* uniform_value = value->get_data();
		m_loaded_images.push_back(LoadedImage(
			id,
			uniform_value));
		//material._texturePaths[uniformId] = uniformValue;
	}

	void Material::load_image(
		std::string id,
		std::string path)
	{
		m_loaded_images.push_back(LoadedImage(
			id,
			Bitmap::create_bitmap_from_asset(path).release()));
	}

	int Material::get_count() const
	{
		return m_count;
	}

	MaterialTemplate Material::get_template() const
	{
		return m_template;
	}

	UniformMap& Material::get_uniform_map()
	{
		return m_uniforms;
	}

	UniformValues& Material::get_uniform_values()
	{
		return m_values;
	}

	std::string Material::get_shader_source() const
	{
		return m_shader_source;
	}

	std::string Material::get_name() const
	{
		return m_name;
	}

	std::string Material::uniform_type_to_string(UniformType type)
	{
		switch (type)
		{
		case argeo::UniformType::Float:
			return "float";
		case argeo::UniformType::Int:
			return "int";
		case argeo::UniformType::IntVector2:
			return "ivec2";
		case argeo::UniformType::IntVector3:
			return "ivec3";
		case argeo::UniformType::IntVector4:
			return "ivec4";
		case argeo::UniformType::Bool:
			return "bool";
		case argeo::UniformType::BoolVector2:
			return "bvec2";
		case argeo::UniformType::BoolVector3:
			return "bvec3";
		case argeo::UniformType::BoolVector4:
			return "bvec4";
		case argeo::UniformType::FloatVector2:
			return "vec2";
		case argeo::UniformType::FloatVector3:
			return "vec3";
		case argeo::UniformType::FloatVector4:
			return "vec4";
		case argeo::UniformType::FloatMatrix22:
			return "mat2x2";
		case argeo::UniformType::FloatMatrix33:
			return "mat3";
		case argeo::UniformType::FloatMatrix44:
			return "mat4";
		case argeo::UniformType::FloatMatrix23:
			return "mat2x3";
		case argeo::UniformType::FloatMatrix24:
			return "mat2x4";
		case argeo::UniformType::FloatMatrix32:
			return "mat3x2";
		case argeo::UniformType::FloatMatrix34:
			return "mat3x4";
		case argeo::UniformType::FloatMatrix42:
			return "mat4x2";
		case argeo::UniformType::FloatMatrix43:
			return "mat4x3";
		case argeo::UniformType::Sampler1D:
			return "sampler1D";
		case argeo::UniformType::Sampler2D:
			return "sampler2D";
		case argeo::UniformType::Sampler2DRect:
			return "sampler2DRect";
		case argeo::UniformType::Sampler2DRectShadow:
			return "sampler2DRectShadow";
		case argeo::UniformType::Sampler3D:
			return "sampler3D";
		case argeo::UniformType::SamplerCube:
			return "samplerCube";
		case argeo::UniformType::Sampler1DShadow:
			return "sampler1DShadow";
		case argeo::UniformType::Sampler2DShadow:
			return "sampler2DShadow";
		case argeo::UniformType::Sampler2DExternal:
			return "sampler2DExternal";
		case argeo::UniformType::Sampler1DArray:
			return "sampler1DArray";
		case argeo::UniformType::Sampler2DArray:
			return "sampler2DArray";
		case argeo::UniformType::Sampler1DArrayShadow:
			return "sampler1DArrayShadow";
		case argeo::UniformType::Sampler2DArrayShadow:
			return "sampler2DArrayShadow";
		case argeo::UniformType::SamplerCubeShadow:
			return "samplerCubeShadow";
		case argeo::UniformType::IntSampler1D:
			return "sampler1DArray";
		case argeo::UniformType::IntSampler2D:
			return "isampler2D";
		case argeo::UniformType::IntSampler2DRect:
			return "isampler2DRect";
		case argeo::UniformType::IntSampler3D:
			return "isampler3D";
		case argeo::UniformType::IntSamplerCube:
			return "isamplerCube";
		case argeo::UniformType::IntSampler1DArray:
			return "isampler1DArray";
		case argeo::UniformType::IntSampler2DArray:
			return "isampler2DArray";
		case argeo::UniformType::UnsignedIntSampler1D:
			return "usampler1D";
		case argeo::UniformType::UnsignedIntSampler2D:
			return "usampler2D";
		case argeo::UniformType::UnsignedIntSampler2DRect:
			return "usampler2DRect";
		case argeo::UniformType::UnsignedIntSampler3D:
			return "usampler3D";
		case argeo::UniformType::UnsignedIntSamplerCube:
			return "usamplerCube";
		case argeo::UniformType::UnsignedIntSampler1DArray:
			return "usampler1DArray";
		case argeo::UniformType::UnsignedIntSampler2DArray:
			return "usampler2DArray";
		}
	}


	std::map<std::string, std::vector<std::string>> Material::g_uniform_list;
	
	MaterialCache Material::g_material_cache;

	const std::string Material::polyline_outline_material_name = "PolylineOutline";
	const std::string Material::polyline_glow_material_name	   = "PolylineGlow";
	const std::string Material::color_material_name			   = "Color";

	Material::initializer Material::g_initializer;

	void Material::initialize()
	{ 
		g_material_cache.add_material(
			polyline_outline_material_name,
			new Material(
				polyline_outline_material_name,
				MaterialTemplate(
				polyline_outline_material,
				MaterialTemplate::Values
			{
				{ "color",        new UniformValue<vec4>(vec4(1.0, 1.0, 1.0, 1.0), UniformType::FloatVector4) },
				{ "outlineColor", new UniformValue<vec4>(vec4(1.0, 0.0, 0.0, 1.0), UniformType::FloatVector4) },
				{ "outlineWidth", new UniformValue<float>(2.0, UniformType::Float) },
			})));

		g_material_cache.add_material(
			polyline_glow_material_name,
			new Material(
				polyline_glow_material_name,
				MaterialTemplate(
					polyline_glow_material,
					MaterialTemplate::Values
		{
			{ "color",     new UniformValue<vec4>(vec4(0.0, 0.5, 1.0, 1.0), UniformType::FloatVector4) },
			{ "glowPower", new UniformValue<float>(1.0, UniformType::Float) },
		})));

		g_material_cache.add_material(
			color_material_name,
			new Material(
				color_material_name,
				MaterialTemplate(
					MaterialTemplate::Components
		{
			{ "diffuse", "color.rgb" },
			{ "alpha", "color.a" },
		},
					MaterialTemplate::Values
		{
			{ "color",     new UniformValue<vec4>(vec4(0.0, 0.5, 1.0, 1.0), UniformType::FloatVector4) },
		})));
	}
}
