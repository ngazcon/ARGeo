#include "ModelGltf.h"

#include "DrawCommand.h"
#include "Device.h"
#include "ShaderCache.h"
#include "TextureHelper.h"

#include "ShaderObjectGL.h"

#include <stdexcept>

namespace dsn
{
	ModelGltf::ModelGltf(std::string filepath)
	{
		tinygltf::TinyGLTFLoader loader;
		std::string    err;

		bool ret = loader.LoadASCIIFromFile(
			scene,
			&err, 
			filepath);

		if (!err.empty()) 
		{
			throw new std::runtime_error(
				"Err: " + err);
		}

		if (!ret) 
		{
			throw new std::runtime_error(
				"Failed to parse glTF\n");
		}
	}


	ModelGltf::~ModelGltf()
	{ }

	void ModelGltf::buffer_load(
		std::string id,
		std::vector<unsigned char> data)
	{
		m_load_resources.buffers[id] = data;
		m_load_resources.pending_buffer_loads--;
	}

	void ModelGltf::parse_buffers()
	{
		auto buffers = scene->buffers;
		for (auto entry : buffers)
		{
			std::string id = entry.first;
			tinygltf::Buffer buffer  = buffers[id];

			// The extension 'KHR_binary_glTF' uses a special buffer entitled just 'binary_glTF'.
			// The 'KHR_binary_glTF' check is for backwards compatibility for the Cesium model converter
			// circa Cesium 1.15-1.20 when the converter incorrectly used the buffer name 'KHR_binary_glTF'.
			//if ((id == "binary_glTF") || (id == "KHR_binary_glTF"))
			{
				// Buffer is the binary glTF file itself that is already loaded
				//loadResources.buffers[id] = model._cachedGltf.bgltf;
			}
			//else if (buffer.type == "arraybuffer")
			{
				m_load_resources.pending_buffer_loads++;
				buffer_load(
					id, 
					buffer.data);
			}
		}
	}

	void ModelGltf::parse_buffer_views()
	{
		auto buffer_views = scene->bufferViews;
		for (auto entry : buffer_views)
		{
			std::string id = entry.first;
			if (buffer_views[id].target == TINYGLTF_TARGET_ARRAY_BUFFER)
			{
				m_load_resources.buffers_to_create.push(id);
			}
		}
	}

	void ModelGltf::shader_load(
		std::string id,
		std::vector<unsigned char> data)
	{
		m_load_resources.shaders[id] = ShaderToCreate(
			id,
			data);
		m_load_resources.pending_shader_loads--;
	}

	void ModelGltf::parse_shaders()
	{
		auto shaders = scene->shaders;
		for (auto entry : shaders)
		{
			std::string id = entry.first;
			tinygltf::Shader shader  = shaders[id];

			m_load_resources.pending_shader_loads++;
			shader_load(
				id,
				shader.source);
		}
	}

	void ModelGltf::parse_programs()
	{
		auto programs = scene->programs;
		for (auto id : programs) 
		{
			m_load_resources.programs_to_create.push(id.first);
		}
	}

	void ModelGltf::image_load(
		std::string id,
		std::vector<unsigned char> data)
	{
		m_load_resources.pending_texture_loads--;
		m_load_resources.textures_to_create.push(TextureToCreate(
			id,
			data));
	}

	void ModelGltf::parse_textures()
	{
		auto images   = scene->images;
		auto textures = scene->textures;

		for (auto entry : textures)
		{
			std::string id = entry.first;
			tinygltf::Image image    = images[textures[id].source];

			// Image references either uri (external or base64-encoded) or bufferView
			if (!image.bufferView.empty() && !image.mimeType.empty()) 
			{
				m_load_resources.textures_to_create_from_buffer_view.push(
					TextureFromBufferView(
						id,
						image.bufferView,
						image.mimeType));
			}
			else
			{
				m_load_resources.pending_texture_loads++;
				image_load(id, image.image);
			}
		}
	}


