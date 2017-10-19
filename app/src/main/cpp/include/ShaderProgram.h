#pragma once

#include "ShaderObject.h"
#include "ShaderVertexAttributeCollection.h"
#include "ShaderVertexAttributeLocations.h"

#include "AutomaticUniformFactoryCollection.h"
#include "UniformCollection.h"
#include "UniformBlock.h"
#include "UniformBlockCollection.h"
#include "UniformMap.h"
#include "ICleanableObserver.h"
#include "FragmentOutputs.h"

namespace argeo
{
	class Context;
	class DrawCommand;
	class UniformState;
	struct CachedShaderProgram;

	class ShaderProgram : public ICleanableObserver
	{

	public:

		// Virtual destructor.
		~ShaderProgram() { }

		// The collection of uniforms that are active in the program.
		virtual UniformCollection* get_uniforms() = 0;

		// Get the uniform for the given name.
		// It's used by a delayed technique:
		// When a uniform change it's value then aumatically add itself to the dirty list.
		// Only the uniforms that are in the dirty list are upload to gpu.
		virtual IUniform* get_uniform(std::string name) = 0;

		// The collection of uniforms blocks that are active in the program.
		virtual UniformBlockCollection* get_uniforms_blocks() = 0;
		
		virtual UniformBlock* get_uniform_block(std::string name) = 0;

		// The collection of attributes that are used in the program.
		virtual ShaderVertexAttributeCollection* get_attributes() = 0;


		virtual ShaderVertexAttributeLocations get_attribute_locations() = 0;

		// Get the fragment output index for the given name.
		virtual int get_fragments_output(std::string name) = 0;

		// Get the fragments outputs for this program.
		virtual FragmentOutputs* get_fragments_outputs() = 0;

		virtual unsigned int get_id() const = 0;

		virtual ShaderObject* get_fragment_shader() const = 0;

		virtual CachedShaderProgram* get_cached_shader_program() const = 0;
		virtual void set_cached_shader_program(CachedShaderProgram* value) = 0;

		virtual ShaderObject* get_vertex_shader() const = 0;

		// Bind the uniform block to the binding point.
		virtual void bind_uniform_block(UniformBlock* uniform_block, int binding_point) = 0;

		// Initialize the automatics uniforms.
		void partition_uniforms(UniformCollection* uniforms);

		// Set the manual uniforms.
		// Set the draw automatic uniform in the given context.
		void set_uniforms(
			UniformState* uniform_state,
			UniformMap*   uniform_map = nullptr);

	private:
		std::vector<std::unique_ptr<DrawAutomaticUniform>> m_draw_automatic_uniforms;

		std::vector<IUniform*> m_manual_uniforms;

	};
}
