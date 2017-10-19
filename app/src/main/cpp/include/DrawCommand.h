#pragma once

#include "RenderState.h"
#include "ShaderProgram.h"
#include "VertexArray.h"
#include "PrimitiveType.h"
#include "UniformMap.h"
#include "ICommand.h"
#include "FrameBuffer.h"
#include "Matrix.h"
#include "BoundingVolume.h"

namespace argeo
{
	class DrawCommand;

	struct DerivedCommands
	{
		struct ShadowCommand
		{
			ShadowCommand()
				: receive_command(nullptr)
			{ }

			DrawCommand* receive_command;

			unsigned int cast_shader_program_id;
			unsigned int receive_shader_program_id;

			std::vector<DrawCommand*> cast_commands;

			bool cast_shadows;
			bool receive_shadows;
			bool receive_shader_cast_shadows;
		} shadows;
		
	};

	enum class CommandPass : unsigned int
	{
		Opaque		   = 0,
		Globe		   = 1,
		Translucent    = 2,
		CameraPreview  = 3,
		OverlayCommand = 4,
		NumberOfPasses = 5,
	};

	class DrawCommand : public ICommand
	{
	public:

		DrawCommand(
			PrimitiveType   primitive_type,
			VertexArray*    vertex_array	= nullptr,
			ShaderProgram*  shader_program  = nullptr, 
			RenderState*    render_state    = nullptr, 
			FrameBuffer*    frame_buffer    = nullptr,
			UniformMap*	    uniform_map     = nullptr,
			const bool&		cull            = false,
			BoundingVolume* bounding_volume = nullptr,
			const unsigned int& count		= 0,
			const unsigned int& instances   = 0,
			const unsigned int& offset		= 0,
			const CommandPass& command_pass = CommandPass::Opaque);


		DrawCommand(const DrawCommand &obj);  // copy constructor

		~DrawCommand();

		void set_framebuffer(FrameBuffer* value);
		FrameBuffer* get_framebuffer();

		void set_render_state(RenderState* value);
		RenderState* get_render_state();

		void set_shader_program(ShaderProgram* value);
		ShaderProgram* get_shader_program();

		void set_vertex_array(VertexArray* value);
		VertexArray* get_vertex_array();

		void set_primitive_type(const PrimitiveType& value);
		PrimitiveType get_primitive_type();

		void set_uniform_map(UniformMap* value);
		UniformMap* get_uniform_map();

		void  set_model_matrix(const mat4d& value);
		mat4d get_model_matrix();

		void set_bounding_volume(BoundingVolume* value);
		BoundingVolume* get_bounding_volume();

		void set_pass(const CommandPass& value);
		CommandPass get_pass() const;

		void set_cull(const bool& value);
		bool get_cull();

		void set_instances(const unsigned int& value);
		unsigned int get_instances();

		void set_offset(const unsigned int& value);
		unsigned int get_offset();

		void set_count(const unsigned int& value);
		unsigned int get_count();

		DerivedCommands* get_derived_commands();

		void set_cast_shadows(const bool& value);
		bool get_cast_shadows();

		void set_receive_shadows(const bool& value);
		bool get_receive_shadows();

		void set_dirty(const bool& value);
		bool get_dirty();

		void set_execute_on_closest_frustum(const bool& value);
		bool get_execute_on_closest_frustum();

		void set_last_time_dirty(const std::time_t& value);
		std::time_t get_last_time_dirty();

		void execute(
			Context*   context,
			PassState* pass_state = nullptr);

		DrawCommand* shallow_clone(DrawCommand* result = nullptr);

	private:

		unsigned int m_offset;
		unsigned int m_count;
		unsigned int m_instances;

		std::unique_ptr<DerivedCommands> m_derived_commands;

		PrimitiveType    m_primitive_type;
		RenderState*     m_render_state;
		FrameBuffer*     m_framebuffer;
		UniformMap*      m_uniform_map;
		ShaderProgram*   m_shader_program;
		VertexArray*     m_vertex_array;
		

		bool			m_execute_on_closest_frustum;
		bool            m_dirty;
		bool			m_cull;
		bool			m_receive_shadows;
		bool			m_cast_shadows;
		BoundingVolume* m_bounding_volume;

		mat4d           m_model_matrix;

		CommandPass     m_pass;
		std::time_t		m_last_time_dirty;
	};
}