	 void ModelGltf::parse_nodes() 
	 {
		 m_runtime_nodes_by_id.clear();
		 m_runtime_nodes_by_name.clear();
        
		//var skinnedNodes = [];
        //var skinnedNodesIds = model._loadResources.skinnedNodesIds;

        auto nodes = scene->nodes;

        for (auto entry : nodes) 
		{
			std::string id = entry.first;
			tinygltf::Node node      = nodes[id];

			RuntimeNode* runtime_node = new RuntimeNode();
			runtime_node->public_node = new ModelNode(
				this, 
				id,
				&node, 
				runtime_node,				
				get_transform(node));

			m_runtime_nodes_by_id[id]          = runtime_node;
			m_runtime_nodes_by_name[node.name] = runtime_node;


            //if (defined(node.skin)) {
            //    skinnedNodesIds.push(id);
            //    skinnedNodes.push(runtimeNode);
            //}
        }
    }

	 void ModelGltf::parse_materials()
	 {
		 m_runtime_material_by_id.clear();
		 m_runtime_material_by_name.clear();

		 auto materials   = scene->materials;
		 for (auto entry : materials)
		 {
			 std::string id = entry.first;

			 // Allocated now so ModelMaterial can keep a reference to it.
			 m_uniform_maps[id] = new UniformMap();

			 tinygltf::Material material   = materials[id];
			 ModelMaterial* model_material = new ModelMaterial(this, &material, id);

			 m_runtime_material_by_name[material.name] = model_material;
			 m_runtime_material_by_id[id]			   = model_material;
		 }
	 }

	 void ModelGltf::parse_meshes()
	 {
		 m_runtime_mesh_by_name.clear();
		 m_runtime_mesh_by_id.clear();

		 auto meshes = scene->meshes;
		 for (auto entry : meshes)
		 {
			 std::string id		 = entry.first;
			 tinygltf::Mesh mesh = meshes[id];

			 m_runtime_mesh_by_name[mesh.name] = new ModelMesh(
				 &mesh,
				 m_runtime_material_by_id,
				 id);

			 //if (usesQuantizedAttributes)
			 //{
			 //	 // Cache primitives according to their program
			 //	 var primitives = mesh.primitives;
			 //	 var primitivesLength = primitives.length;
			 //	 for (var i = 0; i < primitivesLength; i++)
			 //	 {
			 //		 var primitive = primitives[i];
			 //		 var programId = getProgramForPrimitive(model, primitive);
			 //		 var programPrimitives = model._programPrimitives[programId];
			 //		 if (!defined(programPrimitives))
			 //		 {
			 //			 programPrimitives = [];
			 //			 model._programPrimitives[programId] = programPrimitives;
			 //		 }
			 //		 programPrimitives.push(primitive);
			 //	 }
		 }
	 }

	 void ModelGltf::parse() 
	 {
		 //if (!model._loadRendererResourcesFromCache) 
		 {
			 parse_buffers();
			 parse_buffer_views();
			 parse_shaders();
			 parse_programs();
			 parse_textures();
		 }

		 parse_materials();
		 parse_meshes();
		 parse_nodes();
	 }

	 mat4d ModelGltf::get_transform(tinygltf::Node node)
	 {
		 return mat4d::identity();
		 
		//mat_translate(
		//	 node.translation[0],
		//	 node.translation[1],
		//	 node.translation[2]) * 
		//	 mat4d(quat_to_matrix(
		//		 quaternion(
		//			 node.rotation[0],
		//			 node.rotation[1],
		//			 node.rotation[2],
		//			 node.rotation[3])))
		//	 * mat_scale(
		//		 node.scale[0],
		//		 node.scale[1],
		//		 node.scale[2]);
	 }

	 ///////////////////////////////////////////////////////////////////////////

