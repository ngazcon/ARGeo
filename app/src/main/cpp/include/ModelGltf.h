#pragma once

#define TINYGLTF_LOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "tiny_gltf_loader.h"

#include "Quaternion.h"
#include "Context.h"
#include "ShaderVertexAttributeLocations.h"
#include "Buffer.h"
#include "ShaderProgram.h"
#include "Texture.h"

#include <string>
#include <queue>
#include <memory>

namespace dsn
{
	class ModelGltf;
	class DrawCommand;
	class RuntimeNode;

	struct ModelMaterial
	{
		ModelMaterial(
			ModelGltf* model,
			tinygltf::Material*  material,
			std::string id)
			: id(id)
			, model(model)
			, material(material)
		{ }

		std::string id;
		ModelGltf*  model;
		tinygltf::Material*   material;
	};

	struct ModelMesh
	{
		ModelMesh(
			tinygltf::Mesh* mesh,
			std::map<std::string, ModelMaterial*> materials_by_id,
			std::string id)
			: id(id)
			, mesh(mesh)
			, materials_by_id(materials_by_id)
		{ }

		std::string id;
		tinygltf::Mesh* mesh;
		std::map<std::string, ModelMaterial*> materials_by_id;
	};

	struct ModelNode
	{
		ModelNode(
			ModelGltf* model,
			std::string id,
			tinygltf::Node* node,
			RuntimeNode* runtime_node,
			mat4d model_matrix)
			: id(id)
			, model(model)
			, node(node)
			, runtime_node(runtime_node)
			, model_matrix(model_matrix)
		{ }
		
		std::string id;
		ModelGltf* model;
		tinygltf::Node* node;
		RuntimeNode* runtime_node;
		mat4d model_matrix;
	};

	struct NodeCommand
	{
		std::unique_ptr<BoundingVolume> bounding_volume;
		std::unique_ptr<DrawCommand>    draw_command;
		std::unique_ptr<DrawCommand>    pick_command;
	};

	struct RuntimeNode
	{
		mat4d model_matrix;
		std::vector<NodeCommand*> node_commands;
		std::vector<RuntimeNode*> childrens;
		std::vector<RuntimeNode*> parents;
		ModelNode* public_node;
	};

	struct ShaderToCreate
	{
		ShaderToCreate(
			std::string id,
			std::vector<unsigned char> source,
			std::string buffer_view = "")
			: id(id)
			, source(source)
			, buffer_view(buffer_view)
		{ }

		std::string id;
		std::string buffer_view;
		std::vector<unsigned char> source;
	};

	struct TextureToCreate
	{
		TextureToCreate(
			std::string id,
			std::vector<unsigned char> image,
			std::string buffer_view = "")
			: id(id)
			, buffer_view(buffer_view)
			, image(image)
		{ }

		std::string id;
		std::string buffer_view;
		std::vector<unsigned char> image;
	};


	struct TextureFromBufferView
	{
		TextureFromBufferView(
			std::string id,
			std::string buffer_view,
			std::string mime_type)
			: id(id)
			, buffer_view(buffer_view)
			, mime_type(mime_type)
		{ }

		std::string id;
		std::string buffer_view;
		std::string mime_type;
	};

	class ModelGltf
	{
	public:
		ModelGltf(std::string filepath);
		~ModelGltf();

	private:
		void parse();
		void parse_buffers();
		void parse_buffer_views();
		void parse_materials();
		void parse_textures();
		void parse_programs();
		void parse_shaders();
		void parse_nodes();
		void parse_meshes();

		void load_textures_from_buffer_views();

		void image_load(
			std::string id,
			std::vector<unsigned char> data);
		
		void image_create_from_buffer(
			TextureFromBufferView texture_from_buffer_view,
			std::vector<unsigned char> data);

		void buffer_load(
			std::string id,
			std::vector<unsigned char> data);

		void shader_load(
			std::string id,
			std::vector<unsigned char> data);

		typedef std::string(ModelGltf::*ModifyShaderCallback)(std::string, std::string);

		std::string modify_shader(
			std::string source,
			std::string program_id,
			ModifyShaderCallback callback);


		std::string get_program_for_primitive(tinygltf::Primitive primitive);
		ShaderVertexAttributeLocations get_attribute_locations(tinygltf::Primitive primitive);
		ShaderVertexAttributeLocations create_attribute_locations(std::vector<std::string> attributes);
		std::string get_shader_source(ShaderToCreate shader);



		void create_vertex_arrays(Context* context);

		void create_buffers(Context* context);

		void create_samplers(Context* context);

		void create_texture(
			TextureToCreate texture_to_create,
			Context* context);
		void create_textures(Context* context);

		void create_programs(Context* context);
		void create_program(
			std::string id, 
			Context* context);

		static mat4d get_transform(tinygltf::Node node);

	private:
		ModifyShaderCallback m_vertex_shader_loaded;
		ModifyShaderCallback m_fragment_shader_loaded;
		ModifyShaderCallback m_pick_vertex_shader_loaded;
		ModifyShaderCallback m_pick_fragment_shader_loaded;

		std::map<std::string,  UniformMap*>  m_uniform_maps;

		std::map<std::string,  ModelMesh*> m_runtime_mesh_by_name;
		std::map<std::string, ModelMesh*> m_runtime_mesh_by_id;


		std::map<std::string,  ModelMaterial*> m_runtime_material_by_name;
		std::map<std::string, ModelMaterial*> m_runtime_material_by_id;

		std::map<std::string,  RuntimeNode*> m_runtime_nodes_by_name;
		std::map<std::string, RuntimeNode*> m_runtime_nodes_by_id;

