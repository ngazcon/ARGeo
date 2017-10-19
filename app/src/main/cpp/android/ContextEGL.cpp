#include <ReadPixelBufferGL.h>
#include <WritePixelBufferGL.h>
#include "include/ContextEGL.h"

#include "Device.h"
#include "TypeConverterGL.h"
#include "IndexBufferGL.h"
#include "IPickable.h"
#include "PickId.h"
#include "PickFramebuffer.h"
#include "include/SurfaceWindowEGL.h"


namespace argeo
{
	const ShaderVertexAttributeLocations ContextEGL::attribute_locations =
	{
		{ "position",			0 },
		{ "textureCoordinates",	1 },
	};

	ContextEGL::ContextEGL(
		Surface* surface,
		Context* shared_context,
		const unsigned int& width,
		const unsigned int& height)
		: m_clear_color(ColorHelper::black_color())
		, m_shader_cache(new ShaderCache())
		, m_current_shader_program(nullptr)
		, m_current_framebuffer(nullptr)
		, m_surface(surface)
	{
		// Request an OpenGL ES 3 context.
		const EGLint attribs[] = 
		{
			EGL_CONTEXT_CLIENT_VERSION, 3, 
			EGL_NONE
		};

		// Create the egl context.
		SurfaceEGL* egl_window = dynamic_cast<SurfaceEGL*>(surface);
		if (shared_context != nullptr)
		{
			ContextEGL* egl_context = static_cast<ContextEGL*>(shared_context);			
			m_context = eglCreateContext(
				egl_window->get_display(),
				egl_window->get_config(),
				egl_context->get_native_context(),
				attribs);
		}
		else
		{
			m_context = eglCreateContext(
				egl_window->get_display(),
				egl_window->get_config(),
				EGL_NO_CONTEXT, 
				attribs);
		}

		if (m_context == EGL_NO_CONTEXT)
		{
			throw std::logic_error("Cannot create the context.");
		}

		// Make the context as current
		attach();


		float clear_color[4];

		// Get the default values.
		glGetFloatv(GL_DEPTH_CLEAR_VALUE, &m_clear_depth);
		glGetFloatv(GL_COLOR_CLEAR_VALUE, clear_color);

		glGetIntegerv(GL_STENCIL_CLEAR_VALUE, &m_clear_stencil);

		m_clear_color.red   = clear_color[0] * 255.0;
		m_clear_color.green = clear_color[1] * 255.0;
		m_clear_color.blue  = clear_color[2] * 255.0;
		m_clear_color.alpha = clear_color[3] * 255.0;

		// Set defaults states
		m_default_render_state = RenderState::from_cache();
		m_default_pass_state   = new PassState(this);

		m_current_pass_state   = m_default_pass_state;
		m_current_render_state = m_default_render_state;

		m_default_clear_command = new ClearCommand();

		// Force apply the default render state.
		m_current_render_state->apply(m_current_pass_state);
	}

	ContextEGL::~ContextEGL()
	{ 
		detach();
	}

	void ContextEGL::detach()
	{
		SurfaceEGL* window =
			dynamic_cast<SurfaceEGL*>(m_surface);

		if (eglMakeCurrent(
			window->get_display(),
			EGL_NO_SURFACE,
			EGL_NO_SURFACE,
			EGL_NO_CONTEXT) == EGL_FALSE)
		{
			throw std::logic_error("Cannot detach the context.");
		}
	}

	void ContextEGL::attach()
	{
		SurfaceEGL* window =
			dynamic_cast<SurfaceEGL*>(m_surface);
		
		if(eglMakeCurrent(
			window->get_display(), 
			window->get_surface(),
			window->get_surface(),
			m_context) == EGL_FALSE)
		{
			throw std::logic_error("Cannot attach the context as current.");
		}
	}

	std::unique_ptr<PickFramebuffer> ContextEGL::create_pick_frame_buffer()
	{
		return std::unique_ptr<PickFramebuffer>(
			new PickFramebuffer(this));
	}

