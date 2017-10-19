#include "Model.h"
#include "Device.h"

#include "IndicesUnsignedInt.h"
#include "VertexAttributeFloatVector2.h"
#include "VertexAttributeFloatVector3.h"
#include "VertexAttributeDoubleVector3.h"

#include "UniformSampler.h"
#include "TextureHelper.h"
#include "PickId.h"

#include "StringHelper.h"
#include "EmbeddedResources.h"
#include "AssimpIOSystem.h"
#include "Scene.h"

#include <stack>
#include <future>
#include <regex>
#include <Transforms.h>

#include <assimp/postprocess.h>

namespace argeo
{
	Model::Model(
		std::string filepath,
		std::string name,
		Scene* scene,
		bool normalize_positions,
		HeightClamping height_reference,
		OrientationClamping orientation_reference)
		: m_state(ModelState::NeedsLoad)
		, m_has_outline(true)
		, m_height_changed(true)
		, m_allow_picking(true)
		, m_children_model_matrix_changed(false)
		, m_model_matrix_changed(false)
		, m_texture_loading(false)
		, m_program_loading(false)
		, m_mesh_loading(false)
		, m_asynchronous(true)
		, m_incrementally_load_textures(true)
		, m_just_loaded(false)
		, m_mipmaps(true)
		, m_center(false)
        , m_show(true)
		, m_name(name)
		, m_filepath(filepath)
		, m_model_matrix(mat4d::identity())
		, m_rotation(quaternion::identity())
		, m_scale(1.0)
		, m_position(vec3d(0.0, 0.0, 0.0))
		, m_outline_stroke(10.0)
		, m_height_clamping(height_reference)
		, m_orientation_clamping(orientation_reference)
		, m_normalize_positions(normalize_positions)
		, m_scene(scene)
		, m_shadow_mode(ShadowMode::CastAndReceive)
	    , m_mode(SceneMode::Scene3D)
        , m_rtc_center(vec3d::undefined())
        , m_rtc_center_2D(vec3d::undefined())
        , m_rtc_center_3D(vec3d::undefined())
        , m_initial_radius(std::numeric_limits<double>::quiet_NaN())
        , m_maximum_scale(std::numeric_limits<double>::quiet_NaN())
        , m_minimum_pixel_size(0.0)
        , m_remove_update_height_callback(nullptr)
        , m_up_axis(Axis::X)
	{
		if (m_name.empty())
		{
			throw std::invalid_argument("name");
		}

		m_scene->add_raster_source_changed_listener(this);

		AssimpIOSystem* io_system = new AssimpIOSystem();
		m_importer.SetIOHandler(io_system);

		m_ai_scene = m_importer.ReadFile(
			filepath,
			aiProcess_OptimizeMeshes |
			aiProcess_ImproveCacheLocality |
			aiProcess_RemoveRedundantMaterials |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType |
			aiProcess_GenNormals |
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate);

		// If the import failed, report it
		if (!m_ai_scene)
		{
			std::string error = m_importer.GetErrorString();
			throw std::runtime_error(m_importer.GetErrorString());
		}
	}


	Model::~Model()
	{
        m_remove_update_height_callback();
		m_scene->remove_raster_source_changed_listeners(this);
	}

	///////////////////////////////////////////////////////////
	//						PARSE SCENE
	///////////////////////////////////////////////////////////

	void Model::parse()
	{
		// Parse the model.
		parse_materials();
		parse_meshes();
		parse_render_states();
		parse_shaders();
		parse_programs();
		parse_nodes();
		parse_embebbed_textures();
	}

	void Model::parse_nodes()
	{
		m_model_root_nodes.push_back(parse_node(
			m_ai_scene->mRootNode));
	}

	ModelNode* Model::parse_node(
		const aiNode* node)
	{
		ModelNode* model_node = new ModelNode(
			this,
			node,
			node->mName.C_Str());

		std::string id;
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			std::stringstream id_stream;
			id_stream << node->mMeshes[i];
			id = id_stream.str();

			model_node->add_mesh(
				m_model_meshes_by_id[id]);
		}

