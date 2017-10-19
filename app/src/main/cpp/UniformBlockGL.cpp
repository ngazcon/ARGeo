#include "UniformBlockGL.h"
#include "UniformBufferGL.h"

#include "GL.h"

namespace argeo
{
	UniformBlockGL::UniformBlockGL(std::string name, int location, int binding_point, int data_size)
		: m_name(name)
		, m_location(location)
		, m_binding_point(binding_point)
		, m_data_size(data_size)
		, m_members(new UniformBlockMemberCollection())
	{ 
		// UBOs must be bound with aligned length and offset, and it varies per vendor.
		glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &m_uniform_buffer_align);
	}


	UniformBlockGL::~UniformBlockGL()
	{ }

	void UniformBlockGL::bind(UniformBuffer* buffer)
	{
		unsigned int buffer_handle = 
			static_cast<UniformBufferGL*>(buffer)->get_handle();

		glBindBufferBase(
			GL_UNIFORM_BUFFER,
			m_binding_point,
			buffer_handle);
	}

	void UniformBlockGL::bind(
		UniformBuffer* buffer,
		int offset_in_bytes,
		int size_in_bytes)
	{
		unsigned int buffer_handle =
			static_cast<UniformBufferGL*>(buffer)->get_handle();

		glBindBufferRange(
			GL_UNIFORM_BUFFER,
			m_binding_point,
			buffer_handle,
			offset_in_bytes,
			realign_offset(size_in_bytes, m_uniform_buffer_align));
	}

	
	unsigned int UniformBlockGL::realign_offset(std::size_t offset, std::size_t align)
	{
		return (offset + align - 1) & ~(align - 1);
	}

	// Getters and Setters
	std::string UniformBlockGL::get_name() const
	{
		return m_name;
	}

	int UniformBlockGL::get_data_size() const
	{
		return m_data_size;
	}

	int UniformBlockGL::get_location() const
	{
		return m_location;
	}

	int UniformBlockGL::get_binding_point() const
	{
		return m_binding_point;
	}

	void UniformBlockGL::set_binding_point(int value)
	{
		m_binding_point = value;
	}

	UniformBlockMemberCollection* UniformBlockGL::get_members()
	{
		return m_members.get();
	}
}