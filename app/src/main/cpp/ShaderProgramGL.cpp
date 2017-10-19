#include "ShaderProgramGL.h"

#include "ShaderObjectGL.h"

#include "UniformBoolGL.h"
#include "UniformIntGL.h"
#include "UniformFloatGL.h"
#include "UniformSampler.h"
#include "UniformFloatVector2GL.h"
#include "UniformFloatVector3GL.h"
#include "UniformFloatVector4GL.h"
#include "UniformFloatMatrix3GL.h"
#include "UniformFloatMatrix4GL.h"
#include "UniformFloatMatrix42GL.h"

#include "UniformBoolArrayGL.h"
#include "UniformFloatArrayGL.h"
#include "UniformFloatVector2ArrayGL.h"
#include "UniformFloatVector3ArrayGL.h"
#include "UniformFloatVector4ArrayGL.h"
#include "UniformFloatMatrix3ArrayGL.h"
#include "UniformFloatMatrix4ArrayGL.h"

#include "UniformBlockGL.h"
#include "UniformBlockArrayMember.h"
#include "UniformBlockMatrixMember.h"
#include "UniformBlockMatrixArrayMember.h"

#include "TypeConverterGL.h"

#include "GL.h"

namespace argeo
{

	ShaderProgramGL::ShaderProgramGL(
		std::string vertex_shader_source,
		std::string fragment_shader_source,
		ShaderVertexAttributeLocations vertex_attribute_locations)
		: m_max_texture_unit_index(0)
	{
		// Create the shader program.
		m_handle = glCreateProgram();

		ShaderObjectGL* vertex_shader   = nullptr;
		ShaderObjectGL* fragment_shader = nullptr;
		ShaderObjectGL* geometry_shader = nullptr;

		// Create the vertex shader object.
		vertex_shader =
			new ShaderObjectGL(vertex_shader_source, ShaderObjectType::VertexShader);

		// Create the fragment shader object.
		fragment_shader =
			new ShaderObjectGL(fragment_shader_source, ShaderObjectType::FragmentShader);

		// Attach the shader objects to the program.
		glAttachShader(m_handle, vertex_shader->get_handle());
		glAttachShader(m_handle, fragment_shader->get_handle());

		// Bind the vertex attributes locations (if any)
		for (auto entry : vertex_attribute_locations)
		{
			glBindAttribLocation(m_handle,entry.second, entry.first.c_str());
		}
		m_attribute_locations = vertex_attribute_locations;

		// Link the program with the shader objects.
		glLinkProgram(m_handle);

		// Check the status of the shader program.
		GLint status = 0;
		glGetProgramiv(m_handle, GL_LINK_STATUS, &status);
		if (!status)
		{
			GLint info_len = 0;
			glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &info_len);
			std::string str_buffer;
			if (info_len)
			{
				char *buffer = new char[info_len];
				GLint actual_len;
				glGetProgramInfoLog(m_handle, info_len, &actual_len, buffer);
				str_buffer = std::string(buffer);
				delete[] buffer;
			}
			throw std::runtime_error("Shader Program: " + str_buffer);
		}

		// Don't need these anymore.
		glDeleteShader(vertex_shader->get_handle());
		glDeleteShader(fragment_shader->get_handle());

		// Set the compiled shader objects.
		m_vertex_shader   = std::unique_ptr<ShaderObject>(vertex_shader);
		m_fragment_shader = std::unique_ptr<ShaderObject>(fragment_shader);

		// Find parameters of the shader program.
		m_vertex_attributes = find_attributes(this);
		m_uniforms			= find_uniforms(this);
		m_uniforms_blocks	= find_uniform_blocks(this);

		m_fragments_outputs = std::unique_ptr<FragmentOutputs>(new FragmentOutputsGL(this));

