#include "DrawCommand.h"
#include "Context.h"

namespace argeo
{
	DrawCommand::DrawCommand(
		PrimitiveType   primitive_type,
		VertexArray*    vertex_array,
		ShaderProgram*  shader_program,
		RenderState*    render_state,
		FrameBuffer*    frame_buffer,
		UniformMap*	    uniform_map,
		const bool&		cull,
		BoundingVolume* bounding_volume,
		const unsigned int& count,
		const unsigned int& instances,
		const unsigned int& offset,
		const CommandPass& command_pass)
		: m_primitive_type(primitive_type)
		, m_vertex_array(vertex_array)
		, m_shader_program(shader_program)
		, m_render_state(render_state)
		, m_framebuffer(frame_buffer)
		, m_uniform_map(uniform_map)
		, m_bounding_volume(bounding_volume)
		, m_cull(cull)
		, m_count(count)
		, m_instances(instances)
		, m_offset(offset)
		, m_model_matrix(mat4d::identity())
		, m_derived_commands(new DerivedCommands())
		, m_dirty(true)
		, m_execute_on_closest_frustum(false)
		, m_pass(command_pass)
	{ }

	DrawCommand::DrawCommand(const DrawCommand &obj)
            : m_primitive_type(obj.m_primitive_type)
            , m_vertex_array(obj.m_vertex_array)
            , m_shader_program(obj.m_shader_program)
            , m_render_state(obj.m_render_state)
            , m_framebuffer(obj.m_framebuffer)
            , m_uniform_map(obj.m_uniform_map)
            , m_bounding_volume(obj.m_bounding_volume)
            , m_cull(obj.m_cull)
            , m_count(obj.m_count)
            , m_instances(obj.m_instances)
            , m_offset(obj.m_offset)
            , m_model_matrix(obj.m_model_matrix)
            , m_derived_commands(new DerivedCommands())
            , m_dirty(true)
            , m_execute_on_closest_frustum(obj.m_execute_on_closest_frustum)
            , m_pass(obj.m_pass)
	{ }

	DrawCommand::~DrawCommand()
	{ }

	void DrawCommand::set_framebuffer(FrameBuffer* value)
	{
		if (m_framebuffer != value)
		{
			m_dirty = true;
			m_framebuffer = value;
		}
	}

	FrameBuffer* DrawCommand::get_framebuffer()
	{
		return m_framebuffer;
	}

	void DrawCommand::set_pass(const CommandPass& value)
	{
		if (m_pass != value)
		{
			m_dirty = true;
			m_pass = value;
		}
	}
	
	CommandPass DrawCommand::get_pass() const
	{
		return m_pass;
	}

	void DrawCommand::set_primitive_type(const PrimitiveType& value)
	{
		if (m_primitive_type != value)
		{
			m_dirty = true;
			m_primitive_type = value;
		}
	}

	PrimitiveType DrawCommand::get_primitive_type()
	{
		return m_primitive_type;
	}

	void DrawCommand::set_render_state(RenderState* value)
	{
		if (m_render_state != value)
		{
			m_dirty = value;
			m_render_state = value;
		}
	}

	void DrawCommand::set_shader_program(ShaderProgram* value)
	{
		if (m_shader_program != value)
		{
			m_dirty = true;
			m_shader_program = value;
		}
	}

	void DrawCommand::set_vertex_array(VertexArray* value)
	{
		if (m_vertex_array != value)
		{
			m_dirty = true;
			m_vertex_array = value;
		}
	}

	RenderState* DrawCommand::get_render_state()
	{
		return m_render_state;
	}

	ShaderProgram* DrawCommand::get_shader_program()
	{
		return m_shader_program;
	}

	VertexArray* DrawCommand::get_vertex_array()
	{
		return m_vertex_array;
	}

	void DrawCommand::set_uniform_map(UniformMap* value)
	{
		if (m_uniform_map != value) 
		{
			m_dirty = true;
			m_uniform_map = value;
		}		
	}

	UniformMap* DrawCommand::get_uniform_map()
	{
		return m_uniform_map;
	}

