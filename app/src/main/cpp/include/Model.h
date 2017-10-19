#pragma once


#include "HeightClamping.h"
#include "OrientationClamping.h"

#include "VertexArray.h"

#include "ModelNode.h"
#include "ModelMaterial.h"
#include "ModelMesh.h"

#include "ShaderProgram.h"
#include "DrawCommand.h"
#include "Texture.h"
#include "BoundingVolume.h"
#include "ShadowMode.h"

#include "IPrimitive.h"

#include "PickableNode.h"
#include "Geodetic3D.h"
#include "Geocentric3D.h"
#include "CommonShaders.h"
#include "RasterSourceChangedListener.h"
#include "Axis.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include <memory>
#include <queue>
#include <vector>
#include <set>

#include <map>


namespace argeo
{
	enum class ModelState : unsigned int
	{
		NeedsLoad = 0,
		Loading = 1,
		Loaded = 2, // Renderable, but textures can still be pending.
		Failed = 3
	};

	struct TextureToCreate
	{
		TextureToCreate(
			std::string id,
			std::vector<unsigned char> image,
			unsigned int width,
			unsigned int height)
			: id(id)
			, image(image)
			, width(width)
			, height(height)
		{ }

		TextureToCreate(
			std::string id,
			std::string path)
			: id(id)
			, path(path)
		{ }

		std::string id;
		std::string path;

		std::vector<unsigned char> image;
		unsigned int width;
		unsigned int height;
	};

	//Not many file formats embed their textures into the file.An example is Quake's MDL format 
	// (which is also used by some GameStudio versions) 
	struct EmbebbedTextureToLoad
	{
		EmbebbedTextureToLoad(
			std::string id,
			unsigned int width,
			unsigned int height)
			: id(id)
			, width(width)
			, height(height)
		{ }

		std::string id;
		unsigned int width;
		unsigned int height;
	};

	struct RenderStateToLoad
	{
		RenderStateToLoad(
			std::string id,
			RenderStateParameters parameters)
			: id(id)
			, parameters(parameters)
		{ }

		std::string id;
		RenderStateParameters parameters;
	};

	struct MeshToLoad
	{
		MeshToLoad(std::string id)
			: id(id)
		{ }

		std::string id;
	};

	struct ShaderProgramToCreate
	{
		ShaderProgramToCreate(
			std::string id,
			std::string vertex_shader,
			std::string fragment_shader)
			: id(id)
			, vertex_shader(vertex_shader)
			, fragment_shader(fragment_shader)
		{ }

		std::string id;
		std::string vertex_shader;
		std::string fragment_shader;
	};

	struct ModelNodeCommand
	{
		std::unique_ptr<BoundingVolume> bounding_volume;

		std::unique_ptr<DrawCommand>    outline_command;
		std::unique_ptr<DrawCommand>    draw_command;
        std::unique_ptr<DrawCommand>    draw_command_2D;

		std::unique_ptr<DrawCommand>    pick_command;
        std::unique_ptr<DrawCommand>    pick_command_2D;

	};

	struct RuntimeModelNode
	{
		mat4d model_matrix;

		std::vector<ModelNodeCommand*> node_commands;
		std::vector<RuntimeModelNode*> childrens;
		std::vector<RuntimeModelNode*> parents;

		ModelNode* public_node;
		PickableNode* pickable_node;
	};