	std::unique_ptr<FrameBuffer> ContextEGL::create_frame_buffer()
	{
		return std::unique_ptr<FrameBuffer>(
			new FrameBufferGL());
	}

	std::unique_ptr<VertexArray> ContextEGL::create_vertex_array(
		VertexBufferAttributes attributes,
		IndexBuffer* index_buffer)
	{
		std::unique_ptr<VertexArray> vertex_array = create_vertex_array();

		vertex_array->set_dispose_buffers(true);
		vertex_array->set_index_buffer(index_buffer);

		for (auto entry : attributes)
		{
			vertex_array->get_attributes()->set_attribute(
				entry.second,
				entry.first);
		}

		return vertex_array;
	}

	std::unique_ptr<VertexArray> ContextEGL::create_vertex_array(MeshBuffers* mesh_buffers)
	{
		std::unique_ptr<VertexArray> vertex_array = create_vertex_array();

		vertex_array->set_dispose_buffers(true);
		vertex_array->set_index_buffer(mesh_buffers->get_index_buffer());

		for (int i = 0; i < mesh_buffers->get_attributes()->get_maximum_count(); i++)
		{
			vertex_array->get_attributes()->set_attribute(
				mesh_buffers->get_attributes()->get_attribute(i),
				i);
		}

		return vertex_array;
	}

	std::unique_ptr<VertexArray> ContextEGL::create_vertex_array(
			Mesh* mesh,
			const ShaderVertexAttributeLocations& locations,
			const BufferHint& hint)
	{
		return create_vertex_array(
				Device::create_mesh_buffers(mesh, locations, hint).get());
	}

	std::unique_ptr<VertexArray> ContextEGL::create_vertex_array(
		Mesh* mesh,
		ShaderVertexAttributeCollection* shader_attributes,
		const BufferHint& hint)
	{
		return create_vertex_array(
			Device::create_mesh_buffers(mesh, shader_attributes, hint).get());
	}

	std::unique_ptr<VertexArray> ContextEGL::create_vertex_array()
	{
		return std::unique_ptr<VertexArray>(
			new VertexArrayGL());
	}

	UniformState* ContextEGL::get_uniform_state()
	{
		return &m_uniform_state;
	}

	FrameBuffer* ContextEGL::get_frame_buffer()
	{
		return m_current_framebuffer;
	}

	ShaderCache* ContextEGL::get_shader_cache()
	{
		return m_shader_cache.get();
	}

	/* **************************************************************************
	*							DRAW METHODS
	* **************************************************************************/

	void ContextEGL::end_frame()
	{
		m_current_framebuffer = nullptr;
		FrameBufferGL::unbind();

		m_current_shader_program = nullptr;
		ShaderProgramGL::unbind();

        ReadPixelBufferGL::unbind();
        WritePixelBufferGL::unbind();

		// TODO: Unbind the textures
		//TextureUnitsGL* texture_units = static_cast<TextureUnitsGL*>(Device::get_texture_units());
		//texture_units->unbind(m_max_texture_unit_index);
	}

	void ContextEGL::begin_frame()
	{
		m_max_texture_unit_index = 0;
	}