		// Then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			model_node->add_child(parse_node(
				node->mChildren[i]));
		}

		return model_node;
	}

	void Model::parse_meshes()
	{
		if (!m_ai_scene->HasMeshes())
		{
			return;
		}

		// Process all the scene's meshes (if any)
		for (unsigned int i = 0; i < m_ai_scene->mNumMeshes; i++)
		{
			aiMesh* mesh = m_ai_scene->mMeshes[i];

			// Generate the model for the mesh.
			std::stringstream id_stream;
			id_stream << i;
			std::string id = id_stream.str();

			ModelMesh* model_mesh = parse_mesh(id, mesh);

			m_model_meshes_by_id[id] = model_mesh;
			m_model_meshes_by_name[mesh->mName.C_Str()] = model_mesh;
		}
	}


	ModelMesh* Model::parse_mesh(
		std::string id,
		const aiMesh* ai_mesh)
	{
		std::stringstream id_material_stream;
		std::string id_material;
		id_material_stream << ai_mesh->mMaterialIndex;
		id_material = id_material_stream.str();

		ModelMesh* model_mesh = new ModelMesh(
			id,
			ai_mesh->mName.C_Str(),
			id_material);

		m_load_resources.meshes_to_load.push(MeshToLoad(id));

		return model_mesh;
	}



	void Model::parse_materials()
	{
		if (!m_ai_scene->HasMaterials())
		{
			return;
		}

		// Process all the scene's materials (if any)
		for (unsigned int i = 0; i < m_ai_scene->mNumMaterials; i++)
		{
			aiMaterial* material = m_ai_scene->mMaterials[i];

			// Generate the materials and uniform maps.
			std::stringstream id_stream;
			id_stream << i;
			std::string id = id_stream.str();

			parse_material(id, material);
		}
	}


	void Model::parse_material_texture(
		std::string id,
		const aiTextureType ai_texture_type,
		const unsigned int ai_texture_index,
		const aiMaterial* ai_material)
	{
		aiString path;
		if (ai_material->GetTexture(ai_texture_type, ai_texture_index, &path) == AI_SUCCESS)
		{
			std::string texture_path = path.C_Str();

			// Add the texture to the creation queue.
			// The embebbed textures starts with *.<texture_number>
			if (strncmp(texture_path.c_str(), "*.", 2) != 0)
			{
				if (m_load_resources.unique_texture_ids.find(texture_path) == m_load_resources.unique_texture_ids.end())
				{
					m_load_resources.unique_texture_ids.insert(texture_path);
					m_load_resources.textures_to_create.push(TextureToCreate(
						texture_path,
						texture_path));
				}
			}
		}
	}

	ModelMaterial* Model::parse_material(
		std::string id,
		const aiMaterial* ai_material)
	{
		MaterialTemplate material_template;

		ModelMaterial* model_material = new ModelMaterial(id, ai_material);

		for (unsigned int i = 0; i < ai_material->mNumProperties; i++)
		{
			aiMaterialProperty* property = ai_material->mProperties[i];

			// Add the texture to the load queue.
			if (property->mSemantic != aiTextureType_NONE)
			{
				parse_material_texture(
					id,
					(aiTextureType)property->mSemantic,
					property->mIndex,
					ai_material);
			}
		}

		aiString name;
		ai_material->Get(AI_MATKEY_NAME, name);
		m_model_materials_by_id[id] = model_material;
		m_model_materials_by_name[name.C_Str()] = model_material;

		return model_material;
	}




	void Model::parse_embebbed_textures()
	{
		if (!m_ai_scene->HasTextures())
		{
			return;
		}

		for (unsigned int i = 0; i < m_ai_scene->mNumTextures; i++)
		{
			aiTexture* texture = m_ai_scene->mTextures[i];

			std::stringstream stream;
			stream << "*." << i;
			std::string id = stream.str();

			if (m_load_resources.unique_texture_ids.find(id) == m_load_resources.unique_texture_ids.end())
			{
				m_load_resources.unique_program_ids.insert(id);
				m_load_resources.embebbed_textures_to_create.push(EmbebbedTextureToLoad(
					id,
					texture->mWidth,
					texture->mHeight));
			}
		}
	}

	void Model::parse_programs()
	{
		for (auto entry : m_model_meshes_by_id)
		{
			parse_program(entry.second);
		}
	}

	void Model::parse_program(ModelMesh* model_mesh)
	{
		aiMesh*     ai_mesh = m_ai_scene->mMeshes[atoi(model_mesh->id.c_str())];
		aiMaterial* ai_material = m_ai_scene->mMaterials[atoi(model_mesh->material.c_str())];

		std::string id = get_shader_program_id(ai_mesh, ai_material);
		if (m_load_resources.unique_program_ids.find(id) == m_load_resources.unique_program_ids.end())
		{
			m_load_resources.unique_program_ids.insert(id);
			m_load_resources.programs_to_create.push(
				ShaderProgramToCreate(
					id,
					get_vertex_shader_id(ai_mesh, ai_material),
					get_fragment_shader_id(ai_mesh, ai_material)));
		}
	}

	std::string Model::get_shader_program_id(
		aiMesh* ai_mesh,
		aiMaterial* ai_material)
	{
		aiShadingMode shading_mode;
		if (AI_SUCCESS != ai_material->Get(AI_MATKEY_SHADING_MODEL, shading_mode))
		{
			shading_mode = aiShadingMode_Blinn;
		}

		unsigned int bit_mask = 0;

		if (ai_material->GetTextureCount(aiTextureType_HEIGHT) > 0)
		{
			bit_mask |= 1 << 0;
		}

		aiColor3D color;
		if (AI_SUCCESS == ai_material->Get(AI_MATKEY_COLOR_SPECULAR, color))
		{
			bit_mask |= 1 << 1;
		}

		if (ai_mesh->HasTextureCoords(0))
		{
			bit_mask |= 1 << 2;
		}

		std::stringstream stream;
		stream << shading_mode;
		stream << bit_mask;
		return stream.str();
	}


	void Model::parse_shaders()
	{
		for (auto entry : m_model_meshes_by_id)
		{
			parse_shader(entry.second);
		}
	}

	void Model::parse_shader(ModelMesh* model_mesh)
	{
		aiMesh*     ai_mesh = m_ai_scene->mMeshes[atoi(model_mesh->id.c_str())];
		aiMaterial* ai_material = m_ai_scene->mMaterials[atoi(model_mesh->material.c_str())];

		aiShadingMode shading_mode;
		if (AI_SUCCESS != ai_material->Get(AI_MATKEY_SHADING_MODEL, shading_mode))
		{
			shading_mode = aiShadingMode_Blinn;
		}

		std::string fragment_shader_id = get_fragment_shader_id(ai_mesh, ai_material);
		if (m_load_resources.shaders.find(fragment_shader_id) == m_load_resources.shaders.end())
		{
			std::string base_source = shading_model_fragment_sources.find(shading_mode)->second;
			std::string modified_source = modify_fragment_shader(base_source, ai_mesh, ai_material);
			m_load_resources.shaders[fragment_shader_id] = modified_source;
		}

		std::string vertex_shader_id = get_vertex_shader_id(ai_mesh, ai_material);
		if (m_load_resources.shaders.find(vertex_shader_id) == m_load_resources.shaders.end())
		{
			std::string base_source = shading_model_vertex_sources.find(shading_mode)->second;
			std::string modified_source = modify_vertex_shader(base_source, ai_mesh, ai_material);
			m_load_resources.shaders[vertex_shader_id] = modified_source;
		}
	}

	std::string Model::get_fragment_shader_id(
		aiMesh* ai_mesh,
		aiMaterial* ai_material)
	{
		aiShadingMode shading_mode;
		if (AI_SUCCESS != ai_material->Get(AI_MATKEY_SHADING_MODEL, shading_mode))
		{
			shading_mode = aiShadingMode_Blinn;
		}

		unsigned int bit_mask = 0;

		if (ai_material->GetTextureCount(aiTextureType_HEIGHT) > 0)
		{
			bit_mask |= 1 << 0;
		}

		aiColor3D color;
		if (AI_SUCCESS == ai_material->Get(AI_MATKEY_COLOR_SPECULAR, color))
		{
			bit_mask |= 1 << 1;
		}

		if (ai_mesh->HasTextureCoords(0))
		{
			bit_mask |= 1 << 2;
		}

		std::stringstream stream;
		stream << shading_mode;
		stream << bit_mask;
		return stream.str();
	}

	std::string Model::get_vertex_shader_id(
		aiMesh* ai_mesh,
		aiMaterial* ai_material)
	{
		aiShadingMode shading_mode;
		if (AI_SUCCESS != ai_material->Get(AI_MATKEY_SHADING_MODEL, shading_mode))
		{
			shading_mode = aiShadingMode_Blinn;
		}

		unsigned int bit_mask = 0;

		if (ai_mesh->HasTextureCoords(0))
		{
			bit_mask |= 1 << 0;
		}

		std::stringstream stream;
		stream << shading_mode;
		stream << bit_mask;
		return stream.str();
	}


	std::string Model::modify_vertex_shader(
		std::string vertex_shader_source,
		aiMesh* ai_mesh,
		aiMaterial* ai_material)
	{
		std::string modified_vertex_source;

		modified_vertex_source += "#define APPLY_NORMAL_MAPPING ";
		modified_vertex_source += (ai_material->GetTextureCount(aiTextureType_HEIGHT) > 0 ? "1" : "0");
		modified_vertex_source += "\n";

		modified_vertex_source += "#define HAS_TEXTURE_COORDINATES ";
		modified_vertex_source += (ai_mesh->HasTextureCoords(0) ? "1" : "0");
		modified_vertex_source += "\n";

		modified_vertex_source += vertex_shader_source;

		return modified_vertex_source;
	}

	std::string Model::modify_fragment_shader(
		std::string fragment_shader_source,
		aiMesh* ai_mesh,
		aiMaterial* ai_material)
	{
		std::string modified_fragment_source;

		modified_fragment_source += "#define APPLY_NORMAL_MAPPING ";
		modified_fragment_source += (ai_material->GetTextureCount(aiTextureType_HEIGHT) > 0 ? "1" : "0");
		modified_fragment_source += "\n";

		aiColor3D color;
		modified_fragment_source += "#define HAS_SPECULAR ";
		modified_fragment_source += (AI_SUCCESS == ai_material->Get(AI_MATKEY_COLOR_SPECULAR, color) ? "1" : "0");
		modified_fragment_source += "\n";

		modified_fragment_source += "#define HAS_TEXTURE_COORDINATES ";
		modified_fragment_source += (ai_mesh->HasTextureCoords(0) ? "1" : "0");
		modified_fragment_source += "\n";

		modified_fragment_source = create_material_method_definition(
			modified_fragment_source,
			ai_material);

		modified_fragment_source = create_uniforms_declaration(
			modified_fragment_source,
			ai_material);

		modified_fragment_source += fragment_shader_source;

		return modified_fragment_source;
	}

	std::string Model::create_material_method_definition(
		std::string shader_source,
		aiMaterial* ai_material)
	{
		shader_source += "og_material og_getMaterial(og_materialInput materialInput)\n{\n";
		shader_source += "	og_material material = og_getDefaultMaterial(materialInput);\n";

		// Add the material properties to the uniform map.
		std::map<std::string, std::string> components;
		for (unsigned int i = 0; i < ai_material->mNumProperties; i++)
		{
			aiMaterialProperty* property = ai_material->mProperties[i];

			std::string component;
			std::string declaration;

			if (get_material_component_declaration(
				ai_material,
				property,
				component,
				declaration))
			{
				components[component] += declaration;
			}
		}

		for (auto entry : components)
		{
			shader_source += "	material." + entry.first + " = " + entry.second + ";\n";
		}

		shader_source += "	return material;\n}\n";

		return shader_source;
	}

	// Writes uniform declarations to the shader file and connects uniform values with
	// corresponding material properties through the returnUniforms function.
	std::string Model::create_uniforms_declaration(
		std::string shader_source,
		aiMaterial* ai_material)
	{
		std::string modified_shader_source = shader_source;

		// Add the material properties to the uniform map.
		for (unsigned int i = 0; i < ai_material->mNumProperties; i++)
		{
			aiMaterialProperty* property = ai_material->mProperties[i];
			std::string uniform_declaration;
			std::string id;
			bool valid = false;

			if (property->mSemantic == aiTextureType_NONE)
			{
				valid = get_material_value_uniform_declaration(
					property->mKey.C_Str(),
					ai_material,
					uniform_declaration,
					id);
			}
			else
			{
				valid = get_material_texture_uniform_declaration(
					(aiTextureType)property->mSemantic,
					property->mIndex,
					ai_material,
					uniform_declaration,
					id);
			}

			if (valid)
			{
				// Add uniform declaration to source code.
				std::regex uniform_declaration_regex("uniform\\s+" + uniform_declaration + "\\s+" + id + "\\s*;");
				if (!std::regex_search(modified_shader_source, uniform_declaration_regex))
				{
					modified_shader_source = "uniform " + uniform_declaration + " " + id + ";\n" + modified_shader_source;
				}
			}
		}

		return modified_shader_source;
	}

	void Model::parse_render_states()
	{
		for (auto entry : m_model_meshes_by_id)
		{
			parse_render_state(entry.second);
		}
	}

	void Model::parse_render_state(ModelMesh* model_mesh)
	{
		aiMesh*     ai_mesh = m_ai_scene->mMeshes[atoi(model_mesh->id.c_str())];
		aiMaterial* ai_material = m_ai_scene->mMaterials[atoi(model_mesh->material.c_str())];

		std::string id = get_render_state_id(ai_mesh, ai_material);
		if (m_load_resources.unique_render_state_ids.find(id) == m_load_resources.unique_render_state_ids.end())
		{
			m_load_resources.unique_render_state_ids.insert(id);
			m_load_resources.render_states_to_load.push(
				RenderStateToLoad(
					id,
					get_render_state_parameters(ai_mesh, ai_material)));
		}
	}

	std::string Model::get_render_state_id(
		aiMesh* ai_mesh,
		aiMaterial* ai_material)
	{
		return "a";
	}

	RenderStateParameters Model::get_render_state_parameters(
		aiMesh* ai_mesh,
		aiMaterial* ai_material)
	{
		RenderStateParameters parameters;
		parameters.facet_culling.enabled = true;
		parameters.facet_culling.face = CullFace::Back;
		parameters.facet_culling.front_face_winding_order = WindingOrder::CounterClockwise;

        bool has_translucency = false;
        for (unsigned int i = 0; i < ai_material->mNumProperties; i++)
        {
            aiMaterialProperty *property = ai_material->mProperties[i];

            if (property->mSemantic == aiTextureType_OPACITY)
            {
                has_translucency = true;
                break;
            }

            if (property->mSemantic == aiTextureType_NONE && assimp_matkey_equals(property->mKey.C_Str(), AI_MATKEY_OPACITY))
            {
                float value;
                if (AI_SUCCESS != ai_material->Get(property->mKey.C_Str(), 0, 0, value))
                {
                    continue;
                }

                if (value < 1.0)
                {
                    has_translucency = true;
                    break;
                }
            }
        }

		parameters.blending.enabled    = has_translucency;
		parameters.depth_test.enabled  = true;
		parameters.depth_test.function = DepthTestFunction::LessOrEqual;

		if (m_has_outline)
		{
			parameters.stencil_mask = 0xFF;
			parameters.stencil_test.enabled = true;
			parameters.stencil_test.front_face = StencilTestFace(
				StencilOperation::Keep,
				StencilOperation::Keep,
				StencilOperation::Replace,
				StencilTestFunction::Always,
				1,
				0xFF);
		}

		return parameters;
	}

	///////////////////////////////////////////////////////////
	//						RESOURCES
	///////////////////////////////////////////////////////////

	void Model::create_resources(FrameState* frame_state)
	{
        Context* context = frame_state->get_context();
        bool scene_3D_only = frame_state->get_scene_3D_only();

		create_meshes(context);
		create_programs(context);
		load_embebbed_textures();
		create_textures(context);
		create_vertex_arrays(context);
		create_render_states(context);
		create_uniform_maps(context);
		create_runtime_nodes(context, scene_3D_only);
	}

	void Model::create_render_states(Context* context)
	{
		if (m_load_resources.render_states_to_load.empty())
		{
			return;
		}

		if (!m_load_resources.create_render_states)
		{
			return;
		}

		m_load_resources.create_render_states = false;

		while (!m_load_resources.render_states_to_load.empty())
		{
			RenderStateToLoad render_state = m_load_resources.render_states_to_load.front();
			create_render_state(render_state, context);
			m_load_resources.render_states_to_load.pop();
		}
	}

	void Model::create_render_state(
		RenderStateToLoad render_state_to_load,
		Context* context)
	{
		RenderStateParameters mesh_params = render_state_to_load.parameters;
		m_render_resources.render_states_by_id[render_state_to_load.id] =
			RenderState::from_cache(mesh_params);

		if (m_has_outline)
		{
			RenderStateParameters outline_params = render_state_to_load.parameters;
			outline_params.depth_test.enabled = false;
			outline_params.stencil_mask = 0x00;
			outline_params.stencil_test.front_face.mask = 0xFF;
			outline_params.stencil_test.front_face.function = StencilTestFunction::NotEqual;
			m_render_resources.outline_render_states_by_id[render_state_to_load.id] =
				RenderState::from_cache(outline_params);
		}
	}

	void Model::create_command(
		RuntimeModelNode*  runtime_node,
		Context* context,
        const bool& scene_3D_only)
	{
		for (ModelMesh* model_mesh : runtime_node->public_node->get_meshes_iterator())
		{
			std::string id = model_mesh->id;

			ModelMaterial* model_material = m_model_materials_by_id[model_mesh->material];
			VertexArray*   vertex_array = m_render_resources.vertex_arrays[model_mesh->id];
			UniformMap*    uniform_map = m_render_resources.uniform_maps_by_material_id[model_mesh->material];
			Mesh*          mesh = m_render_resources.meshes_by_id[id];

			aiMesh*     ai_mesh = m_ai_scene->mMeshes[atoi(model_mesh->id.c_str())];
			aiMaterial* ai_material = m_ai_scene->mMaterials[atoi(model_mesh->material.c_str())];

			ModelNodeCommand* command = new ModelNodeCommand();

			// Calculate the bounding volume for the geometry from the max and min positions.
			VertexAttributeDoubleVector3* position_attribute =
				static_cast<VertexAttributeDoubleVector3*>(mesh->get_attributes()["position"]);
			vec3d min = position_attribute->get_min();
			vec3d max = position_attribute->get_max();
			command->bounding_volume = BoundingSphere::from_corner_points(min, max);

			// Get the program id for this model mesh.
			std::string program_id = get_shader_program_id(ai_mesh, ai_material);

			// Get the render state id for this model mesh.
			std::string render_state_id = get_render_state_id(ai_mesh, ai_material);

			RenderState* render_state = m_render_resources.render_states_by_id[render_state_id];

			const bool casts_shadow = casts_shadows(m_shadow_mode);
			const bool receive_shadow = receive_shadows(m_shadow_mode);

			// Create the draw command.
			command->draw_command = std::unique_ptr<DrawCommand>(new DrawCommand(
				mesh->get_primitive_type(),
				vertex_array,
				m_render_resources.shader_programs_by_id[program_id],
				render_state,
				nullptr,
				uniform_map,
				true));

            // TODO: Translucent
            command->draw_command->set_pass(render_state->get_blending().enabled ?
                            CommandPass::Translucent :
                            CommandPass::Opaque);
			command->draw_command->set_cast_shadows(casts_shadow);
			command->draw_command->set_receive_shadows(receive_shadow);

			// If the picking is allowed then create the picking command.
			if (m_allow_picking)
			{
				runtime_node->pickable_node = new PickableNode(
					this,
					model_mesh,
					nullptr
				);

				// Create the pick id for the mesh.
				PickId* pick_id =
					context->create_pick_id(runtime_node->pickable_node).release();

				// Create the uniform map for the command.
				UniformMap* pick_uniform_map = new UniformMap(*uniform_map);
				(*pick_uniform_map)["og_pickColor"] = [&, pick_id](IUniform* uniform)
				{
					static_cast<Uniform<vec4>*>(uniform)->set_value(
						ColorHelper::color_to_vec4(pick_id->color));
				};

				command->pick_command = std::unique_ptr<DrawCommand>(new DrawCommand(
					mesh->get_primitive_type(),
					vertex_array,
					m_render_resources.pick_shader_programs_by_id[program_id],
					render_state,
					nullptr,
					pick_uniform_map,
					true));

                command->pick_command->set_pass(render_state->get_blending().enabled ?
                                                CommandPass::Translucent :
                                                CommandPass::Opaque);

				m_render_resources.pick_ids[id] = pick_id;
				m_render_resources.pick_uniform_maps_by_material_id[model_mesh->material] = pick_uniform_map;
			}

			if (m_has_outline)
			{
				RenderState* outline_render_state =
					m_render_resources.outline_render_states_by_id[render_state_id];

				UniformMap* outline_uniform_map = new UniformMap(*uniform_map);

				(*outline_uniform_map)["og_outlineStroke"] = [&](IUniform* uniform)
				{
					static_cast<Uniform<float>*>(uniform)->set_value(m_outline_stroke);
				};

				(*outline_uniform_map)["og_outlineColor"] = [&](IUniform* uniform)
				{
					static_cast<Uniform<vec3>*>(uniform)->set_value(
						to_vec3(ColorHelper::color_to_vec4(ColorHelper::red_color())));
				};

				command->outline_command = std::unique_ptr<DrawCommand>(new DrawCommand(
					mesh->get_primitive_type(),
					vertex_array,
					m_render_resources.outline_shader_programs_by_id[program_id],
					outline_render_state,
					nullptr,
					outline_uniform_map,
					true));

				m_render_resources.outline_uniform_maps_by_material_id[model_mesh->material] =
					outline_uniform_map;
			}

            if (!scene_3D_only)
            {
                command->draw_command_2D = std::unique_ptr<DrawCommand>(
                        new DrawCommand(*command->draw_command));
                if (m_allow_picking)
                {
                    command->pick_command_2D = std::unique_ptr<DrawCommand>(
                            new DrawCommand(*command->pick_command));
                }
            }

			// Add the command to the runtime node commands.
			runtime_node->node_commands.push_back(command);
		}
	}

	void Model::create_programs(Context* context)
	{
		if (m_load_resources.programs_to_create.empty())
		{
			return;
		}

		if (!m_load_resources.meshes_to_load.empty())
		{
			return;
		}

		if (m_asynchronous)
		{
			if (!m_load_resources.programs_to_create.empty() && !m_program_loading)
			{
				std::thread load_thread(&Model::program_load_thread_entry_point, this);
				load_thread.detach();
			}
		}
		else
		{
			while (!m_load_resources.programs_to_create.empty())
			{
				ShaderProgramToCreate shader_program = m_load_resources.programs_to_create.front();
				create_program(context, shader_program);
				m_load_resources.programs_to_create.pop();
			}
		}
	}

	void Model::create_program(
		Context* context,
		ShaderProgramToCreate program_to_create)
	{
		std::string id = program_to_create.id;

		if (m_render_resources.shader_programs_by_id.find(id) == m_render_resources.shader_programs_by_id.end())
		{
			std::string vertex_source = m_load_resources.shaders[program_to_create.vertex_shader];
			std::string fragment_source = m_load_resources.shaders[program_to_create.fragment_shader];

			m_render_resources.shader_programs_by_id[id] = context->get_shader_cache()->get_shader_program(
				vertex_source,
				fragment_source);

			if (m_allow_picking)
			{
				m_render_resources.pick_shader_programs_by_id[id] = context->get_shader_cache()->get_shader_program(
					vertex_source,
					Device::create_fragment_pick_shader_source(fragment_source, "uniform"));
			}

			if (m_has_outline)
			{
				m_render_resources.outline_shader_programs_by_id[id] = context->get_shader_cache()->get_shader_program(
					Device::create_vertex_outline_shader_source(vertex_source),
					Device::create_fragment_outline_shader_source(fragment_source));
			}
		}
	}

	void Model::program_load_thread_entry_point()
	{
		m_program_loading = true;
		std::unique_ptr<Surface> pixel_surface = Device::create_pixel_buffer_surface(1, 1);
		while (!m_load_resources.programs_to_create.empty())
		{
			ShaderProgramToCreate shader_program = m_load_resources.programs_to_create.front();
			create_program(
				pixel_surface->get_context(),
				shader_program);
			m_load_resources.programs_to_create.pop();
		}
		std::unique_ptr<Fence> fence(Device::create_fence());
		ClientWaitResult result = fence->client_wait();
		m_program_loading = false;
	}

	void Model::create_uniform_maps(Context* context)
	{
		if (!m_load_resources.finished_program_creation())
		{
			return;
		}

		if (!m_load_resources.create_uniform_maps)
		{
			return;
		}

		m_load_resources.create_uniform_maps = false;

		std::string id;
		UniformFunction uniform_function;
		IUniformValue*  uniform_value = nullptr;

		for (auto entry : m_model_materials_by_id)
		{
			UniformMap* uniform_map = new UniformMap();
			UniformValues* uniform_values = new UniformValues();

			const aiMaterial* ai_material = entry.second->material;

			// Add the material properties to the uniform map.
			for (unsigned int i = 0; i < ai_material->mNumProperties; i++)
			{
				aiMaterialProperty* property = ai_material->mProperties[i];
				bool valid = false;

				if (property->mSemantic == aiTextureType_NONE)
				{
					valid = get_material_value_uniform(
						property->mKey.C_Str(),
						ai_material,
						uniform_function,
						uniform_value,
						id);
				}
				else
				{
					valid = get_material_texture_uniform(
						(aiTextureType)property->mSemantic,
						property->mIndex,
						ai_material,
						uniform_function,
						uniform_value,
						id);
				}

				(*uniform_map)[id] = uniform_function;
				(*uniform_values)[id] = uniform_value;
			}

			m_render_resources.uniform_maps_by_material_id[entry.first] = uniform_map;
		}
	}

	void Model::create_meshes(Context* context)
	{
		if (m_load_resources.meshes_to_load.empty())
		{
			return;
		}

		if (m_asynchronous)
		{
			if (!m_load_resources.meshes_to_load.empty() && !m_mesh_loading)
			{
				std::thread load_thread(&Model::mesh_load_thread_entry_point, this);
				load_thread.detach();
			}
		}
		else
		{
			while (!m_load_resources.meshes_to_load.empty())
			{
				MeshToLoad mesh = m_load_resources.meshes_to_load.front();
				create_vertex_array(mesh);
				m_load_resources.meshes_to_load.pop();
			}
		}
	}

	void Model::mesh_load_thread_entry_point()
	{
		m_mesh_loading = true;
		while (!m_load_resources.meshes_to_load.empty())
		{
			MeshToLoad mesh = m_load_resources.meshes_to_load.front();
			create_vertex_array(mesh);
			m_load_resources.meshes_to_load.pop();
		}
		m_mesh_loading = false;
	}

	void Model::create_vertex_array(MeshToLoad mesh_to_load)
	{
		unsigned int index = atoi(mesh_to_load.id.c_str());
		aiMesh* ai_mesh = m_ai_scene->mMeshes[index];
		unsigned int num_vertices = ai_mesh->mNumVertices;

		vec3d max_position(-std::numeric_limits<double>::max());
		vec3d min_position(std::numeric_limits<double>::max());

		std::vector<vec3d> positions(num_vertices);
		std::vector<vec3>  tangents(num_vertices);
		std::vector<vec3>  bitangents(num_vertices);
		std::vector<vec3>  normals(num_vertices);

		unsigned int num_uv_channels = ai_mesh->GetNumUVChannels();
		std::vector<std::vector<vec2>> textures_coords(num_uv_channels);

		// Process vertex positions, normals and texture coordinates
		for (unsigned int i = 0; i < num_vertices; i++)
		{
			if (ai_mesh->HasPositions())
			{
				aiVector3D vertex_position = ai_mesh->mVertices[i];

				vec3d position(
					vertex_position.x,
					vertex_position.y,
					vertex_position.z);

				positions[i] = position;

				// Compute the max and min positions of the mesh.
				max_position.x = std::max(max_position.x, position.x);
				max_position.y = std::max(max_position.y, position.y);
				max_position.z = std::max(max_position.z, position.z);

				min_position.x = std::min(min_position.x, position.x);
				min_position.y = std::min(min_position.y, position.y);
				min_position.z = std::min(min_position.z, position.z);
			}

			if (ai_mesh->HasNormals())
			{
				aiVector3D vertex_normal = ai_mesh->mNormals[i];
				normals[i] = vec3(vertex_normal.x, vertex_normal.y, vertex_normal.z);
			}

			if (ai_mesh->HasTangentsAndBitangents())
			{
				aiVector3D vertex_tangent = ai_mesh->mTangents[i];
				aiVector3D vertex_bitangent = ai_mesh->mBitangents[i];

				tangents[i] = vec3(vertex_tangent.x, vertex_tangent.y, vertex_tangent.z);
				bitangents[i] = vec3(vertex_bitangent.x, vertex_bitangent.y, vertex_bitangent.z);
			}

			for (int j = 0; j < num_uv_channels; j++)
			{
				if (ai_mesh->HasTextureCoords(j))
				{
					aiVector3D vertex_texture_coord = ai_mesh->mTextureCoords[j][i];
					textures_coords[j].push_back(vec2(vertex_texture_coord.x, vertex_texture_coord.y));
				}
			}
		}

		// Process indices
		IndicesUnsignedInt* indices = new IndicesUnsignedInt();
		for (unsigned int i = 0; i < ai_mesh->mNumFaces; i++)
		{
			aiFace face = ai_mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices->add(face.mIndices[j]);
			}
		}

		// Create the mesh.
		Mesh* mesh = new Mesh();
		mesh->set_primitive_type(PrimitiveType::Triangles);
		mesh->set_front_face_winding_order(WindingOrder::CounterClockwise);


		// Add the attributes to the mesh and the technique.
		if (ai_mesh->HasPositions())
		{
			VertexAttributeDoubleVector3* positions_attribute = new VertexAttributeDoubleVector3("position", num_vertices);
			positions_attribute->set_values(
				positions,
				max_position,
				min_position);
			mesh->add_attribute(positions_attribute);
		}

		if (ai_mesh->HasNormals())
		{
			VertexAttributeFloatVector3* normals_attribute = new VertexAttributeFloatVector3("normal", num_vertices);
			normals_attribute->set_values(normals);
			mesh->add_attribute(normals_attribute);
		}

		if (ai_mesh->HasTangentsAndBitangents())
		{
			VertexAttributeFloatVector3* tangents_attribute = new VertexAttributeFloatVector3("tangent", num_vertices);
			VertexAttributeFloatVector3* bitangents_attribute = new VertexAttributeFloatVector3("bitangent", num_vertices);
			tangents_attribute->set_values(tangents);
			bitangents_attribute->set_values(bitangents);
			mesh->add_attribute(tangents_attribute);
			mesh->add_attribute(bitangents_attribute);
		}

		for (int j = 0; j < num_uv_channels; j++)
		{
			if (ai_mesh->HasTextureCoords(j))
			{
				std::stringstream attribute_name;
				attribute_name << "textureCoordinates";
				if (j > 0)
				{
					attribute_name << j - 1;
				}

				std::string attribute = attribute_name.str();
				VertexAttributeFloatVector2* texture_coords_attribute = new VertexAttributeFloatVector2(attribute_name.str(), num_vertices);
				texture_coords_attribute->set_values(textures_coords[j]);
				mesh->add_attribute(texture_coords_attribute);
			}
		}

		mesh->set_indices(indices);

		m_render_resources.meshes_by_id[mesh_to_load.id] = mesh;
	}

	void Model::load_embebbed_texture(EmbebbedTextureToLoad embebbed_texture)
	{
		unsigned int index = atoi(embebbed_texture.id.substr(2, embebbed_texture.id.length() - 2).c_str());
		aiTexture* texture = m_ai_scene->mTextures[index];

		std::vector<unsigned char> image(embebbed_texture.width * embebbed_texture.height, 0);
		memcpy(
			image.data(),
			reinterpret_cast<unsigned char*>(texture->pcData),
			embebbed_texture.width * embebbed_texture.height * sizeof(unsigned char));

		// The texture data is allways ARGB_8888
		m_load_resources.textures_to_create.push(TextureToCreate(
			embebbed_texture.id,
			image,
			embebbed_texture.width,
			embebbed_texture.height));
	}

	void Model::load_embebbed_textures()
	{
		while (!m_load_resources.embebbed_textures_to_create.empty())
		{
			EmbebbedTextureToLoad embebbed_texture = m_load_resources.embebbed_textures_to_create.front();
			load_embebbed_texture(embebbed_texture);
			m_load_resources.embebbed_textures_to_create.pop();
		}
	}

	void Model::create_textures(Context* context)
	{
		if (m_load_resources.textures_to_create.empty())
		{
			return;
		}

		// Load all textures in the load queue.
		if (m_asynchronous)
		{
			if (!m_load_resources.textures_to_create.empty() && !m_texture_loading)
			{
				std::thread load_thread(&Model::texture_load_thread_entry_point, this);
				load_thread.detach();
			}
		}
		else
		{
			while (!m_load_resources.textures_to_create.empty())
			{
				TextureToCreate texture = m_load_resources.textures_to_create.front();
				create_texture(texture);
				m_load_resources.textures_to_create.pop();
			}
		}
	}

	void Model::texture_load_thread_entry_point()
	{
		m_texture_loading = true;
		std::unique_ptr<Surface> pixel_surface = Device::create_pixel_buffer_surface(1, 1);
		while (!m_load_resources.textures_to_create.empty())
		{
			TextureToCreate texture = m_load_resources.textures_to_create.front();
			create_texture(texture);
			m_load_resources.textures_to_create.pop();
		}
		std::unique_ptr<Fence> fence(Device::create_fence());
		ClientWaitResult result = fence->client_wait();
		m_texture_loading = false;
	}

	void Model::create_texture(TextureToCreate texture_to_create)
	{
		Texture2D* texture = nullptr;

		//In OpenGL ES 2.0, in caso of no-power of 2 textures, the wrap mode can only be GL_CLAMP_TO_EDGE.
		//There are restrictions as well on the filter you can use which can be only GL_NEAREST or GL_LINEAR
		// (in other words no MIPMAPPING)
		bool npot = !TextureHelper::is_power_of_two(texture_to_create.image.size());

		if (texture_to_create.image.size() > 0)
		{
			npot = !TextureHelper::is_power_of_two(texture_to_create.image.size());

			unsigned int width = texture_to_create.width;
			unsigned int height = texture_to_create.height;

			Texture2D* texture_2D = Device::create_texture2D(TextureDescription(
				width,
				height,
				TextureFormat::RedGreenBlueAlpha8,
				m_mipmaps && !npot)).release();

			std::unique_ptr<WritePixelBuffer> pixel_buffer = Device::create_write_pixel_buffer(
				PixelBufferHint::Stream,
				width * height * sizeof(vec4ub));

			pixel_buffer->copy_from_system(texture_to_create.image);

            texture_2D->copy_from_buffer(
				pixel_buffer.get(),
				ImageFormat::BlueGreenRedAlpha,
				ImageDataType::UnsignedByte);

			texture = texture_2D;
			if (!npot)
			{
				texture->set_sampler(Device::get_texture_samplers()->get_linear_mipmap_clamp());
			}
		}
		else
		{
			std::string directory = StringHelper::dirname(m_filepath);
			std::string fullpath = directory.length() > 0 ?
				directory + EmbeddedResources::get_os_separator() + texture_to_create.path :
				texture_to_create.path;

			std::unique_ptr<Bitmap> bitmap = Bitmap::create_bitmap_from_asset(fullpath);

			npot = !TextureHelper::is_power_of_two(bitmap->get_width() * bitmap->get_height());
			texture = Device::create_texture2D(
				bitmap.get(),
				TextureHelper::imaging_pixel_format_to_texture_format(bitmap->get_pixel_format()),
				m_mipmaps && !npot).release();

			if (!npot)
			{
				texture->set_sampler(Device::get_texture_samplers()->get_linear_repeat());
			}
		}

		// The texture id 
		m_render_resources.textures_by_path[texture_to_create.path] = texture;
	}



	void Model::create_vertex_arrays(Context* context)
	{
		if (!m_load_resources.finished_pending_meshes_loads() || !m_load_resources.finished_program_creation())
		{
			return;
		}

		if (!m_load_resources.create_vertex_arrays)
		{
			return;
		}

		m_load_resources.create_vertex_arrays = false;

		for (auto entry : m_model_meshes_by_id)
		{
			std::string	id = entry.first;
			ModelMesh* model_mesh = entry.second;
			Mesh* mesh = m_render_resources.meshes_by_id[id];
			aiMesh*     ai_mesh = m_ai_scene->mMeshes[atoi(model_mesh->id.c_str())];
			aiMaterial* ai_material = m_ai_scene->mMaterials[atoi(model_mesh->material.c_str())];


			std::string program_id = get_shader_program_id(ai_mesh, ai_material);

			m_render_resources.vertex_arrays[id] = context->create_vertex_array(
				mesh,
				m_render_resources.shader_programs_by_id[program_id]->get_attributes(),
				BufferHint::StaticDraw).release();
		}
	}

	void Model::create_runtime_nodes(
            Context* context,
            const bool& scene_3D_only)
	{
		if (!m_load_resources.finished_everything_but_texture_creation())
		{
			return;
		}

		if (!m_load_resources.create_runtime_nodes)
		{
			return;
		}

		m_load_resources.create_runtime_nodes = false;

		std::stack<ModelNode*> node_stack;
		RuntimeModelNode* parent = nullptr;
		for (ModelNode* model_node : m_model_root_nodes)
		{
			node_stack.push(model_node);
			while (!node_stack.empty())
			{
				ModelNode* node = node_stack.top();
				node_stack.pop();

				RuntimeModelNode* child_node = create_runtime_node(node, parent, context, scene_3D_only);
				parent = child_node;

				// Then do the same for each of its children
				for (auto child : node->get_children_iterator())
				{
					node_stack.push(child);
				}
			}
		}
	}

	RuntimeModelNode* Model::create_runtime_node(
		ModelNode* model_node,
		RuntimeModelNode* parent_node,
		Context* context,
        const bool& scene_3D_only)
	{
		// Add the runtime node to the collections.
		RuntimeModelNode* runtime_node = new RuntimeModelNode();
		runtime_node->public_node = model_node;
		const aiNode* ai_node = model_node->get_ai_node();

		aiMatrix4x4 m = ai_node->mTransformation;
		runtime_node->model_matrix = mat4d(
			m.a1, m.a2, m.a3, m.a4,
			m.b1, m.b2, m.b3, m.b4,
			m.c1, m.c2, m.c3, m.c4,
			m.d1, m.d2, m.d3, m.d4);

		if (parent_node != nullptr)
		{
			runtime_node->parents.push_back(runtime_node);
			parent_node->childrens.push_back(runtime_node);
		}
		else
		{
			m_runtime_root_nodes.push_back(runtime_node);
		}


		m_runtime_nodes_by_name[model_node->get_name()] = runtime_node;
		create_command(runtime_node, context, scene_3D_only);

		return runtime_node;
	}

	///////////////////////////////////////////////////////////
	//						UPDATE
	///////////////////////////////////////////////////////////

	void Model::update(FrameState* frame_state)
	{
        if (frame_state->get_mode() == SceneMode::Morphing)
        {
            return;
        }

		Context* context = frame_state->get_context();

		m_default_texture2D = context->get_default_texture2D();

		if (m_state == ModelState::NeedsLoad)
		{
			parse();
			m_state = ModelState::Loading;
		}
		if (m_state == ModelState::Loading)
		{
			create_resources(frame_state);

			// Transition from LOADING -> LOADED once resources are downloaded and created.
			// Textures may continue to stream in while in the LOADED state.
			if ((m_load_resources.finished()) ||
				(m_load_resources.finished_everything_but_texture_creation() && m_incrementally_load_textures))
			{
				m_importer.FreeScene();

				m_state = ModelState::Loaded;
				m_just_loaded = true;
			}
		}

        const bool show = m_show && m_scale != 0;
		
		if ((show && m_state == ModelState::Loaded) || m_just_loaded)
		{
            const bool mode_changed = m_mode != frame_state->get_mode();
            m_mode = frame_state->get_mode();

			bool model_matrix_hierarchy_changed =
                    m_model_matrix_changed ||
                    m_children_model_matrix_changed ||
                    m_height_changed ||
                    mode_changed;


			if (m_just_loaded)
			{
				calculate_bounding_volume();
                m_initial_radius = m_bounding_sphere->get_radius();

                //TODO
                vec3d center;// = Cartesian3.fromArray(extensions.CESIUM_RTC.center);

                if (center != vec3d::zero())
                {
                    m_rtc_center_3D = center;

                    MapProjection* projection = frame_state->get_map_projection();
                    Ellipsoid ellipsoid = projection->get_ellipsoid();
                    Geodetic3D geodetic = ellipsoid.to_geodetic3D(center);

                    vec3d projected_position = projection->project(geodetic).to_vec3d();
                    projected_position       = vec3d(
                            projected_position.z,
                            projected_position.x,
                            projected_position.y);

                    m_rtc_center_2D  = projected_position;
                    m_rtc_center_eye = vec3d::zero();
                    m_rtc_center     = m_rtc_center_3D;
                }
			}

			update_clamping();

            if (model_matrix_hierarchy_changed || m_just_loaded)
            {
                mat4d model_matrix = m_height_clamping == HeightClamping::Ground ?
                                 m_clamped_model_matrix :
                                 m_model_matrix;

                const double scale = get_scale(frame_state);
                m_computed_model_matrix = mat_multiply_by_scale(model_matrix, vec3d(scale));

                if (m_up_axis == Axis::Y)
                {
                    m_computed_model_matrix *= Y_UP_TO_Z_UP;
                }
                else if (m_up_axis == Axis::X)
                {
                    m_computed_model_matrix *= X_UP_TO_Z_UP;
                }
            }

			if (model_matrix_hierarchy_changed ||
                m_just_loaded)
			{

				update_nodes_model_matrix(
					m_just_loaded,
					model_matrix_hierarchy_changed,
                    frame_state->get_map_projection());
			}

			update_shadows();
            m_just_loaded = false;
		}

        if (show)
        {
            push_commands(frame_state);
        }
	}

	void Model::update_shadows()
	{
		if (m_current_shadow_mode !=  m_shadow_mode)
		{
			m_current_shadow_mode = m_shadow_mode;

			const bool casts_shadow = casts_shadows(m_shadow_mode);
			const bool receive_shadow = receive_shadows(m_shadow_mode);

			std::stack<RuntimeModelNode*> node_stack;
			for (RuntimeModelNode* root : m_runtime_root_nodes)
			{
				node_stack.push(root);
				while (!node_stack.empty())
				{
					RuntimeModelNode* node = node_stack.top();
					node_stack.pop();
					
					for (ModelNodeCommand* node : node->node_commands)
					{
						node->draw_command->set_cast_shadows(casts_shadow);
						node->draw_command->set_receive_shadows(receive_shadow);
					}

					for (RuntimeModelNode* child : node->childrens)
					{
						node_stack.push(child);
					}
				}
			}
		}
	}

	void Model::calculate_bounding_volume()
	{
		vec3d min(std::numeric_limits<double>::max());
		vec3d max(-std::numeric_limits<double>::max());

		std::stack<RuntimeModelNode*> node_stack;
		for (RuntimeModelNode* root : m_runtime_root_nodes)
		{
			node_stack.push(root);
			while (!node_stack.empty())
			{
				RuntimeModelNode* node = node_stack.top();
				node_stack.pop();

				mat4d transformation = node->public_node->get_global_matrix();

				for (auto model_mesh : node->public_node->get_meshes_iterator())
				{
					Mesh* mesh = m_render_resources.meshes_by_id[model_mesh->id];

					VertexAttributeDoubleVector3* position_attribute = static_cast<VertexAttributeDoubleVector3*>(
						mesh->get_attributes()["position"]);

					vec3d max_position = position_attribute->get_max();
					vec3d min_position = position_attribute->get_min();

					vec3d transformed_max = transformation * max_position;
					vec3d transformed_min = transformation * min_position;

					max.x = std::max(max.x, transformed_max.x);
					max.y = std::max(max.y, transformed_max.y);
					max.z = std::max(max.z, transformed_max.z);

					min.x = std::min(min.x, transformed_min.x);
					min.y = std::min(min.y, transformed_min.y);
					min.z = std::min(min.z, transformed_min.z);
				}

				for (RuntimeModelNode* child : node->childrens)
				{
					node_stack.push(child);
				}
			}
		}

        m_bounding_sphere = BoundingSphere::from_corner_points(max, min);

        if (m_up_axis == Axis::Y)
        {
            m_bounding_sphere->transform_without_scale(Y_UP_TO_Z_UP)->clone(m_bounding_sphere.get());
        }
        else if (m_up_axis == Axis::X)
        {
            m_bounding_sphere->transform_without_scale(X_UP_TO_Z_UP)->clone(m_bounding_sphere.get());
        }
	}

	void Model::update_clamping()
	{
        if (m_remove_update_height_callback = nullptr)
        {
            m_remove_update_height_callback();
            m_remove_update_height_callback = nullptr;
        }

		ITerrain* terrain   = m_scene->get_terrain();
		Ellipsoid ellipsoid = terrain->get_ellipsoid();

        if (m_height_clamping == HeightClamping::None)
        {
            return;
        }

        // Compute cartographic position so we don't recompute every update.
        mat4d model_matrix = get_model_matrix();

        m_position.x = model_matrix.data[12];
        m_position.y = model_matrix.data[13];
        m_position.z = model_matrix.data[14];

        Geodetic3D geodetic_position = ellipsoid.to_geodetic3D(m_position);

        m_update_height_function = [&, terrain, geodetic_position](vec3d& position)
        {
            mat4d normalize_center_orientate_matrix = mat4d::identity();

            if (m_height_clamping == HeightClamping::Ground)
            {
                Ellipsoid ellipsoid = terrain->get_ellipsoid();
                Geodetic3D geodetic = ellipsoid.to_geodetic3D(position);
                geodetic = Geodetic3D(
                        geodetic.get_longitude(),
                        geodetic.get_latitude(),
                        geodetic.get_height() + geodetic_position.get_height());
                m_clamped_position = ellipsoid.to_geocentric3D(geodetic).to_vec3d();

                if (m_orientation_clamping != OrientationClamping::None && m_mode == SceneMode::Scene3D)
                {
                    Geocentric3D normal = ellipsoid.geodetic_surface_normal(geodetic);
                    m_orientation = quaternion::from_two_vectors(
                            vec3d::unit_y(),
                            normal.to_vec3d());

                    // Orientate the model with respect the reference.
                    normalize_center_orientate_matrix *=
                            mat4d(quat_to_matrix(m_orientation));
                }
            }

            m_clamped_model_matrix = get_model_matrix();
            m_clamped_model_matrix.data[12] = m_clamped_position.x;
            m_clamped_model_matrix.data[13] = m_clamped_position.y;
            m_clamped_model_matrix.data[14] = m_clamped_position.z;



            if (m_normalize_positions)
            {
                double one_over_radius = 1.0 / m_bounding_sphere->get_radius();
                normalize_center_orientate_matrix *=
                        mat_scale(
                                one_over_radius,
                                one_over_radius,
                                one_over_radius);
            }

            if (m_center)
            {
                vec3d center = m_bounding_sphere->get_center();
                normalize_center_orientate_matrix *=
                        mat_translate(
                                -center.x,
                                -center.y,
                                -center.z);
            }


            m_clamped_model_matrix *= normalize_center_orientate_matrix;

            m_height_changed = true;
        };

        m_remove_update_height_callback = terrain->get_raster_source()->add_listener(
                m_update_height_function,
                geodetic_position);

        // Set the correct height now
        double height = terrain->get_point_height(Geodetic2D(
                geodetic_position.get_longitude(),
                geodetic_position.get_latitude()));

        if (!std::isnan(height))
        {
            // Compute the clamped cartesian and call updateHeight callback
            Geodetic3D geodetic(
                    geodetic_position.get_longitude(),
                    geodetic_position.get_latitude(),
                    height);
            vec3d position = ellipsoid.to_geocentric3D(geodetic).to_vec3d();
            m_update_height_function(position);
        }
	}

	void Model::update_nodes_model_matrix(
            const bool& just_loaded,
            const bool& model_matrix_changed,
            MapProjection* projection)
	{
		std::stack<RuntimeModelNode*> node_stack;

        if (m_mode != SceneMode::Scene3D)
        {
            vec4d translation = mat_get_column(m_computed_model_matrix, 3);
            if (translation != vec4d::unit_w())
            {
                m_computed_model_matrix = Transforms::basis_to_2D(projection, m_computed_model_matrix);
                m_rtc_center = m_rtc_center_3D;
            }
            else
            {
                const vec3d center   = m_bounding_sphere->get_center();
                const mat4d to_2D    = Transforms::wgs84_to_2D_model_matrix(projection, center);
                m_computed_model_matrix = to_2D * m_computed_model_matrix;

                if (!vec_undefined(m_rtc_center))
                {
                    mat_set_translation(m_computed_model_matrix, vec3d::zero());
                    m_rtc_center = m_rtc_center_2D;
                }
            }
        }

		for (RuntimeModelNode* root : m_runtime_root_nodes)
		{
			node_stack.push(root);

			while (!node_stack.empty())
			{
				RuntimeModelNode* node = node_stack.top();
				node_stack.pop();

				if (just_loaded || model_matrix_changed)
				{
					node->model_matrix =
                        m_computed_model_matrix *
						node->public_node->get_local_matrix();

					for (ModelNodeCommand* command : node->node_commands)
					{
						DrawCommand* draw_command    = command->draw_command.get();
                        DrawCommand* draw_command_2D = command->draw_command_2D.get();
						DrawCommand* outline_command = command->outline_command.get();
						DrawCommand* pick_command    = command->pick_command.get();

						// Calculate the new bounding volume using the new calculated node matrix.
						draw_command->set_bounding_volume(command->bounding_volume->
							transform(node->model_matrix)->clone(draw_command->get_bounding_volume()));

                        if (!vec_undefined(m_rtc_center))
                        {
                            BoundingSphere* bounding_sphere = static_cast<BoundingSphere*>(draw_command->get_bounding_volume());
                            bounding_sphere->set_center(bounding_sphere->get_center() + m_rtc_center);
                        }

						// Set the new model matrix.
						draw_command->set_model_matrix(node->model_matrix);

						if (m_allow_picking)
						{
							pick_command->set_bounding_volume(draw_command->get_bounding_volume());
							pick_command->set_model_matrix(node->model_matrix);
						}

						if (m_has_outline)
						{
							outline_command->set_bounding_volume(draw_command->get_bounding_volume());
							outline_command->set_model_matrix(node->model_matrix);
						}

                        if (draw_command_2D != nullptr && m_mode == SceneMode::Scene2D)
                        {
                            mat4d node_matrix = node->model_matrix;
                            node_matrix.data[13] -= ArgeoMath::sign(node_matrix.data[13]) * 2.0 * PI * projection->get_ellipsoid().get_maximum_radius();
                            draw_command_2D->set_model_matrix(node_matrix);

                            draw_command_2D->set_bounding_volume(command->bounding_volume->
                                    transform(node_matrix)->clone(draw_command_2D->get_bounding_volume()));

                            if (m_allow_picking)
                            {
                                DrawCommand* pick_command_2D = command->pick_command_2D.get();
                                pick_command_2D->set_bounding_volume(draw_command->get_bounding_volume());
                                pick_command_2D->set_model_matrix(node->model_matrix);
                            }
                        }
					}

					// A node's transform needs to be updated if
					// - It was targeted for animation this frame, or
					// - Any of its ancestors were targeted for animation this frame
					for (RuntimeModelNode* child : node->childrens)
					{
						node_stack.push(child);
					}
				}
			}
		}

		m_children_model_matrix_changed = false;
	}

	void Model::push_commands(FrameState* frame_state)
	{
        const double idl2D = frame_state->get_map_projection()->get_ellipsoid().get_maximum_radius() * PI;
        BoundingSphere* bounding_volume = nullptr;

		std::stack<RuntimeModelNode*> node_stack;
		for (RuntimeModelNode* root : m_runtime_root_nodes)
		{
			node_stack.push(root);

			while (!node_stack.empty())
			{
				RuntimeModelNode* node = node_stack.top();
				node_stack.pop();

				for (ModelNodeCommand* command : node->node_commands)
				{
                    //TODO
                    if (!node->public_node->get_show())
                    {
                        continue;
                    }

					if (frame_state->get_pass() == Pass::Render)
					{
						// Render pass
						DrawCommand* draw_command = command->draw_command.get();
						frame_state->get_command_queue().push_back(draw_command);

                        bounding_volume = static_cast<BoundingSphere*>(command->bounding_volume.get());

                        if ((frame_state->get_mode() == SceneMode::Scene2D) &&
                            (bounding_volume->get_center().y + bounding_volume->get_radius() > idl2D ||
                             bounding_volume->get_center().y - bounding_volume->get_radius() > idl2D))
                        {
                            DrawCommand* draw_command_2D = command->draw_command_2D.get();
                            frame_state->get_command_queue().push_back(draw_command_2D);
                        }

                        // TODO: Silhoute 2D
						if (m_has_outline && get_is_picked())
						{
							DrawCommand* outline_command = command->outline_command.get();
							frame_state->get_command_queue().push_back(outline_command);
						}
					}
					else if (m_allow_picking)
					{
						// Picking pass
						DrawCommand* picking_command = command->pick_command.get();
						frame_state->get_command_queue().push_back(picking_command);

                        bounding_volume = static_cast<BoundingSphere*>(command->bounding_volume.get());

                        if ((frame_state->get_mode() == SceneMode::Scene2D) &&
                            (bounding_volume->get_center().y + bounding_volume->get_radius() > idl2D ||
                             bounding_volume->get_center().y - bounding_volume->get_radius() > idl2D))
                        {
                            DrawCommand* picking_command_2D = command->pick_command_2D.get();
                            frame_state->get_command_queue().push_back(picking_command_2D);
                        }
					}
				}

				for (RuntimeModelNode* child : node->childrens)
				{
					node_stack.push(child);
				}
			}
		}
	}

	void Model::model_matrix_changed()
	{
		m_children_model_matrix_changed = true;
	}

	void Model::update_model_matrix()
	{
		if (m_model_matrix_changed)
		{
			m_model_matrix =
				mat_translate(
					m_translation.x,
					m_translation.y,
					m_translation.z)
				* mat4d(quat_to_matrix(m_rotation));
			m_model_matrix_changed = false;
		}
	}

	void Model::set_euler_angles(
		double pitch,
		double yaw,
		double roll)
	{
		set_rotation(quaternion::from_yaw_pitch_roll(yaw, pitch, roll));
	}

	void Model::set_translation(vec3d translation)
	{
		set_translation(
			translation.x,
			translation.y,
			translation.z);
	}

	void Model::translate(vec3d translation)
	{
		translate(
			translation.x,
			translation.y,
			translation.z);
	}

	void Model::set_translation(
		double x,
		double y,
		double z)
	{
		vec3d translation = vec3d(x, y, z);
		if (m_translation != translation)
		{
			m_model_matrix_changed = true;
			m_translation = translation;
		}
	}

	void Model::translate(
		double x,
		double y,
		double z)
	{
		vec3d translation = vec3d(x, y, z);
		m_model_matrix_changed = true;
		m_position += translation;
	}

	void Model::set_rotation(quaternion rotation)
	{
		if (m_rotation != rotation)
		{
			m_model_matrix_changed = true;
			m_rotation = rotation;
		}
	}

	void Model::rotate(quaternion rotation)
	{
		m_model_matrix_changed = true;
		m_rotation *= rotation;
	}

	void Model::set_scale(const double& value)
	{
		if (m_scale != value)
		{
			m_model_matrix_changed = true;
			m_scale = value;
		}
	}

	quaternion Model::get_rotation() const
	{
		return m_rotation;
	}

	vec3d Model::get_translation() const
	{
		return m_translation;
	}

	double Model::get_scale() const
	{
		return m_scale;
	}

    double Model::scale_in_pixels(
            const vec3d &position_wc,
            const double &radius,
            FrameState *frame_state)
    {
        static BoundingSphere bounding_sphere;
        bounding_sphere.set_center(position_wc);
        bounding_sphere.set_radius(radius);
        return frame_state->get_camera()->get_pixel_size(
                &bounding_sphere,
                frame_state->get_context()->get_drawing_surface_width(),
                frame_state->get_context()->get_drawing_surface_height());
    }

    double Model::get_scale(FrameState *frame_state)
    {
        double scale = m_scale;

        if (m_minimum_pixel_size != 0.0)
        {
            // Compute size of bounding sphere in pixels
            Context* context = frame_state->get_context();
            double max_pixel_size = ::fmax(context->get_drawing_surface_width(), context->get_drawing_surface_height());
            mat4d m = m_height_clamping == HeightClamping::Ground ?
                                 m_clamped_model_matrix :
                                 m_model_matrix;

            vec3d position(
                    m.data[12],
                    m.data[13],
                    m.data[14]);

            if (!vec_undefined(m_rtc_center))
            {
                position += m_rtc_center;
            }

            if (m_mode != SceneMode::Scene3D)
            {
                MapProjection* projection = frame_state->get_map_projection();
                Geodetic3D geodetic = projection->get_ellipsoid().to_geodetic3D(position);
                position = projection->project(geodetic).to_vec3d();
                position = vec3d(
                        position.z,
                        position.x,
                        position.y);
            }

            double radius = m_bounding_sphere->get_radius();
            double meters_per_pixel = scale_in_pixels(position, radius, frame_state);

            // metersPerPixel is always > 0.0
            double pixels_per_meter   = 1.0 / meters_per_pixel;
            double diameter_in_pixels = ::fmin(pixels_per_meter * (2.0 * radius), max_pixel_size);

            // Maintain model's minimum pixel size
            if (diameter_in_pixels < m_minimum_pixel_size)
            {
                scale = (m_minimum_pixel_size * meters_per_pixel) / (2.0 *m_initial_radius);
            }
        }

        return !std::isnan(m_maximum_scale) ?
               ::fmin(m_maximum_scale, scale) :
               scale;
    }

    BoundingSphere* Model::get_bounding_sphere()
    {
        if (m_state != ModelState::Loaded)
        {
            throw std::logic_error("The model is not loaded.  Use Model.readyPromise or wait for Model.ready to be true.");
        }

        mat4d model_matrix = m_height_clamping == HeightClamping::Ground ?
                  m_clamped_model_matrix :
                  m_model_matrix;

        vec3d non_uniform_scale = mat_get_scale(model_matrix);
        double scale = !std::isnan(m_maximum_scale) ? ::fmin(m_maximum_scale, m_scale) : m_scale;
        non_uniform_scale *= scale;

        m_scaled_bounding_sphere->set_center(m_bounding_sphere->get_center() * non_uniform_scale);
        m_scaled_bounding_sphere->set_radius(vec_max_component(non_uniform_scale) * m_initial_radius);

        if (!vec_undefined(m_rtc_center))
        {
            m_scaled_bounding_sphere->set_center(m_scaled_bounding_sphere->get_center() + m_rtc_center);
        }

        return m_scaled_bounding_sphere.get();
    }

	mat4d Model::get_model_matrix()
	{
		update_model_matrix();
		return m_model_matrix;
	}

	std::string Model::get_name() const
	{
		return m_name;
	}

	void Model::set_name(std::string name)
	{
		m_name = name;
	}

	bool Model::get_show() const
	{
		return m_show;
	}

	void Model::set_show(bool value)
	{
		m_show = value;
	}

	ShadowMode Model::get_shadow_mode() const
	{
		return m_shadow_mode;
	}

	void Model::set_shadow_mode(const ShadowMode& value)
	{
		m_shadow_mode = value;
	}

	void Model::on_raster_source_changed(RasterSource* value)
	{ }

	double Model::get_outline_stroke() const
	{
		return m_outline_stroke;
	}

	void Model::set_outline_stroke(const double& value)
	{
		m_outline_stroke = value;
	}

	bool Model::get_allow_picking() const
	{
		return m_allow_picking;
	}

	void Model::set_allow_picking(bool value)
	{
		m_allow_picking = value;
	}

	void Model::set_orientation_clamping(OrientationClamping value)
	{
		m_orientation_clamping = value;
	}

	OrientationClamping Model::get_orientation_clamping() const
	{
		return m_orientation_clamping;
	}

	void Model::set_height_clamping(HeightClamping value)
	{
		m_height_clamping = value;
	}

	HeightClamping Model::get_height_clamping() const
	{
		return m_height_clamping;
	}


	////////////////////////////////////////////////////////////////////////
	//				  CONVERTION ASSIMP - ARGEO
	////////////////////////////////////////////////////////////////////////