	class Scene;
	class Model :
		public IPrimitive,
		public RasterSourceChangedListener
	{
		friend class ModelNode;

	public:
		Model(
			std::string filepath,
			std::string name,
			Scene* scene,
			bool normalize_positions = false,
			HeightClamping height_reference = HeightClamping::None,
			OrientationClamping orientation_reference = OrientationClamping::None);
		~Model();

		void on_raster_source_changed(RasterSource* value);

		void update(FrameState* frame_state);

		void set_translation(vec3d position);

		void translate(vec3d translation);

		void set_euler_angles(
			double pitch,
			double yaw,
			double roll);

		void set_translation(
			double x,
			double y,
			double z);

		void translate(
			double x,
			double y,
			double z);

		void set_rotation(quaternion rotation);
		void rotate(quaternion rotation);

        double get_scale() const;
		void set_scale(const double& value);

		quaternion get_rotation() const;
		vec3d      get_translation() const;


		mat4d get_model_matrix();

        bool get_show() const;
		void set_show(bool value);

		void set_shadow_mode(const ShadowMode& mode);
		ShadowMode get_shadow_mode() const;

		std::string get_name() const;
		void set_name(std::string name);

		void set_height_clamping(HeightClamping value);
		HeightClamping get_height_clamping() const;

		void set_orientation_clamping(OrientationClamping value);
		OrientationClamping get_orientation_clamping() const;

        BoundingSphere* get_bounding_sphere();

		bool get_allow_picking() const;
		void set_allow_picking(bool value);

		double get_outline_stroke() const;
		void set_outline_stroke(const double& value);

	private:
        double get_scale(FrameState* frame_state);
        double scale_in_pixels(
                const vec3d& position_wc,
                const double& radius,
                FrameState* frame_state);

		void parse_meshes();
		ModelMesh* parse_mesh(
			std::string id,
			const aiMesh* ai_mesh);

		void parse_nodes();
		ModelNode* parse_node(
			const aiNode* ai_node);


		bool assimp_matkey_equals(
			const char* char1,
			const char* char2,
			unsigned int a,
			unsigned int b);


		void parse();
		void parse_programs();
		void parse_program(ModelMesh* model_mesh);

		void parse_shaders();
		void parse_shader(ModelMesh* model_mesh);

		void parse_embebbed_textures();

		void parse_render_states();
		void parse_render_state(ModelMesh* model_mesh);

		void parse_materials();
		ModelMaterial* parse_material(
			std::string id,
			const aiMaterial* ai_material);

		void parse_material_texture(
			std::string id,
			const aiTextureType ai_texture_type,
			const unsigned int ai_texture_index,
			const aiMaterial* ai_material);

		std::string parse_material_name(const char* ai_key);
		std::string parse_material_component_key(const char* ai_key);

		std::string parse_texture_value(
			const aiTextureType ai_texture_type,
			const unsigned int ai_texture_index);

		std::string parse_texture_channels(
			const aiTextureType ai_texture_type);

		std::string parse_texture_component_key(
			const aiTextureType ai_texture_type);

		std::string parse_texture_name(
			const aiTextureType ai_texture_type,
			const unsigned int ai_texture_index);

	private:

		// Resources
		void create_resources(FrameState* frame_state);

		void load_embebbed_texture(EmbebbedTextureToLoad embebbed_texture_to_load);
		void load_embebbed_textures();
		void texture_load_thread_entry_point();

		void create_textures(Context* context);
		void create_texture(TextureToCreate texture_to_create);

		void create_programs(Context* context);
		void create_program(
			Context* context,
			ShaderProgramToCreate program_to_create);
		void program_load_thread_entry_point();

		std::string get_shader_program_id(
			aiMesh* ai_mesh,
			aiMaterial* ai_material);

		std::string get_vertex_shader_id(
			aiMesh* ai_mesh,
			aiMaterial* ai_material);

		std::string get_fragment_shader_id(
			aiMesh* ai_mesh,
			aiMaterial* ai_material);

		std::string modify_vertex_shader(
			std::string vertex_shader_source,
			aiMesh* ai_mesh,
			aiMaterial* ai_material);

		std::string modify_fragment_shader(
			std::string fragment_shader_source,
			aiMesh* ai_mesh,
			aiMaterial* ai_material);

		std::string create_uniforms_declaration(
			std::string shader_source,
			aiMaterial* ai_material);

		std::string create_material_method_definition(
			std::string shader_source,
			aiMaterial* ai_material);

		bool get_material_component_declaration(
			const aiMaterial* ai_material,
			const aiMaterialProperty* ai_property,
			std::string& component,
			std::string& declaration);

		bool get_material_texture_uniform(
			const aiTextureType ai_texture_type,
			const unsigned int ai_texture_index,
			const aiMaterial* ai_material,
			UniformFunction& uniform_function,
			IUniformValue*   uniform_value,
			std::string& id);

		bool get_material_texture_uniform_declaration(
			const aiTextureType ai_texture_type,
			const unsigned int ai_texture_index,
			const aiMaterial* ai_material,
			std::string& uniform_declaration,
			std::string& id);

		bool get_material_value_uniform(
			const char* ai_key,
			const aiMaterial* ai_material,
			UniformFunction& uniform_function,
			IUniformValue*   uniform_value,
			std::string& id);

		bool get_material_value_uniform_declaration(
			const char* ai_key,
			const aiMaterial* ai_material,
			std::string& uniform_declaration,
			std::string& id);

		void create_runtime_nodes(
                Context* context,
                const bool& scene_3D_only);

		RuntimeModelNode* create_runtime_node(
			ModelNode* model_node,
			RuntimeModelNode* parent_node,
			Context* context,
            const bool& scene_3D_only);

		void create_vertex_array(MeshToLoad mesh_to_load);
		void create_meshes(Context* context);
		void mesh_load_thread_entry_point();

		void create_render_states(Context* context);
		void create_render_state(
			RenderStateToLoad render_state_to_load,
			Context* context);

		std::string get_render_state_id(
			aiMesh* ai_mesh,
			aiMaterial* ai_material);

		RenderStateParameters get_render_state_parameters(
			aiMesh* ai_mesh,
			aiMaterial* ai_material);

		void create_uniform_maps(Context* context);
		void create_command(
                RuntimeModelNode* runtime_node,
                Context* context,
                const bool& scene_3D_only);

		void create_vertex_arrays(Context* context);

		void push_commands(FrameState* frame_state);

	private:
		void calculate_bounding_volume();
		void update_nodes_model_matrix(
			const bool& just_loaded,
			const bool& model_matrix_changed,
            MapProjection* projection);

		void update_model_matrix();
		void update_clamping();
		void update_shadows();
		void model_matrix_changed();

	private:
		bool m_mipmaps;

		SceneMode m_mode;
		HeightClamping m_height_clamping;
		OrientationClamping m_orientation_clamping;

		std::string  m_name;

		Texture2D* m_default_texture2D;

		struct LoadResources
		{
			std::queue<RenderStateToLoad> render_states_to_load;
			std::set<std::string> unique_render_state_ids;

			std::queue<MeshToLoad> meshes_to_load;

			std::queue<EmbebbedTextureToLoad> embebbed_textures_to_create;
			std::queue<TextureToCreate> textures_to_create;
			std::set<std::string> unique_texture_ids;

			std::queue<ShaderProgramToCreate> programs_to_create;
			std::set<std::string> unique_program_ids;
			std::map<std::string, std::string> shaders;

			bool create_vertex_arrays = true;
			bool create_render_states = true;
			bool create_uniform_maps = true;
			bool create_runtime_nodes = true;

			bool finished_pending_meshes_loads()
			{
				return meshes_to_load.empty();
			};

			bool finished_program_creation()
			{
				return programs_to_create.empty();
			};

			bool finished_render_state_creation()
			{
				return render_states_to_load.empty();
			}

			bool finished_texture_creation()
			{
				return
					textures_to_create.empty() &&
					embebbed_textures_to_create.empty();
			};

			bool finished_everything_but_texture_creation()
			{
				return
					finished_render_state_creation() &&
					finished_pending_meshes_loads() &&
					finished_program_creation() &&
					embebbed_textures_to_create.empty();
			};

			bool finished()
			{
				return finished_texture_creation() && finished_everything_but_texture_creation();
			};

		} m_load_resources;

		// Resources
		struct RenderResources
		{
			std::map<std::string, Texture*>     textures_by_path;
			std::map<std::string, VertexArray*> vertex_arrays;
			std::map<std::string, Mesh*>        meshes_by_id;

			std::map<std::string, RenderState*> render_states_by_id;
			std::map<std::string, RenderState*> outline_render_states_by_id;

			std::map<std::string, PickId*>        pick_ids;
			std::map<std::string, ShaderProgram*> outline_shader_programs_by_id;
			std::map<std::string, ShaderProgram*> pick_shader_programs_by_id;
			std::map<std::string, ShaderProgram*> shader_programs_by_id;

			std::map<std::string, UniformMap*>    uniform_maps_by_material_id;
			std::map<std::string, UniformMap*>    pick_uniform_maps_by_material_id;
			std::map<std::string, UniformMap*>    outline_uniform_maps_by_material_id;

		} m_render_resources;

		std::vector<RuntimeModelNode*>			  m_runtime_root_nodes;
		std::map<std::string, RuntimeModelNode*>  m_runtime_nodes_by_name;
		std::vector<ModelNode*>					  m_model_root_nodes;
		std::map<std::string, ModelMesh*>		  m_model_meshes_by_name;
		std::map<std::string, ModelMesh*>		  m_model_meshes_by_id;
		std::map<std::string, ModelMaterial*>	  m_model_materials_by_name;
		std::map<std::string, ModelMaterial*>	  m_model_materials_by_id;


		ModelState m_state;

		bool  m_mesh_loading;
		bool  m_texture_loading;
		bool  m_program_loading;
		bool  m_incrementally_load_textures;
		bool  m_asynchronous;
		bool  m_just_loaded;
		bool  m_height_changed;
		bool  m_show;
		bool  m_model_matrix_changed;
		bool  m_children_model_matrix_changed;

		bool m_has_outline;
		bool m_allow_picking;

		bool m_normalize_positions;
		bool m_center;

		double m_outline_stroke;

		mat4d m_model_matrix;
		mat4d m_clamped_model_matrix;
        mat4d m_computed_model_matrix;

		quaternion m_rotation;
		vec3d      m_translation;

        double m_scale;
        double m_maximum_scale;
        double m_minimum_pixel_size;

        std::function<void()> m_remove_update_height_callback;
        std::function<void(vec3d&)> m_update_height_function;

		quaternion m_orientation;
		vec3d      m_position;
		vec3d      m_clamped_position;

        vec3d      m_rtc_center_eye;
        vec3d      m_rtc_center;
        vec3d      m_rtc_center_2D;
        vec3d      m_rtc_center_3D;

		Axis       m_up_axis;
		ShadowMode m_shadow_mode;
		ShadowMode m_current_shadow_mode;

		Scene*  m_scene;
        std::unique_ptr<BoundingSphere> m_bounding_sphere;
        std::unique_ptr<BoundingSphere> m_scaled_bounding_sphere;

        double m_initial_radius;

		std::string m_filepath;

		const aiScene* m_ai_scene;
		Assimp::Importer m_importer;

	};

	const std::map<aiShadingMode, std::string> shading_model_vertex_sources =
	{
		{ aiShadingMode_Blinn, common_vs },
	};

	const std::map<aiShadingMode, std::string> shading_model_fragment_sources =
	{
		{ aiShadingMode_Blinn, common_fs },
	};
}