	void ContextEGL::draw(
		DrawCommand* draw_command,
		PassState*   pass_state)
	{
		pass_state = pass_state != nullptr ?
			pass_state :
			m_default_pass_state;

		apply_before_draw(draw_command, pass_state);

		VertexArrayGL* vertex_array = static_cast<VertexArrayGL*>(draw_command->get_vertex_array());
		IndexBufferGL* index_buffer = static_cast<IndexBufferGL*>(vertex_array->get_index_buffer());

		PrimitiveType primitive_type = draw_command->get_primitive_type();
		unsigned int offset = draw_command->get_offset();
		unsigned int count = draw_command->get_count();
		unsigned int instances = draw_command->get_instances();

		if (index_buffer != nullptr)
		{
			count = count > 0 ? count : index_buffer->get_count();

			// offset in vertices to offset in bytes
			offset = offset * VertexArraySizes::size_of(index_buffer->get_type());
			void* offset_ptr = reinterpret_cast<void*>(offset);

			if (instances > 0)
			{
				glDrawElementsInstanced(
					TypeConverterGL::ToEnum(primitive_type),
					count,
					TypeConverterGL::ToEnum(index_buffer->get_type()),
					offset_ptr,
					instances);
			}
			else
			{
				glDrawElements(
					TypeConverterGL::ToEnum(primitive_type),
					count,
					TypeConverterGL::ToEnum(index_buffer->get_type()),
					offset_ptr);
			}
		}
		else
		{
			count = count > 0 ? count : vertex_array->get_maximum_array_index() + 1;

			if (instances > 0)
			{
				glDrawArraysInstanced(
					TypeConverterGL::ToEnum(primitive_type),
					offset,
					count,
					instances);
			}
			else
			{
				glDrawArrays(
					TypeConverterGL::ToEnum(primitive_type),
					offset,
					count);
			}
		}

		apply_after_draw(draw_command, pass_state);
	}


	void ContextEGL::clear(
		ClearCommand* clear_command,
		PassState*  pass_state)
	{
		clear_command = clear_command != nullptr ? clear_command : m_default_clear_command;
		pass_state = pass_state != nullptr ? pass_state : m_default_pass_state;

		// TODO: StencilMaskSeparate
		if (m_clear_color != clear_command->color)
		{
			glClearColor(
				(float)clear_command->color.red / 255.0f,
				(float)clear_command->color.green / 255.0f,
				(float)clear_command->color.blue / 255.0f,
				(float)clear_command->color.alpha / 255.0f);

			m_clear_color = clear_command->color;
		}

		if (m_clear_depth != clear_command->depth)
		{
			glClearDepthf(clear_command->depth);
			m_clear_depth = clear_command->depth;
		}

		if (m_clear_stencil != clear_command->stencil)
		{
			glClearStencil(clear_command->stencil);
			m_clear_stencil = clear_command->stencil;
		}

		RenderState* render_state = clear_command->render_state != nullptr ?
			clear_command->render_state :
			m_default_render_state;
		apply_render_state(render_state, pass_state, true);

		// The command's framebuffer takes presidence over the pass' framebuffer, e.g., for off-screen rendering.
		FrameBuffer* frame_buffer = clear_command->frame_buffer != nullptr ?
			clear_command->frame_buffer :
			pass_state->get_framebuffer();
		apply_frame_buffer(frame_buffer);

		glClear(TypeConverterGL::ToEnum(clear_command->buffers));
	}

	/* **************************************************************************
	*							PRIVATE METHODS
	* **************************************************************************/

	void ContextEGL::apply_render_state(
		RenderState* render_state,
		PassState*   pass_state,
		bool clear)
	{
		RenderState* previous_render_state = m_current_render_state;
		PassState*   previous_pass_state = m_current_pass_state;
		m_current_pass_state = pass_state;
		m_current_render_state = render_state;

		RenderState::apply_transition(
			previous_render_state,
			render_state,
			previous_pass_state,
			pass_state,
			clear);
	}

	void ContextEGL::apply_frame_buffer(FrameBuffer* frame_buffer)
	{
		FrameBufferGL* gl_frame_buffer = static_cast<FrameBufferGL*>(frame_buffer);

		if (m_current_framebuffer != gl_frame_buffer)
		{
			if (gl_frame_buffer != nullptr)
			{
				gl_frame_buffer->bind();
			}
			else
			{
				FrameBufferGL::unbind();
			}

			m_current_framebuffer = gl_frame_buffer;
		}

		if (gl_frame_buffer != nullptr)
		{
			gl_frame_buffer->clean();
		}
	}