#pragma region Assimp conversion

	// http://assimp.sourceforge.net/lib_html/materials.html
	bool Model::get_material_value_uniform_declaration(
		const char* ai_key,
		const aiMaterial* ai_material,
		std::string& uniform_declaration,
		std::string& id)
	{
		if (assimp_matkey_equals(ai_key, AI_MATKEY_NAME))
		{
			return false;
		}

		if (assimp_matkey_equals(ai_key, AI_MATKEY_SHADING_MODEL))
		{
			return false;
		}

		if (assimp_matkey_equals(ai_key, AI_MATKEY_COLOR_AMBIENT) ||
			assimp_matkey_equals(ai_key, AI_MATKEY_COLOR_DIFFUSE) ||
			assimp_matkey_equals(ai_key, AI_MATKEY_COLOR_SPECULAR) ||
			assimp_matkey_equals(ai_key, AI_MATKEY_COLOR_EMISSIVE) ||
			assimp_matkey_equals(ai_key, AI_MATKEY_COLOR_REFLECTIVE))
		{
			aiColor3D color;
			if (AI_SUCCESS != ai_material->Get(ai_key, 0, 0, color))
			{
				return false;
			}

			uniform_declaration = "vec3";
			id = parse_material_name(ai_key);
			return true;
		}

		if (assimp_matkey_equals(ai_key, AI_MATKEY_SHININESS) ||
			assimp_matkey_equals(ai_key, AI_MATKEY_OPACITY) ||
			assimp_matkey_equals(ai_key, AI_MATKEY_REFRACTI))
		{
			float value;
			if (AI_SUCCESS != ai_material->Get(ai_key, 0, 0, value))
			{
				return false;
			}

			uniform_declaration = "float";
			id = parse_material_name(ai_key);
			return true;
		}
		return false;
	}

	bool Model::get_material_texture_uniform_declaration(
		const aiTextureType ai_texture_type,
		const unsigned int ai_texture_index,
		const aiMaterial* ai_material,
		std::string& uniform_declaration,
		std::string& id)
	{
		aiString path;
		if (ai_material->GetTexture(ai_texture_type, ai_texture_index, &path) == AI_SUCCESS)
		{
			uniform_declaration = "sampler2D";
			id = parse_texture_name(ai_texture_type, ai_texture_index);
			return true;
		}

		return false;
	}

	bool Model::get_material_value_uniform(
		const char* ai_key,
		const aiMaterial* ai_material,
		UniformFunction& uniform_function,
		IUniformValue*   uniform_value,
		std::string& id)
	{
		if (assimp_matkey_equals(ai_key, AI_MATKEY_NAME))
		{
			return false;
		}

		if (assimp_matkey_equals(ai_key, AI_MATKEY_SHADING_MODEL))
		{
			return false;
		}

		if (assimp_matkey_equals(ai_key, AI_MATKEY_COLOR_AMBIENT) ||
			assimp_matkey_equals(ai_key, AI_MATKEY_COLOR_DIFFUSE) ||
			assimp_matkey_equals(ai_key, AI_MATKEY_COLOR_SPECULAR) ||
			assimp_matkey_equals(ai_key, AI_MATKEY_COLOR_EMISSIVE) ||
			assimp_matkey_equals(ai_key, AI_MATKEY_COLOR_REFLECTIVE))
		{
			aiColor3D color;
			if (AI_SUCCESS != ai_material->Get(ai_key, 0, 0, color))
			{
				return false;
			}

			uniform_value = new UniformValue<vec3>(
				vec3(color.r, color.g, color.b),
				UniformType::FloatVector3);

			uniform_function = [&, uniform_value](IUniform* value)
			{
				static_cast<Uniform<vec3>*>(value)->set_value(
					static_cast<UniformValue<vec3>*>(uniform_value)->get_data());
			};

			id = parse_material_name(ai_key);
			return true;
		}

		if (assimp_matkey_equals(ai_key, AI_MATKEY_SHININESS) ||
			assimp_matkey_equals(ai_key, AI_MATKEY_OPACITY) ||
			assimp_matkey_equals(ai_key, AI_MATKEY_REFRACTI))
		{
			float value;
			if (AI_SUCCESS != ai_material->Get(ai_key, 0, 0, value))
			{
				throw std::runtime_error("Cannot parse");
			}

			uniform_value = new UniformValue<float>(
				value,
				UniformType::Float);

			uniform_function = [&, uniform_value](IUniform* value)
			{
				static_cast<Uniform<float>*>(value)->set_value(
					static_cast<UniformValue<float>*>(uniform_value)->get_data());
			};

			id = parse_material_name(ai_key);
			return true;
		}
		return false;
	}

	bool Model::get_material_texture_uniform(
		const aiTextureType ai_texture_type,
		const unsigned int ai_texture_index,
		const aiMaterial* ai_material,
		UniformFunction& uniform_function,
		IUniformValue*   uniform_value,
		std::string& id)
	{
		aiString path;
		if (ai_material->GetTexture(ai_texture_type, ai_texture_index, &path) == AI_SUCCESS)
		{
			uniform_value = new UniformValue<std::string>(
				path.C_Str(),
				UniformType::Sampler2D);

			uniform_function = [&, uniform_value](IUniform* value)
			{
				std::string path = static_cast<UniformValue<std::string>*>(uniform_value)->get_data();

				if (m_render_resources.textures_by_path.find(path) != m_render_resources.textures_by_path.end())
				{
					static_cast<UniformSampler*>(value)->set_texture(m_render_resources.textures_by_path[path]);
				}
				// Use the default texture.
				else
				{
					static_cast<UniformSampler*>(value)->set_texture(m_default_texture2D);
				}
			};
			id = parse_texture_name(ai_texture_type, ai_texture_index);
			return true;
		}

		return false;
	}

	bool Model::get_material_component_declaration(
		const aiMaterial* ai_material,
		const aiMaterialProperty* ai_property,
		std::string& component,
		std::string& declaration)
	{
		if (assimp_matkey_equals(ai_property->mKey.C_Str(), AI_MATKEY_NAME))
		{
			return false;
		}

		if (assimp_matkey_equals(ai_property->mKey.C_Str(), AI_MATKEY_SHADING_MODEL))
		{
			return false;
		}

		std::string channels;
		std::stringstream value_stream;

		if (ai_property->mSemantic != aiTextureType_NONE)
		{
			aiTextureType ai_texture_type = static_cast<aiTextureType>(ai_property->mSemantic);
			unsigned int ai_texture_index = ai_property->mIndex;
			component = parse_texture_component_key(ai_texture_type);

			/*
			* Textures are organized in stacks, each stack being evaluated independently.
			* The final color value from a particular texture stack is used in the shading equation.
			* For example, the computed color value of the diffuse texture stack (aiTextureType_DIFFUSE)
			* is multipled with the amount of incoming diffuse light to obtain the final diffuse color of a pixel.
			* Stack Resulting equation
			* ------------------------
			* | Constant base color | color
			* ------------------------
			* | Blend operation 0 | +
			* ------------------------
			* | Strength factor 0 | 0.25*
			* ------------------------
			* | Texture 0 | texture_0
			* ------------------------
			* | Blend operation 1 | *
			* ------------------------
			* | Strength factor 1 | 1.0*
			* ------------------------
			* | Texture 1 | texture_1
			* ------------------------
			* ... ...
			*/
			if (ai_texture_type == aiTextureType_AMBIENT ||
				ai_texture_type == aiTextureType_DIFFUSE ||
				ai_texture_type == aiTextureType_SPECULAR)
			{
				// Get the texture operation.
				aiTextureOp texture_op;
				if (AI_SUCCESS != ai_material->Get(AI_MATKEY_TEXOP(ai_texture_type, ai_texture_index), texture_op))
				{
					texture_op = aiTextureOp_Multiply;
				}

				switch (texture_op)
				{
				case aiTextureOp_Add:
					value_stream << " + ";
					break;
				case aiTextureOp_Multiply:
					value_stream << " * ";
					break;
				case aiTextureOp_Subtract:
					value_stream << " - ";
					break;
				case aiTextureOp_SignedAdd:
					value_stream << " + ";
					break;
				case aiTextureOp_SmoothAdd:
					value_stream << " + ";
					break;
				case aiTextureOp_Divide:
					value_stream << " / ";
					break;
				}

				// Get the blending factor.
				float texture_blend;
				if (AI_SUCCESS == ai_material->Get(AI_MATKEY_TEXBLEND(ai_texture_type, ai_texture_index), texture_blend))
				{
					value_stream << texture_blend << " * ";
				}
			}

			// If the texture is a opacity texture then multiply by the opacity value.
			if (ai_texture_type == aiTextureType_OPACITY)
			{
				value_stream << " * ";
			}

			value_stream << parse_texture_value(ai_texture_type, ai_texture_index);

			declaration = value_stream.str();
			return true;

		}
		else
		{
			component = parse_material_component_key(ai_property->mKey.C_Str());
			declaration = parse_material_name(ai_property->mKey.C_Str());
			return true;
		}
	}

	std::string Model::parse_texture_channels(
		const aiTextureType ai_texture_type)
	{
		switch (ai_texture_type)
		{
		case aiTextureType_OPACITY:
			return "r";
		case aiTextureType_REFLECTION:
			return "r";
		case aiTextureType_SHININESS:
			return "r";
		case aiTextureType_UNKNOWN:
			return "rgb";
		case aiTextureType_DIFFUSE:
			return "rgb";
		case aiTextureType_SPECULAR:
			return "r";
		case aiTextureType_AMBIENT:
			return "rgb";
		case aiTextureType_DISPLACEMENT:
			return "rgb";
		case aiTextureType_EMISSIVE:
			return "r";
		case aiTextureType_LIGHTMAP:
			return "rgb";
		case aiTextureType_HEIGHT:
		case aiTextureType_NORMALS:
			return "rgb";
		}
	}

	std::string Model::parse_texture_value(
		const aiTextureType ai_texture_type,
		const unsigned int ai_texture_index)
	{
		std::stringstream value_stream;
		std::string channels = parse_texture_channels(ai_texture_type);
		switch (ai_texture_type)
		{
		case aiTextureType_HEIGHT:
		case aiTextureType_NORMALS:
			value_stream << "normalize(texture(";
			value_stream << parse_texture_name(ai_texture_type, ai_texture_index);
			value_stream << ", materialInput.st)." << channels << " * 2.0f - 1.0f)";
			break;

		default:
			value_stream << "texture(";
			value_stream << parse_texture_name(ai_texture_type, ai_texture_index);
			value_stream << ", materialInput.st)";
			value_stream << "." << channels;
			break;
		}
		return value_stream.str();
	}

	std::string Model::parse_texture_component_key(
		const aiTextureType ai_texture_type)
	{
		switch (ai_texture_type)
		{
		case aiTextureType_OPACITY:
			return "alpha";
		case aiTextureType_REFLECTION:
			return "reflection";
		case aiTextureType_SHININESS:
			return "shininess";
		case aiTextureType_UNKNOWN:
			return "unknown";
		case aiTextureType_DIFFUSE:
			return "diffuse";
		case aiTextureType_SPECULAR:
			return "specular";
		case aiTextureType_AMBIENT:
			return "ambient";
		case aiTextureType_DISPLACEMENT:
			return "displacement";
		case aiTextureType_EMISSIVE:
			return "emission";
		case aiTextureType_LIGHTMAP:
			return "lightmap";
			// It seems that assimp identify the normal and bumps as height map.
		case aiTextureType_HEIGHT:
		case aiTextureType_NORMALS:
			return "normal";
		}
	}

	std::string Model::parse_texture_name(
		const aiTextureType ai_texture_type,
		const unsigned int ai_texture_index)
	{
		std::stringstream name_stream;
		switch (ai_texture_type)
		{
		case aiTextureType_OPACITY:
			name_stream << "opacityTexture";
			break;
		case aiTextureType_REFLECTION:
			name_stream << "reflectionTexture";
			break;
		case aiTextureType_SHININESS:
			name_stream << "shininessTexture";
			break;
		case aiTextureType_UNKNOWN:
			name_stream << "unknownTexture";
			break;
		case aiTextureType_DIFFUSE:
			name_stream << "diffuseTexture";
			break;
		case aiTextureType_SPECULAR:
			name_stream << "specularTexture";
			break;
		case aiTextureType_AMBIENT:
			name_stream << "ambientTexture";
			break;
		case aiTextureType_DISPLACEMENT:
			name_stream << "displacementTexture";
			break;
		case aiTextureType_EMISSIVE:
			name_stream << "emissiveTexture";
			break;
		case aiTextureType_LIGHTMAP:
			name_stream << "lightmapTexture";
			break;
			// It seems that assimp identify the normal and bumps as height map.
		case aiTextureType_HEIGHT:
		case aiTextureType_NORMALS:
			name_stream << "normalTexture";
			break;
		}

		if (ai_texture_index > 0)
		{
			name_stream << ai_texture_index - 1;
		}

		return name_stream.str();
	}

	bool Model::assimp_matkey_equals(
		const char* char1,
		const char* char2,
		unsigned int a,
		unsigned int b)
	{
		return !strcmp(char1, char2) &&
			a == b;
	}

	std::string Model::parse_material_name(const char* ai_key)
	{
		if (assimp_matkey_equals(ai_key, AI_MATKEY_COLOR_DIFFUSE))
		{
			return "diffuseColor";
		}

		if (assimp_matkey_equals(ai_key, AI_MATKEY_COLOR_SPECULAR))
		{
			return "specularColor";
		}

		if (assimp_matkey_equals(ai_key, AI_MATKEY_COLOR_AMBIENT))
		{
			return "ambientColor";
		}

		if (assimp_matkey_equals(ai_key, AI_MATKEY_COLOR_EMISSIVE))
		{
			return "emissiveColor";
		}

		if (assimp_matkey_equals(ai_key, AI_MATKEY_COLOR_REFLECTIVE))
		{
			return "reflectiveColor";
		}

		if (assimp_matkey_equals(ai_key, AI_MATKEY_COLOR_TRANSPARENT))
		{
			return "transparentColor";
		}

		if (assimp_matkey_equals(ai_key, AI_MATKEY_SHININESS))
		{
			return "shininess";
		}

		if (assimp_matkey_equals(ai_key, AI_MATKEY_OPACITY))
		{
			return "transparent";
		}

		if (assimp_matkey_equals(ai_key, AI_MATKEY_REFRACTI))
		{
			return "refraction";
		}

		throw std::invalid_argument("Invalid key");
	}

	std::string Model::parse_material_component_key(const char* ai_key)
	{
		if (assimp_matkey_equals(ai_key, AI_MATKEY_COLOR_DIFFUSE))
		{
			return "diffuse";
		}

		if (assimp_matkey_equals(ai_key, AI_MATKEY_COLOR_SPECULAR))
		{
			return "specular";
		}

		if (assimp_matkey_equals(ai_key, AI_MATKEY_COLOR_AMBIENT))
		{
			return "ambient";
		}

		if (assimp_matkey_equals(ai_key, AI_MATKEY_COLOR_EMISSIVE))
		{
			return "emission";
		}

		if (assimp_matkey_equals(ai_key, AI_MATKEY_COLOR_REFLECTIVE))
		{
			return "reflection";
		}

		if (assimp_matkey_equals(ai_key, AI_MATKEY_COLOR_TRANSPARENT))
		{
			return "transparent";
		}

		if (assimp_matkey_equals(ai_key, AI_MATKEY_SHININESS))
		{
			return "shininess";
		}

		if (assimp_matkey_equals(ai_key, AI_MATKEY_OPACITY))
		{
			return "alpha";
		}

		if (assimp_matkey_equals(ai_key, AI_MATKEY_REFRACTI))
		{
			return "refraction";
		}

		throw std::invalid_argument("Invalid key");
	}

#pragma endregion
}