	void DrawCommand::set_model_matrix(const mat4d& value)
	{
		if (m_model_matrix != value)
		{
			m_dirty = true;
			m_model_matrix = value;
		}
	}

	mat4d DrawCommand::get_model_matrix()
	{
		return m_model_matrix;
	}

	void DrawCommand::set_instances(const unsigned int& value)
	{
		if (m_instances != value)
		{
			m_dirty = true;
			m_instances = value;
		}
	}

	unsigned int DrawCommand::get_instances()
	{
		return m_instances;
	}

	void DrawCommand::set_offset(const unsigned int& value)
	{
		if (m_offset != value)
		{
			m_dirty = true;
			m_offset = value;
		}
	}

	unsigned int DrawCommand::get_offset()
	{
		return m_offset;
	}

	void DrawCommand::set_count(const unsigned int& value)
	{
		if (m_count != value)
		{
			m_dirty = true;
			m_count = value;
		}
	}

	unsigned int DrawCommand::get_count()
	{
		return m_count;
	}

	void DrawCommand::set_bounding_volume(BoundingVolume* value)
	{
		if (m_bounding_volume != value)
		{
			m_dirty = true;
			m_bounding_volume = value;
		}
	}

	BoundingVolume* DrawCommand::get_bounding_volume()
	{
		return m_bounding_volume;
	}

	void DrawCommand::set_cull(const bool& value)
	{
		if (m_cull != m_dirty)
		{
			m_dirty = true;
			m_cull = value;
		}		
	}

	bool DrawCommand::get_cull()
	{
		return m_cull;
	}


	DerivedCommands* DrawCommand::get_derived_commands()
	{
		return m_derived_commands.get();
	}

	void DrawCommand::set_cast_shadows(const bool& value)
	{
		if (m_cast_shadows != value)
		{
			m_dirty = true;
			m_cast_shadows = value;
		}
		
	}

	bool DrawCommand::get_cast_shadows()
	{
		return m_cast_shadows;
	}

	void DrawCommand::set_receive_shadows(const bool& value)
	{
		if (m_receive_shadows != value)
		{
			m_dirty = true;
			m_receive_shadows = value;
		}
	}

	bool DrawCommand::get_receive_shadows()
	{
		return m_receive_shadows;
	}

	void DrawCommand::set_dirty(const bool& value)
	{
		m_dirty = value;
	}

	bool DrawCommand::get_dirty()
	{
		return m_dirty;
	}

	void DrawCommand::set_execute_on_closest_frustum(const bool& value)
	{
		if (m_execute_on_closest_frustum != value)
		{
			m_dirty = true;
			m_execute_on_closest_frustum = value;
		}
	}

	bool DrawCommand::get_execute_on_closest_frustum()
	{
		return m_execute_on_closest_frustum;
	}

	void DrawCommand::set_last_time_dirty(const std::time_t& value)
	{
		if (m_last_time_dirty != value)
		{
			m_dirty = true;
			m_last_time_dirty = value;
		}
	}

	std::time_t DrawCommand::get_last_time_dirty()
	{
		return m_last_time_dirty;
	}

	void DrawCommand::execute(
		Context*   context,
		PassState* pass_state)
	{
		context->draw(
			this,
			pass_state);
	}


	DrawCommand* DrawCommand::shallow_clone(DrawCommand* result)
	{
		if (result == nullptr)
		{
			result = new DrawCommand(result->get_primitive_type());
		}

		result->set_bounding_volume(m_bounding_volume);
		result->set_cast_shadows(m_cast_shadows);
		result->set_count(m_count);
		result->set_cull(m_cull);
		result->set_dirty(m_dirty);
		result->set_framebuffer(m_framebuffer);
		result->set_instances(m_instances);
		result->set_last_time_dirty(m_last_time_dirty);
		result->set_model_matrix(m_model_matrix);
		result->set_offset(m_offset);
		result->set_pass(m_pass);
		result->set_primitive_type(m_primitive_type);
		result->set_receive_shadows(m_receive_shadows);
		result->set_render_state(m_render_state);
		result->set_shader_program(m_shader_program);
		result->set_uniform_map(m_uniform_map);
		result->set_vertex_array(m_vertex_array);

		return result;
	}
}