	 void ModelGltf::create_buffers(Context* context)
	 {
		 if (m_load_resources.pending_buffer_loads != 0) 
		 {
			 return;
		 }

		 tinygltf::BufferView buffer_view;
		 auto buffer_views   = scene->bufferViews;

		 while (m_load_resources.buffers_to_create.size() > 0)
		 {
			 std::string id = m_load_resources.buffers_to_create.front();
			 m_load_resources.buffers_to_create.pop();
			 buffer_view    = buffer_views[id];

			 // Only ARRAY_BUFFER here.  
			 // ELEMENT_ARRAY_BUFFER created below.
			std::unique_ptr<VertexBuffer> buffer = Device::create_vertex_buffer(
				 m_load_resources.get_buffer(buffer_view),
				 BufferHint::StaticDraw);

			 //vertexBuffer.vertexArrayDestroyable = false;
			 m_render_resources.buffers[id] = std::move(buffer);
		 }

		 // The Cesium Renderer requires knowing the datatype for an index buffer
		 // at creation type, which is not part of the glTF bufferview so loop
		 // through glTF accessors to create the bufferview's index buffer.
		 auto accessors = scene->accessors;
		 for (auto entry : accessors) 
		 {
			 std::string id = entry.first;

			 tinygltf::Accessor accessor = accessors[id];
			 buffer_view	    = buffer_views[accessor.bufferView];

			if ((buffer_view.target == TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER) &&
				(m_render_resources.buffers[accessor.bufferView]))
			{
				std::vector<unsigned char> buffer = m_load_resources.get_buffer(buffer_view);
				int size_in_bytes = buffer.size() * sizeof(unsigned char);
				
				std::unique_ptr<IndexBuffer> index_buffer = Device::create_index_buffer(
					BufferHint::StaticDraw,
					size_in_bytes);

				if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
				{
					index_buffer->copy_from_system(
						reinterpret_cast<unsigned short*>(buffer.data()),
						0, 
						size_in_bytes);
				}
				else
				{
					index_buffer->copy_from_system(
						reinterpret_cast<unsigned int*>(buffer.data()),
						0,
						size_in_bytes);
				}
				
				m_render_resources.buffers[accessor.bufferView] = std::move(index_buffer);
				// In theory, several glTF accessors with different componentTypes could
				// point to the same glTF bufferView, which would break this.
				// In practice, it is unlikely as it will be UNSIGNED_SHORT.
			}
		 }
	 }

	 ShaderVertexAttributeLocations ModelGltf::create_attribute_locations(
		 std::vector<std::string> attributes) 
	 {
		 ShaderVertexAttributeLocations attribute_locations;
		 int length = attributes.size();

		 // Set the position attribute to the 0th index. In some WebGL implementations the shader
		 // will not work correctly if the 0th attribute is not active. For example, some glTF models
		 // list the normal attribute first but derived shaders like the cast-shadows shader do not use
		 // the normal attribute.
		 for (int i = 1; i < length; ++i)
		 {
			 std::string attribute = attributes[i];
			 if (std::strcmp("position", attribute.c_str()) == 0) 
			 {
				 attributes[i] = attributes[0];
				 attributes[0] = attribute;
				 break;
			 }
		 }

		 for (int i = 0; i < length; ++i) 
		 {
			 attribute_locations[attributes[i]] = i;
		 }

		 return attribute_locations;
	 }

	 std::string ModelGltf::get_shader_source(ShaderToCreate shader) 
	 {
		 if (!shader.source.empty()) 
		 {
			 return std::string(reinterpret_cast<const char*>(
				 shader.source.data()));
		 }

		 tinygltf::BufferView buffer_view = scene->bufferViews[shader.buffer_view];
		 return std::string(reinterpret_cast<const char*>(
			 m_load_resources.get_buffer(buffer_view).data()));
	 }

	 std::string ModelGltf::modify_shader(
		 std::string source,
		 std::string program_id,
		 ModifyShaderCallback callback)
	 {
		 std::string modified_source = source;

		 if (callback != nullptr)
		 {
			 modified_source = (this->*callback)(
				 source,
				 program_id);
		 }

		 return modified_source;
	 }

