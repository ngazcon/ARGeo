#pragma once

#include "UniformMap.h"
#include "Texture2D.h"
#include "FrameState.h"
#include "Bitmap.h"
#include "MaterialTemplate.h"
#include "MaterialCache.h"

#include <unordered_set>
#include <string>

namespace argeo
{
	struct LoadedImage
	{
		LoadedImage()
		{ }

		LoadedImage(
			std::string id,
			Bitmap* image)
			: id(id)
			, image(image)
		{ }

		std::string id;
		Bitmap* image;
	};

	class Material
	{
		typedef std::function<void(Context*)> UpdateMethod;

	public:		

		Material(
			const std::string& name,
			const MaterialTemplate& material_template = MaterialTemplate(),
			const unsigned int&     count = 0);

		~Material();

		void update(FrameState* frame_state);

		std::string get_name() const;

		static const std::string polyline_outline_material_name;
		static const std::string polyline_glow_material_name;
		static const std::string color_material_name;

		static Material* from_name(
			const std::string& name,
			const UniformValues& values = UniformValues());

	private:
		MaterialTemplate get_template() const;

		void validate_template();

		// Writes uniform declarations to the shader file and connects uniform values with
		// corresponding material properties through the returnUniforms function.
		void create_uniform(
			std::string id,
			IUniformValue* value);

		void create_uniforms();
		void create_method_definition();
		
		void initialize_material(
			const std::string& name,
			const MaterialTemplate& material_template);

		// Create all sub-materials by combining source and uniforms together.
		void create_sub_materials();

		// Used for searching or replacing a token in a material's shader source with something else.
		// If excludePeriod is true, do not accept tokens that are preceded by periods.
		// http://stackoverflow.com/questions/641407/javascript-negative-lookbehind-equivalent
		unsigned int replace_token(
			Material* material,
			std::string token,
			std::string new_token,
			bool exclude_period = true);

		UpdateMethod create_texture_2D_update_function(std::string id);

		void update_texture_2D(
			UniformValue<Texture*>* value,
			std::string id,
			Context* context);

		void update_texture_2D(
			UniformValue<std::string>* value,
			std::string id,
			Context* context);

		void update_texture_2D(
			UniformValue<Bitmap*>* value,
			std::string id);

		void load_image(
			std::string id,
			std::string path);

		struct initializer
		{
			initializer()
			{
				initialize();
			}
		};
		static void initialize();
		static initializer g_initializer;

		static std::string uniform_type_to_string(UniformType type);

	public:
		int			   get_count() const;
		UniformMap&    get_uniform_map();
		UniformValues& get_uniform_values();
		std::string    get_shader_source() const;
		 
	private:	
		// Cached list of combined uniform names indexed by type.
		// Used to get the list of uniforms in the same order.
		static std::map<std::string, std::vector<std::string>> g_uniform_list;
		
		static MaterialCache g_material_cache;

		std::string m_default_image_id;
		std::string m_default_cube_map_id;
		Texture2D*  m_default_texture;

		MaterialTemplate m_template;
		unsigned int m_count;

		std::string m_name;
		std::string m_shader_source;

		bool m_translucent;
		
		std::map<std::string, Material*> m_materials;

		std::vector<UpdateMethod> m_update_methods;

		std::map<std::string, std::string> m_texture_paths;
		std::map<std::string, Texture*> m_textures;
		std::vector<LoadedImage> m_loaded_images;

		UniformMap    m_uniforms;
		UniformValues m_values;
	};

	const std::unordered_set<std::string> component_properties =
	{
		"diffuse",
		"specular",
		"ambient",
		"shininess",
		"normal",
		"emission",
		"alpha",
		"opacity",
		"refraction",
		"reflection",
		"transparent",
		"height",
		"displacement"
	};
}