		partition_uniforms(m_uniforms.get());
	}

	ShaderProgramGL::~ShaderProgramGL()
	{
		// Delete the uniforms.
		for (auto pair : *m_uniforms)
		{
			delete pair.second;
		}
		// Delete the program.
		glDeleteProgram(m_handle);
	}

	void ShaderProgramGL::notify_dirty(ICleanable* value)
	{
		m_dirty_uniforms.push_back(value);
	}

	void ShaderProgramGL::bind_uniform_block(UniformBlock* uniform_block, int binding_point)
	{
		glUniformBlockBinding(
			m_handle,
			uniform_block->get_location(),
			binding_point);

		uniform_block->set_binding_point(binding_point);
	}

	void ShaderProgramGL::bind()
	{
		glUseProgram(m_handle);
	}

	void ShaderProgramGL::unbind()
	{
		glUseProgram(0);
	}

	std::unique_ptr<UniformCollection> ShaderProgramGL::find_uniforms(ShaderProgramGL* program)
	{
		GLuint prog = program->get_handle();

		UniformCollection* uniform_collection = new UniformCollection();

		// Get the active uniform list.
		int active_uniforms_count = -1;
		glGetProgramiv(prog, GL_ACTIVE_UNIFORMS, &active_uniforms_count);

		if (active_uniforms_count > 0)
		{
			GLuint* uniform_indices = new GLuint[active_uniforms_count];
			for (int i = 0; i < active_uniforms_count; i++)
				uniform_indices[i] = static_cast<GLuint>(i);

			GLint* uniform_block_indices = new GLint[active_uniforms_count];
			glGetActiveUniformsiv(prog, active_uniforms_count, uniform_indices, GL_UNIFORM_BLOCK_INDEX, uniform_block_indices);

			for (int i = 0; i < active_uniforms_count; i++)
			{
				GLint uniform_block_index = uniform_block_indices[i];

				GLenum uniform_type		  = -1;
				GLint uniform_name_length = -1;
				GLint uniform_size		  = -1;

				// Check if the uniforms belongs to the default uniform block (-1)
				if (uniform_block_index != -1)
					continue;

				//assume no variable names longer than 256
				char uniform_name[GL_ACTIVE_UNIFORM_MAX_LENGTH + 1];

				// Get the name of the ith Uniform 
				glGetActiveUniform(
					prog,
					static_cast<GLuint>(i),
					sizeof(uniform_name) - 1,
					&uniform_name_length,
					&uniform_size,
					&uniform_type,
					uniform_name);

				uniform_name[uniform_name_length] = 0;

				// Get the location of the named uniform
				GLuint uniform_location = glGetUniformLocation(prog, uniform_name);

				// Names starting with the reserved prefix of "gl_" have a location of -1.
				if (strncmp(uniform_name, "gl_", 3) == 0)
					continue;

				std::string corrected_uniform_name = correct_uniform_name(uniform_name);

				(*uniform_collection)[corrected_uniform_name] =
					program->create_uniform(
					corrected_uniform_name,
					TypeConverterGL::ToUniformType(uniform_type),
					uniform_location,
					uniform_size).release();
			}

			// Delete the allocated arrays.
			delete[] uniform_indices;
			delete[] uniform_block_indices;
		}

		return std::unique_ptr<UniformCollection>(uniform_collection);
	}

	std::unique_ptr<ShaderVertexAttributeCollection> ShaderProgramGL::find_attributes(ShaderProgramGL* program)
	{
		GLuint prog = program->get_handle();

		ShaderVertexAttributeCollection* attribute_collection = new ShaderVertexAttributeCollection();

		// Get the active attributes list.
		int active_attributes_count = -1;
		glGetProgramiv(prog, GL_ACTIVE_ATTRIBUTES, &active_attributes_count);

		if (active_attributes_count > 0)
		{
			for (int i = 0; i < active_attributes_count; i++)
			{
				GLenum attribute_type = -1;
				GLint  attribute_data_size = -1;

				//assume no variable names longer than 256
				char name[256];
				int  name_length = -1;

				// Get the attribute information.
				glGetActiveAttrib(
					prog,
					static_cast<GLuint>(i),
					sizeof(name) - 1,
					&name_length,
					&attribute_data_size,
					&attribute_type,
					name);

				name[name_length] = 0;

				// Names starting with the reserved prefix of "gl_" have a location of -1.
				if (strncmp(name, "gl_", 3) == 0)
					continue;

				// Get attribute location.
				GLint attribute_location = glGetAttribLocation(
					prog,
					name);

				(*attribute_collection)[name] = 
					new ShaderVertexAttribute(
					name,
					attribute_location,
					attribute_data_size,
					TypeConverterGL::ToShaderVertexAttributeType(attribute_type));
			}
		}

		return std::unique_ptr<ShaderVertexAttributeCollection>(attribute_collection);
	}

	std::unique_ptr<UniformBlockCollection> ShaderProgramGL::find_uniform_blocks(ShaderProgramGL* program)
	{
		GLuint prog = program->get_handle();

		UniformBlockCollection* uniform_block_collection = new UniformBlockCollection();

		int active_uniforms_blocks = -1;
		glGetProgramiv(prog, GL_ACTIVE_UNIFORM_BLOCKS, &active_uniforms_blocks);
		for (int i = 0; i < active_uniforms_blocks; i++)
		{
			// Assume no variable names longer than 256
			char block_name[256];

			GLint block_name_length = -1;
			GLint block_data_size = -1;
			GLint block_binding_point = -1;
			GLint block_active_uniforms_count = -1;

			// Get the block data size in machine units.
			glGetActiveUniformBlockiv(
				prog,
				static_cast<GLuint>(i),
				GL_UNIFORM_BLOCK_DATA_SIZE,
				&block_data_size);

			// Get the block data size in machine units.
			glGetActiveUniformBlockiv(
				prog,
				static_cast<GLuint>(i),
				GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS,
				&block_active_uniforms_count);

			// Get the block binding point.
			glGetActiveUniformBlockiv(
				prog,
				static_cast<GLuint>(i),
				GL_UNIFORM_BLOCK_BINDING,
				&block_binding_point);

			// Get the block name length.
			glGetActiveUniformBlockiv(
				prog,
				static_cast<GLuint>(i),
				GL_UNIFORM_BLOCK_NAME_LENGTH,
				&block_name_length);

			// Get the block name.
			glGetActiveUniformBlockName(
				prog,
				static_cast<GLuint>(i),
				block_name_length,
				0,
				block_name);


			// Create the uniform block.
			UniformBlock* uniform_block = new UniformBlockGL(
				block_name, 
				i,
				block_binding_point,
				block_data_size);

			// Get all members info.
			if (block_active_uniforms_count > 0)
			{
				GLuint* uniform_indices = new GLuint[block_active_uniforms_count];
				for (int i = 0; i < block_active_uniforms_count; i++)
					uniform_indices[i] = static_cast<GLuint>(i);

				GLint* uniform_name_lengths = new GLint[block_active_uniforms_count];
				GLint* uniform_types = new GLint[block_active_uniforms_count];
				GLint* uniform_sizes = new GLint[block_active_uniforms_count];
				GLint* uniform_offsets = new GLint[block_active_uniforms_count];
				GLint* uniform_array_strides = new GLint[block_active_uniforms_count];
				GLint* uniform_matrix_strides = new GLint[block_active_uniforms_count];
				GLint* uniform_row_majors = new GLint[block_active_uniforms_count];

				glGetActiveUniformsiv(prog, block_active_uniforms_count, uniform_indices, GL_UNIFORM_NAME_LENGTH, uniform_name_lengths);
				glGetActiveUniformsiv(prog, block_active_uniforms_count, uniform_indices, GL_UNIFORM_SIZE, uniform_sizes);
				glGetActiveUniformsiv(prog, block_active_uniforms_count, uniform_indices, GL_UNIFORM_TYPE, uniform_types);
				glGetActiveUniformsiv(prog, block_active_uniforms_count, uniform_indices, GL_UNIFORM_OFFSET, uniform_offsets);
				glGetActiveUniformsiv(prog, block_active_uniforms_count, uniform_indices, GL_UNIFORM_ARRAY_STRIDE, uniform_array_strides);
				glGetActiveUniformsiv(prog, block_active_uniforms_count, uniform_indices, GL_UNIFORM_MATRIX_STRIDE, uniform_matrix_strides);
				glGetActiveUniformsiv(prog, block_active_uniforms_count, uniform_indices, GL_UNIFORM_IS_ROW_MAJOR, uniform_row_majors);

				for (int i = 0; i < block_active_uniforms_count; i++)
				{
					GLint uniform_type = uniform_types[i];
					GLint uniform_name_length = uniform_name_lengths[i];
					GLint uniform_size = uniform_sizes[i];
					GLint uniform_offset = uniform_offsets[i];
					GLint uniform_array_stride = uniform_array_strides[i];
					GLint uniform_matrix_stride = uniform_matrix_strides[i];
					GLint uniform_row_major = uniform_row_majors[i];

					//assume no variable names longer than 256
					char uniform_name[256];
					int  name_length = -1;

					// Get the name of the ith Uniform 
					glGetActiveUniform(
						prog,
						static_cast<GLuint>(i),
						sizeof(uniform_name) - 1,
						&name_length,
						nullptr,
						nullptr,
						uniform_name);

					std::string corrected_uniform_name = correct_uniform_name(uniform_name);
					// Add the member info.
					(*uniform_block->get_members())[corrected_uniform_name] =
						program->create_uniform_block_member(
						corrected_uniform_name,
						TypeConverterGL::ToUniformType(uniform_type),
						uniform_offset,
						uniform_size,
						uniform_array_stride,
						uniform_matrix_stride,
						uniform_row_major);
				}

				delete[] uniform_indices;
				delete[] uniform_name_lengths;
				delete[] uniform_types;
				delete[] uniform_sizes;
				delete[] uniform_offsets;
				delete[] uniform_array_strides;
				delete[] uniform_matrix_strides;
			}

			// Finally add the uniform block to the collection.
			(*uniform_block_collection)[block_name] =
				std::unique_ptr<UniformBlock>(uniform_block);

			// Create a one to one mapping between uniform blocks and uniform buffer objects.
			program->bind_uniform_block(uniform_block, block_binding_point);
		}

		return std::unique_ptr<UniformBlockCollection>(uniform_block_collection);
	}

	bool ShaderProgramGL::isMatrix(UniformType type)
	{
		return
			(type == UniformType::FloatMatrix22) ||
			(type == UniformType::FloatMatrix33) ||
			(type == UniformType::FloatMatrix44) ||
			(type == UniformType::FloatMatrix23) ||
			(type == UniformType::FloatMatrix24) ||
			(type == UniformType::FloatMatrix32) ||
			(type == UniformType::FloatMatrix34) ||
			(type == UniformType::FloatMatrix42) ||
			(type == UniformType::FloatMatrix43);
	}

	std::string ShaderProgramGL::correct_uniform_name(std::string name)
	{
		// On ATI, array uniforms have a [0] suffix
		if (!name.compare(name.length() - 3, 3, "[0]"))
			return name.substr(0, name.length() - 3);

		return name;
	}

	std::unique_ptr<IUniform> ShaderProgramGL::create_uniform(
		std::string name,
		UniformType type,
		int location,
		int resource_size)
	{
		if (resource_size == 1)
		{
			switch (type)
			{
			case UniformType::Bool:
				return std::unique_ptr<IUniform>(
					new UniformBoolGL(name, location, this));

			case UniformType::Float:
				return std::unique_ptr<IUniform>(
					new UniformFloatGL(name, location, this));

			case UniformType::FloatVector2:
				return std::unique_ptr<IUniform>(
					new UniformFloatVector2GL(name, location, this));

			case UniformType::FloatVector3:
				return std::unique_ptr<IUniform>(
					new UniformFloatVector3GL(name, location, this));

			case UniformType::FloatVector4:
				return std::unique_ptr<IUniform>(
					new UniformFloatVector4GL(name, location, this));

			case UniformType::FloatMatrix33:
				return std::unique_ptr<IUniform>(
					new UniformFloatMatrix3GL(name, location, this));

			case UniformType::FloatMatrix42:
				return std::unique_ptr<IUniform>(
					new UniformFloatMatrix42GL(name, location, this));

			case UniformType::FloatMatrix44:
				return std::unique_ptr<IUniform>(
					new UniformFloatMatrix4GL(name, location, this));

			case UniformType::Int:
				return std::unique_ptr<IUniform>(
					new UniformIntGL(name, location, type, this));

			case UniformType::Sampler1D:
			case UniformType::Sampler2D:
			case UniformType::Sampler2DExternal:
			case UniformType::Sampler2DRect:
			case UniformType::Sampler2DRectShadow:
			case UniformType::Sampler3D:
			case UniformType::SamplerCube:
			case UniformType::Sampler1DShadow:
			case UniformType::Sampler2DShadow:
			case UniformType::Sampler1DArray:
			case UniformType::Sampler2DArray:
			case UniformType::Sampler1DArrayShadow:
			case UniformType::Sampler2DArrayShadow:
			case UniformType::SamplerCubeShadow:
			case UniformType::IntSampler1D:
			case UniformType::IntSampler2D:
			case UniformType::IntSampler2DRect:
			case UniformType::IntSampler3D:
			case UniformType::IntSamplerCube:
			case UniformType::IntSampler1DArray:
			case UniformType::IntSampler2DArray:
			case UniformType::UnsignedIntSampler1D:
			case UniformType::UnsignedIntSampler2D:
			case UniformType::UnsignedIntSampler2DRect:
			case UniformType::UnsignedIntSampler3D:
			case UniformType::UnsignedIntSamplerCube:
			case UniformType::UnsignedIntSampler1DArray:
			case UniformType::UnsignedIntSampler2DArray:
				return std::unique_ptr<IUniform>(
					new UniformSampler(
						name,
						location,
						type,
						m_max_texture_unit_index++,
						this));
			}
		}
		else
		{
			switch (type)
			{
			case UniformType::Bool:
				return std::unique_ptr<IUniform>(
					new UniformBoolArrayGL(name, location, resource_size, this));

			case UniformType::Float:
				return std::unique_ptr<IUniform>(
					new UniformFloatArrayGL(name, location, resource_size, this));

			case UniformType::FloatVector2:
				return std::unique_ptr<IUniform>(
					new UniformFloatVector2ArrayGL(name, location, resource_size, this));

			case UniformType::FloatVector3:
				return std::unique_ptr<IUniform>(
					new UniformFloatVector3ArrayGL(name, location, resource_size, this));

			case UniformType::FloatVector4:
				return std::unique_ptr<IUniform>(
					new UniformFloatVector4ArrayGL(name, location, resource_size, this));

			case UniformType::FloatMatrix33:
				return std::unique_ptr<IUniform>(
					new UniformFloatMatrix3ArrayGL(name, location, resource_size, this));

			case UniformType::FloatMatrix44:
				return std::unique_ptr<IUniform>(
					new UniformFloatMatrix4ArrayGL(name, location, resource_size, this));


			case UniformType::Int:
				return std::unique_ptr<IUniform>(
					new UniformIntGL(name, location, type, this));

			// Texture Samplers
			case UniformType::Sampler1D:
			case UniformType::Sampler2D:
			case UniformType::Sampler2DRect:
			case UniformType::Sampler2DRectShadow:
			case UniformType::Sampler3D:
			case UniformType::SamplerCube:
			case UniformType::Sampler1DShadow:
			case UniformType::Sampler2DShadow:
			case UniformType::Sampler1DArray:
			case UniformType::Sampler2DArray:
			case UniformType::Sampler1DArrayShadow:
			case UniformType::Sampler2DArrayShadow:
			case UniformType::SamplerCubeShadow:
			case UniformType::IntSampler1D:
			case UniformType::IntSampler2D:
			case UniformType::IntSampler2DRect:
			case UniformType::IntSampler3D:
			case UniformType::IntSamplerCube:
			case UniformType::IntSampler1DArray:
			case UniformType::IntSampler2DArray:
			case UniformType::UnsignedIntSampler1D:
			case UniformType::UnsignedIntSampler2D:
			case UniformType::UnsignedIntSampler2DRect:
			case UniformType::UnsignedIntSampler3D:
			case UniformType::UnsignedIntSamplerCube:
			case UniformType::UnsignedIntSampler1DArray:
			case UniformType::UnsignedIntSampler2DArray:
				return std::unique_ptr<IUniform>(
					new UniformSampler(
						name, 
						location, 
						type,
						m_max_texture_unit_index++,
						this));
			}
		}
	}

	std::unique_ptr<UniformBlockMember> ShaderProgramGL::create_uniform_block_member(
		std::string name,
		UniformType type,
		int offset_in_bytes,
		int length,
		int array_stride_in_bytes,
		int matrix_stride_in_bytes,
		int row_major)
	{
		if (length == 1)
		{
			if (!isMatrix(type))
			{
				// Non array, non matrix member
				return std::unique_ptr<UniformBlockMember>(
					new UniformBlockMember(name, offset_in_bytes, type));
			}
			else
			{
				// Non array, matrix member
				return std::unique_ptr<UniformBlockMember>(
					new UniformBlockMatrixMember(
					name,
					offset_in_bytes,
					matrix_stride_in_bytes,
					row_major != 0,
					type));
			}
		}
		else
		{
			if (!isMatrix(type))
			{
				// Array, non matrix member
				return std::unique_ptr<UniformBlockMember>(
					new UniformBlockArrayMember(
					name,
					length,
					offset_in_bytes,
					array_stride_in_bytes,
					type));
			}
			else
			{
				// Array, matrix member
				return std::unique_ptr<UniformBlockMember>(
					new UniformBlockMatrixArrayMember(
					name,
					length,
					offset_in_bytes,
					matrix_stride_in_bytes,
					row_major != 0,
					array_stride_in_bytes,
					type));
			}
		}
	}

	void ShaderProgramGL::clean(
		UniformState* uniform_state,
		UniformMap*   uniform_map)
	{
		set_uniforms(uniform_state, uniform_map);

		for (int i = 0; i < m_dirty_uniforms.size(); i++)
		{
			m_dirty_uniforms[i]->clean();
		}

		m_dirty_uniforms.clear();
	}

	unsigned int ShaderProgramGL::get_max_texture_unit_index() const
	{
		return m_max_texture_unit_index;
	}

	IUniform* ShaderProgramGL::get_uniform(std::string name)
	{
		return (*m_uniforms).at(name);
	}

	UniformCollection* ShaderProgramGL::get_uniforms()
	{
		return m_uniforms.get();
	}

	UniformBlock* ShaderProgramGL::get_uniform_block(std::string name)
	{
		return (*m_uniforms_blocks).at(name).get();
	}

	UniformBlockCollection* ShaderProgramGL::get_uniforms_blocks()
	{
		return m_uniforms_blocks.get();
	}

	ShaderVertexAttributeCollection* ShaderProgramGL::get_attributes()
	{
		return m_vertex_attributes.get();
	}

	int ShaderProgramGL::get_fragments_output(std::string name)
	{
		return m_fragments_outputs->get_fragment_output(name);
	}

	FragmentOutputs* ShaderProgramGL::get_fragments_outputs()
	{
		return m_fragments_outputs.get();
	}

	ShaderObject* ShaderProgramGL::get_fragment_shader() const
	{
		return m_fragment_shader.get();
	}

	ShaderObject* ShaderProgramGL::get_vertex_shader() const
	{
		return m_vertex_shader.get();
	}

	unsigned int ShaderProgramGL::get_handle() const
	{
		return m_handle;
	}

	unsigned int ShaderProgramGL::get_id() const
	{
		return m_handle;
	}

	CachedShaderProgram* ShaderProgramGL::get_cached_shader_program() const
	{
		return m_cached_shader_program;
	}

	void ShaderProgramGL::set_cached_shader_program(CachedShaderProgram* value)
	{
		m_cached_shader_program = value;
	}

	ShaderVertexAttributeLocations ShaderProgramGL::get_attribute_locations()
	{
		return m_attribute_locations;
	}
}