	 void ModelGltf::create_program(
		 std::string id,
		 Context* context)
	 {
		 auto programs   = scene->programs;
		 auto shaders    = m_load_resources.shaders;
		 tinygltf::Program program = programs[id];

		 ShaderVertexAttributeLocations attribute_locations = 
			 create_attribute_locations(program.attributes);

		 std::string vs = get_shader_source(shaders[program.vertexShader]);
		 std::string fs = get_shader_source(shaders[program.fragmentShader]);

		 // Add pre-created attributes to attributeLocations
		 int attributes_length  = program.attributes.size();
		 for (auto attribute : m_precreated_attributes)
		 {
			 std::string attribute_name = attribute.first;
			 attribute_locations[attribute_name] = attributes_length++;
		 }
		 
		 std::string drawVS = modify_shader(vs, id, m_vertex_shader_loaded);
		 std::string drawFS = modify_shader(fs, id, m_fragment_shader_loaded);

		 m_render_resources.programs[id] = ShaderCache::get_shader_program(
			 drawVS,
			 drawFS,
			 attribute_locations);

		 if (m_allow_picking) 
		 {
			 // PERFORMANCE_IDEA: Can optimize this shader with a glTF hint. 
			 // https://github.com/KhronosGroup/glTF/issues/181
			 std::string pickVS = modify_shader(vs, id, m_pick_vertex_shader_loaded);
			 std::string pickFS = modify_shader(fs, id, m_pick_fragment_shader_loaded);

			 if (m_pick_fragment_shader_loaded == nullptr)
			 {
				 pickFS = ShaderObjectGL::create_fragment_pick_shader_source(fs, "uniform");
			 }

			 m_render_resources.pick_programs[id] = ShaderCache::get_shader_program(
				 pickVS,
				 pickFS,
				 attribute_locations);
		 }
	 }

	 void ModelGltf::create_programs(Context* context) 
	 {
		 std::string id;

		 if (m_load_resources.pending_shader_loads != 0)
		 {
			 return;
		 }

		 // PERFORMANCE_IDEA: this could be more fine-grained by looking
		 // at the shader's bufferView's to determine the buffer dependencies.
		 if (m_load_resources.pending_buffer_loads != 0)
		 {
			 return;
		 }

		 if (m_asynchronous)
		 {
			 // Create one program per frame
			 if (m_load_resources.programs_to_create.size() > 0)
			 {
				 id = m_load_resources.programs_to_create.front();
				 m_load_resources.programs_to_create.pop();
				 create_program(id, context);
			 }
		 }
		 else 
		 {
			 // Create all loaded programs this frame
			 while (m_load_resources.programs_to_create.size() > 0)
			 {
				 id = m_load_resources.programs_to_create.front();
				 m_load_resources.programs_to_create.pop();
				 create_program(id, context);
			 }
		 }
	 }

	 std::string ModelGltf::get_program_for_primitive(
		 tinygltf::Primitive primitive)
	 {
		 std::string material_id	   = primitive.material;
		 tinygltf::Material material   = scene->materials[material_id];
		 std::string technique_id      = material.technique;
		 tinygltf::Technique technique = scene->techniques[technique_id];
		 return technique.program;
	 }

	 ShaderVertexAttributeLocations ModelGltf::get_attribute_locations(tinygltf::Primitive primitive)
	 {
		 auto techniques = scene->techniques;
		 auto materials  = scene->materials;

		 // Retrieve the compiled shader program to assign index values to attributes
		 ShaderVertexAttributeLocations attribute_locations;

		 tinygltf::Technique technique  = techniques[materials[primitive.material].technique];
		 auto parameters = technique.parameters;
		 auto attributes = technique.attributes;

		 ShaderVertexAttributeCollection* program_attributes = 
			 m_render_resources.programs[technique.program]->get_attributes();

		 // Note: WebGL shader compiler may have optimized and removed some attributes from programAttributeLocations
		 for (auto entry : *program_attributes)
		 {
			 std::string attribute = attributes[entry.first];
			 int index = entry.second->get_location();

			 tinygltf::TechniqueParameter parameter = parameters[attribute];
			 attribute_locations[parameter.semantic] = index;
		 }

		 return attribute_locations;
	 }


