#pragma once

#include "ShaderProgram.h"
#include "Context.h"
#include "DrawCommand.h"
#include "UniformState.h"
#include "FragmentOutputsGL.h"
#include "ShaderVertexAttributeLocations.h"
#include "ShaderCache.h"

#include <string>
#include <vector>

namespace argeo
{
	class ShaderProgramGL : public ShaderProgram
	{
	public:

		ShaderProgramGL(
			std::string vertex_shader_source,
			std::string fragment_shader_source,
			ShaderVertexAttributeLocations vertex_attribute_locations = ShaderVertexAttributeLocations());

		~ShaderProgramGL();

		void clean(
			UniformState* uniform_state,
			UniformMap*   uniform_map  = nullptr);

		void bind();
		static void unbind();

		void bind_uniform_block(UniformBlock* uniform_block, int binding_point);
		void notify_dirty(ICleanable* value);

		IUniform* get_uniform(std::string name);
		UniformCollection* get_uniforms();

		UniformBlock* get_uniform_block(std::string name);
		UniformBlockCollection* get_uniforms_blocks();

		ShaderVertexAttributeCollection* get_attributes();
		ShaderVertexAttributeLocations get_attribute_locations();

		CachedShaderProgram* get_cached_shader_program() const;
		void set_cached_shader_program(CachedShaderProgram* value);

		unsigned int get_id() const;

		unsigned int get_max_texture_unit_index() const;
		int get_fragments_output(std::string name);

		FragmentOutputs* get_fragments_outputs();

		ShaderObject* get_fragment_shader() const;
		ShaderObject* get_vertex_shader() const;

		unsigned int get_handle() const;

	protected:

		static bool isMatrix(UniformType type);

		static std::string correct_uniform_name(std::string name);

		static std::unique_ptr<UniformBlockCollection> find_uniform_blocks(ShaderProgramGL* program);
		static std::unique_ptr<UniformCollection> find_uniforms(ShaderProgramGL* program);
		static std::unique_ptr<ShaderVertexAttributeCollection> find_attributes(ShaderProgramGL* program);

		std::unique_ptr<IUniform> create_uniform(
			std::string name,
			UniformType type,
			int location,
			int resource_size);

		std::unique_ptr<UniformBlockMember> create_uniform_block_member(
			std::string name,
			UniformType type,
			int offset_in_bytes,
			int length,
			int array_stride_in_bytes,
			int matrix_stride_in_bytes,
			int row_major);

	private:

		// The handle to gpu memory.
		unsigned int m_handle;

		// Max Texture index.
		unsigned int m_max_texture_unit_index;

		// The fragment pipeline stage.
		std::unique_ptr<ShaderObject> m_fragment_shader;

		// The vertex pipeline stage.
		std::unique_ptr<ShaderObject> m_vertex_shader;

		// The active uniforms used by this program.
		std::unique_ptr<UniformCollection> m_uniforms;

		// The dirty uniforms that have changed since last render
		std::vector<ICleanable*> m_dirty_uniforms;

		// The active uniforms blocks used by this program.
		std::unique_ptr<UniformBlockCollection> m_uniforms_blocks;

		// The active attributes used by this program.
		std::unique_ptr<ShaderVertexAttributeCollection> m_vertex_attributes;
		ShaderVertexAttributeLocations m_attribute_locations;

		std::unique_ptr<FragmentOutputs> m_fragments_outputs;

		CachedShaderProgram* m_cached_shader_program;
	};
}