	void ContextEGL::apply_before_draw(
		DrawCommand* draw_command,
		PassState*   pass_state)
	{
		RenderState* render_state = draw_command->get_render_state() != nullptr ?
			draw_command->get_render_state() :
			m_default_render_state;

		// The command's framebuffer takes presidence over the pass' framebuffer, e.g., for off-screen rendering.
		FrameBuffer* frame_buffer = draw_command->get_framebuffer() != nullptr ?
			draw_command->get_framebuffer() :
			pass_state->get_framebuffer();

		verify_draw(
			draw_command,
			pass_state,
			frame_buffer);

		apply_render_state(render_state, pass_state);
		apply_shader_program(draw_command, pass_state);
		apply_vertex_array(draw_command->get_vertex_array());
		apply_textures();
		apply_frame_buffer(frame_buffer);
	}

	void ContextEGL::apply_textures()
	{
		TextureUnitsGL* texture_units = static_cast<TextureUnitsGL*>(Device::get_texture_units());
		texture_units->clean();
	}

	void ContextEGL::apply_after_draw(
		DrawCommand* draw_command,
		PassState*   pass_state)
	{
		VertexArrayGL::unbind();
	}

	void ContextEGL::apply_vertex_array(VertexArray* vertex_array)
	{
		VertexArrayGL* vertex_array_GL3x = static_cast<VertexArrayGL*>(vertex_array);
		vertex_array_GL3x->bind();
		vertex_array_GL3x->clean();
	}

	void ContextEGL::apply_shader_program(
		DrawCommand* draw_command,
		PassState*   pass_state)
	{
		ShaderProgramGL* gl_shader_program = static_cast<ShaderProgramGL*>(draw_command->get_shader_program());

		if (m_current_shader_program != gl_shader_program)
		{
			gl_shader_program->bind();
			m_current_shader_program = gl_shader_program;
		}

		m_max_texture_unit_index = std::max(
			m_max_texture_unit_index,
			gl_shader_program->get_max_texture_unit_index());

		m_uniform_state.set_model_matrix(draw_command->get_model_matrix());
		m_current_shader_program->clean(
			&m_uniform_state,
			draw_command->get_uniform_map());
	}

	void ContextEGL::verify_draw(
		DrawCommand* draw_command,
		PassState*   pass_state,
		FrameBuffer* frame_buffer)
	{
		if (draw_command->get_shader_program() == nullptr)
		{
			throw std::invalid_argument("draw_command");
		}

		if (draw_command->get_vertex_array() == nullptr)
		{
			throw std::invalid_argument("draw_command");
		}

		if (frame_buffer != nullptr)
		{
			if (draw_command->get_render_state()->get_depth_test().enabled)
			{
				if ((frame_buffer->get_depth_attachment_texture()		  == nullptr && frame_buffer->get_depth_attachment_render_buffer()		   == nullptr) &&
					(frame_buffer->get_depth_stencil_attachment_texture() == nullptr && frame_buffer->get_depth_stencil_attachment_render_buffer() == nullptr))
				{
					throw std::invalid_argument(
						"The depth test is enabled (draw_command.RenderState.DepthTest.enabled) but the context's Framebuffer property doesn't have a depth or depth/stencil attachment (DepthAttachment or DepthStencilAttachment).");
				}
			}
		}
	}

	DrawCommand* ContextEGL::create_viewport_quad_command(
		const std::string& fragment_shader,
		RenderState* render_state,
		UniformMap* uniform_map)
	{

		return new DrawCommand(
			PrimitiveType::Triangles,
			get_viewport_quad_vertex_array(),
			get_shader_cache()->get_shader_program(
				viewport_quad_vs,
				fragment_shader),
			render_state,
			nullptr,
			uniform_map);
	}