	 void ModelGltf::load_textures_from_buffer_views() 
	 {
		 if (m_load_resources.pending_buffer_loads != 0) 
		 {
			 return;
		 }

		 while (m_load_resources.textures_to_create_from_buffer_view.size() > 0)
		 {
			 TextureFromBufferView texture_from_buffer_view =
				 m_load_resources.textures_to_create_from_buffer_view.front();
			 m_load_resources.textures_to_create_from_buffer_view.pop();

			 tinygltf::BufferView buffer_view = scene->bufferViews[texture_from_buffer_view.buffer_view];

			 std::vector<unsigned char> buffer = m_load_resources.get_buffer(buffer_view);
			 image_create_from_buffer(texture_from_buffer_view, buffer);

			 m_load_resources.pending_buffer_view_to_image++;
		 }
	 }

	 void ModelGltf::create_samplers(Context* context) 
	 {
		 //if (m_load_resources.createSamplers) 
		 //{
		//	 m_load_resources.createSamplers = false;
		 //
		//	 auto samplers = scene->samplers;
		//	 for (auto entry : samplers)
		//	 {
		//		 std::string id  = entry.first;
		//		 Sampler sampler = samplers[id];
		 //
		//		 rendererSamplers[id] = new Sampler({
		//		 wrapS: sampler.wrapS,
		//				wrapT : sampler.wrapT,
		//						minificationFilter : sampler.minFilter,
		//											 magnificationFilter : sampler.magFilter
		//		 });
		//	 }
		 //}
	 }

	 void ModelGltf::create_texture(
		 TextureToCreate texture_to_create, 
		 Context* context) 
	 {
		 auto textures = scene->textures;
		 tinygltf::Texture texture = textures[texture_to_create.id];

		 //var sampler = rendererSamplers[texture.sampler];

		// var mipmap =
		//	 (sampler.minificationFilter == = TextureMinificationFilter.NEAREST_MIPMAP_NEAREST) ||
		//	 (sampler.minificationFilter == = TextureMinificationFilter.NEAREST_MIPMAP_LINEAR) ||
		//	 (sampler.minificationFilter == = TextureMinificationFilter.LINEAR_MIPMAP_NEAREST) ||
		//	 (sampler.minificationFilter == = TextureMinificationFilter.LINEAR_MIPMAP_LINEAR);
		// var requiresNpot = mipmap ||
		//	 (sampler.wrapS == = TextureWrap.REPEAT) ||
		//	 (sampler.wrapS == = TextureWrap.MIRRORED_REPEAT) ||
		//	 (sampler.wrapT == = TextureWrap.REPEAT) ||
		//	 (sampler.wrapT == = TextureWrap.MIRRORED_REPEAT);

		 std::vector<unsigned char> source = texture_to_create.image;
		 bool npot = !TextureHelper::is_power_of_two(source.size());

		 //var npot = !CesiumMath.isPowerOfTwo(source.width) || !CesiumMath.isPowerOfTwo(source.height);

		 if (m_requires_npot && npot)
		 {
			 // WebGL requires power-of-two texture dimensions for mipmapping and REPEAT/MIRRORED_REPEAT 
			 // wrap modes.

			 //var canvas = document.createElement('canvas');
			 //canvas.width = CesiumMath.nextPowerOfTwo(source.width);
			 //canvas.height = CesiumMath.nextPowerOfTwo(source.height);
			 //var canvasContext = canvas.getContext('2d');
			 //canvasContext.drawImage(source, 0, 0, source.width, source.height, 0, 0, canvas.width, canvas.height);
			 //source = canvas;
		 }

		 std::unique_ptr<Texture> tx;

		 if (texture.target == TINYGLTF_TEXTURE_TARGET_TEXTURE2D)
		 {
			 tx = Device::create_texture2D(Texture2DDescription(
				 1,
				 1,
				 TextureFormat::RedGreenBlueAlpha8,
				 m_mipmap));

			 //tx = new Texture({
			 //context: context,
			//		  source : source,
			//				   pixelFormat : texture.internalFormat,
			//								 pixelDatatype : texture.type,
			//												 sampler : sampler,
			//														   flipY : false
			 //});
		 }
		 // GLTF_SPEC: Support TEXTURE_CUBE_MAP. 
		 // https://github.com/KhronosGroup/glTF/issues/40

		 m_render_resources.textures[texture_to_create.id] = std::move(tx);
	 }