		tinygltf::Scene* scene;

		bool m_asynchronous;
		bool m_allow_picking;
		bool m_requires_npot;
		bool m_mipmap;

		std::map<std::string, VertexBufferAttribute*> m_precreated_attributes;

		struct LoadResources
		{
			std::queue<std::string> buffers_to_create;
			unsigned int pending_buffer_loads;
			std::map<std::string, std::vector<unsigned char>> buffers;

			std::queue<std::string> programs_to_create;
			unsigned int pending_shader_loads;
			std::map<std::string, ShaderToCreate> shaders;

			std::queue<TextureToCreate> textures_to_create;
			unsigned int pending_texture_loads;

			std::queue<TextureFromBufferView> textures_to_create_from_buffer_view;
			unsigned int pending_buffer_view_to_image;

			bool createSamplers = true;
			bool createSkins = true;
			bool createRuntimeAnimations = true;
			bool createVertexArrays = true;
			bool createRenderStates = true;
			bool createUniformMaps = true;
			bool createRuntimeNodes = true;

			std::vector<unsigned char> get_buffer(tinygltf::BufferView buffer_view) 
			{
				std::vector<unsigned char> buffer = buffers[buffer_view.buffer];
				std::vector<unsigned char> sub_vector(
					buffer_view.byteLength *
					buffer_view.byteOffset);

				memcpy(
					sub_vector.data(), 
					buffer.data(),
					buffer_view.byteOffset * buffer_view.byteLength);

				return sub_vector;
			};

			//this.skinnedNodesIds = [];


			bool finished_pending_buffer_oads()
			{
				return pending_buffer_loads == 0;
			};

			bool finished_buffers_creation()
			{
				return (pending_buffer_loads == 0) && (buffers_to_create.size() == 0);
			};

			bool finished_program_creation()
			{
				return (pending_shader_loads == 0) && (programs_to_create.size() == 0);
			};

			bool finished_texture_creation()
			{
				bool finished_pending_loads     = pending_texture_loads == 0;
				bool finished_resource_creation =
					(textures_to_create.size() == 0) &&
					(textures_to_create_from_buffer_view.size() == 0);

				return finished_pending_loads && finished_resource_creation;
			};

			bool finished_everything_but_texture_creation()
			{
				bool finished_pending_loads =
					(pending_buffer_loads == 0) &&
					(pending_shader_loads == 0);

				bool finished_resource_creation =
					(buffers_to_create.size() ==  0) &&
					(programs_to_create.size() == 0) &&
					(pending_buffer_view_to_image ==  0);

				return finished_pending_loads && finished_resource_creation;
			};

			bool finished()
			{
				return finished_texture_creation() && finished_everything_but_texture_creation();
			};

		} m_load_resources;

		// Cached between models with the same url/cache-key
		struct RenderResources
		{
			std::map<std::string, std::unique_ptr<Texture>> textures;
			std::map<std::string, std::unique_ptr<TextureSampler>> samplers;
			std::map<std::string, std::unique_ptr<VertexArray>> vertex_arrays;
			std::map<std::string, std::unique_ptr<Buffer>> buffers;
			std::map<std::string, ShaderProgram*> programs;
			std::map<std::string, ShaderProgram*> pick_programs;
		} m_render_resources;



		// Look up tables for conversion.
		std::map<unsigned int, TextureFormat> texture_format
		{
			std::pair<unsigned int, TextureFormat>(TINYGLTF_TEXTURE_FORMAT_RGBA, TextureFormat::RedGreenBlueAlpha16),
		};

		std::map<unsigned int, ImageDataType> texture_image_data_type
		{
			std::pair<unsigned int, ImageDataType>(TINYGLTF_TEXTURE_TYPE_UNSIGNED_BYTE, ImageDataType::UnsignedByte),
		};

		std::map<unsigned int, unsigned int> components_per_attribute
		{
			std::pair<unsigned int, unsigned int>(TINYGLTF_TYPE_SCALAR, 1),
			std::pair<unsigned int, unsigned int>(TINYGLTF_TYPE_VEC2,   2),
			std::pair<unsigned int, unsigned int>(TINYGLTF_TYPE_VEC3,   3),
			std::pair<unsigned int, unsigned int>(TINYGLTF_TYPE_VEC4,   4),
			std::pair<unsigned int, unsigned int>(TINYGLTF_TYPE_MAT2,   4),
			std::pair<unsigned int, unsigned int>(TINYGLTF_TYPE_MAT3,   9),
			std::pair<unsigned int, unsigned int>(TINYGLTF_TYPE_MAT4,   16)
		};

		std::map<unsigned int, ComponentDataType> components_data_type
		{
			std::pair<unsigned int, ComponentDataType>(TINYGLTF_COMPONENT_TYPE_BYTE,           ComponentDataType::Byte),
			std::pair<unsigned int, ComponentDataType>(TINYGLTF_COMPONENT_TYPE_FLOAT,          ComponentDataType::Float),
			std::pair<unsigned int, ComponentDataType>(TINYGLTF_COMPONENT_TYPE_INT,            ComponentDataType::Int),
			std::pair<unsigned int, ComponentDataType>(TINYGLTF_COMPONENT_TYPE_SHORT,          ComponentDataType::Short),
			std::pair<unsigned int, ComponentDataType>(TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE,  ComponentDataType::UnsignedByte),
			std::pair<unsigned int, ComponentDataType>(TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT,   ComponentDataType::UnsignedInt),
			std::pair<unsigned int, ComponentDataType>(TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT, ComponentDataType::UnsignedShort),
		};
	};
}