	VertexArray* ContextEGL::get_viewport_quad_vertex_array()
	{
		if (m_viewport_quad_vertex_array == nullptr)
		{
			std::vector<vec2> positions = 
			{
				vec2(-1.0, -1.0), vec2( 1.0, -1.0),
				vec2( 1.0,  1.0), vec2(-1.0,  1.0)
			};
			VertexBufferAttribute* positions_attribute = new VertexBufferAttribute(
				Device::create_vertex_buffer(positions, BufferHint::StaticDraw).release(),
				ComponentDataType::Float,
				2);
				
			std::vector<vec2> texture_coordinates = 
			{
				vec2(0.0, 0.0), vec2(1.0, 0.0),
				vec2(1.0, 1.0), vec2(0.0, 1.0)
			};
			VertexBufferAttribute* texture_coordinate_attribute = new VertexBufferAttribute(
				Device::create_vertex_buffer(texture_coordinates, BufferHint::StaticDraw).release(),
				ComponentDataType::Float,
				2);
			std::vector<unsigned short> indices =
			{
				(unsigned short)0, (unsigned short)1, (unsigned short)2,
				(unsigned short)0, (unsigned short)2, (unsigned short)3
			};
			
			IndexBuffer* index_buffer = Device::create_index_buffer(
				BufferHint::StaticDraw,
				indices.size() * sizeof(unsigned short)).release();
			index_buffer->copy_from_system<unsigned short>(indices);

			m_viewport_quad_vertex_array = create_vertex_array(
				VertexBufferAttributes
			{
				{ attribute_locations.at("position"),           positions_attribute },
				{ attribute_locations.at("textureCoordinates"), texture_coordinate_attribute },
			},
			index_buffer);
			m_viewport_quad_vertex_array->set_dispose_buffers(true);
		}

		return m_viewport_quad_vertex_array.get();
	}

	std::vector<vec4ub> ContextEGL::read_pixels(
		int   x_offset,
		int   y_offset,
		int   width,
		int   height,
		FrameBuffer* frame_buffer)
	{
		apply_frame_buffer(frame_buffer);

		std::vector<vec4ub> pixels(width * height);
		glReadPixels(
			x_offset,
			y_offset,
			width,
			height,
			TypeConverterGL::ToEnum(ImageFormat::RedGreenBlueAlpha),
			TypeConverterGL::ToEnum(ImageDataType::UnsignedByte),
			pixels.data());

		return pixels;
	}

	IPickable* ContextEGL::get_object_by_pick_color(Color color)
	{
		return m_pickable_objects[ColorHelper::color_to_hexa_rgba(color)];
	}

	std::unique_ptr<PickId> ContextEGL::create_pick_id(IPickable* object)
	{
		unsigned int id = 0;

		if (!m_reusable_ids.empty())
		{
			id = m_reusable_ids.top();
			m_reusable_ids.pop();
		}
		else
		{
			id = ++m_next_id;
		}

		// Overflow
		if (id == 0)
		{
			throw std::out_of_range("Out of ids for picking objects.");
		}

		m_pickable_objects[id] = object;

		return std::unique_ptr<PickId>(new PickId(
			id,
			Color(id),
			object,
			this));
	}

	Texture2D* ContextEGL::get_default_texture2D()
	{
		if (m_default_texture2D == nullptr)
		{
			m_default_texture2D = Device::create_texture2D(TextureDescription(
				1,
				1,
				TextureFormat::RedGreenBlueAlpha8));

			std::unique_ptr<WritePixelBuffer> write_pixel = Device::create_write_pixel_buffer(
				PixelBufferHint::Stream,
				4);

			std::vector<vec4ub> pixel
			{ vec4ub(1) };
			write_pixel->copy_from_system(pixel);

			m_default_texture2D->copy_from_buffer(
				write_pixel.get(),
				ImageFormat::RedGreenBlueAlpha,
				ImageDataType::UnsignedByte);
		}

		return m_default_texture2D.get();
	}

	void ContextEGL::destroy_pick_id(PickId* pick_id)
	{
		m_reusable_ids.push(pick_id->id);
	}

	unsigned int ContextEGL::get_drawing_surface_width()
	{
		return m_surface->get_width();
	}

	unsigned int ContextEGL::get_drawing_surface_height()
	{
		return m_surface->get_height();
	}


	Surface* ContextEGL::get_surface()
	{
		return m_surface;
	}
	
	EGLContext ContextEGL::get_native_context()
	{
		return m_context;
	}
}