	 void ModelGltf::create_textures(Context* context) 
	 {
		 if (m_asynchronous) 
		 {
			 // Create one texture per frame
			 if (m_load_resources.textures_to_create.size() > 0) 
			 {
				 TextureToCreate texture_to_create = m_load_resources.textures_to_create.front();
				 m_load_resources.textures_to_create.pop();
				 create_texture(texture_to_create, context);
			 }
		 }
		 else
		 {
			 // Create all loaded textures this frame
			 while (m_load_resources.textures_to_create.size() > 0)
			 {
				 TextureToCreate texture_to_create = m_load_resources.textures_to_create.front();
				 m_load_resources.textures_to_create.pop();
				 create_texture(texture_to_create, context);
			 }
		 }
	 }


	 void ModelGltf::image_create_from_buffer(
		 TextureFromBufferView texture_from_buffer_view,
		 std::vector<unsigned char> data)
	 {
		 m_load_resources.textures_to_create.push(TextureToCreate(
			 texture_from_buffer_view.id,
			 data));

		 m_load_resources.pending_buffer_view_to_image--;
	 }
		
	 void ModelGltf::create_vertex_arrays(Context* context) 
	 {
		 if (!m_load_resources.finished_buffers_creation() || !m_load_resources.finished_program_creation())
		 {
			 return;
		 }

		 if (!m_load_resources.createVertexArrays)
		 {
			 return;
		 }

		 m_load_resources.createVertexArrays = false;

		 auto accessors = scene->accessors;
		 auto meshes    = scene->meshes;

		 MeshBuffers mesh_buffers;

		 for (auto entry : meshes)
		 {
			 std::string mesh_id = entry.first;

			 std::vector<tinygltf::Primitive> primitives = meshes[mesh_id].primitives;
			 int primitives_length = primitives.size();

			 for (int i = 0; i < primitives_length; ++i)
			 {
				 tinygltf::Primitive primitive = primitives[i];

				 // GLTF_SPEC: This does not take into account attribute arrays,
				 // indicated by when an attribute points to a parameter with a
				 // count property.
				 //
				 // https://github.com/KhronosGroup/glTF/issues/258

				 ShaderVertexAttributeLocations attribute_locations = get_attribute_locations(primitive);

				 int  attribute_location;
				 std::string attribute_name;
				 auto primitive_attributes = primitive.attributes;

				 for (auto entry : primitive_attributes)
				 {
					 attribute_name = entry.first;

					 // Skip if the attribute is not used by the material, e.g., because the asset was exported
					 // with an attribute that wasn't used and the asset wasn't optimized.
					 if (attribute_locations.find(attribute_name) != attribute_locations.end())
					 {
						 attribute_location = attribute_locations[attribute_name];
						 tinygltf::Accessor accessor = accessors[entry.second];

						 mesh_buffers.get_attributes()->set_attribute(
							 new VertexBufferAttribute(
								 static_cast<VertexBuffer*>(m_render_resources.buffers[accessor.bufferView].get()),
								 components_data_type[accessor.componentType],
								 components_per_attribute[accessor.componentType],
								 false,
								 accessor.byteOffset,
								 accessor.byteStride),
							 attribute_location);
					 }
				 }

				 // Add pre-created attributes
				 for (auto entry : m_precreated_attributes)
				 {
					 attribute_name = entry.first;
					 attribute_location = attribute_locations[attribute_name];

					 if (attribute_locations.find(attribute_name) != attribute_locations.end())
					 {
						 VertexBufferAttribute* attribute = entry.second;
						 mesh_buffers.get_attributes()->set_attribute(
							 attribute,
							 attribute_location);
					 }
				 }

				 if (!primitive.indices.empty())
				 {
					 tinygltf::Accessor accessor = accessors[primitive.indices];
					 mesh_buffers.set_index_buffer(static_cast<IndexBuffer*>(
						 m_render_resources.buffers[accessor.bufferView].get()));
				 }

				 m_render_resources.vertex_arrays[mesh_id + ".primitive." + i] = 
					 context->create_vertex_array(&mesh_buffers);
			 }
		 }
	 }

}