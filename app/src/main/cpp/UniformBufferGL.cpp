#include "UniformBufferGL.h"
#include "TypeConverterGL.h"

#include "GL.h"

namespace argeo
{
	UniformBufferGL::UniformBufferGL(int size_in_bytes, BufferHint hint)
		: m_buffer_object(new BufferGL(BufferTarget::Uniform, hint, size_in_bytes))
	{ }

	UniformBufferGL::~UniformBufferGL()
	{ }

	void UniformBufferGL::copy_from_system(void* buffer, int stride_in_bytes, int size_in_bytes)
	{
		m_buffer_object->copy_from_system(buffer, stride_in_bytes, size_in_bytes);
	}

	void UniformBufferGL::copy_from_device(void* buffer, int stride_in_bytes, int size_in_bytes)
	{
		m_buffer_object->copy_from_device(buffer, stride_in_bytes, size_in_bytes);
	}

	int UniformBufferGL::get_size_in_bytes() const
	{
		return m_buffer_object->get_size_in_bytes();
	}

	void* UniformBufferGL::map_range(
		int offset_in_bytes,
		int size_in_bytes,
		BufferAccess access)
	{
		return glMapBufferRange(
			GL_UNIFORM_BUFFER,
			offset_in_bytes,
			size_in_bytes,
			TypeConverterGL::ToEnum(access));
	}

	void UniformBufferGL::unmap()
	{
		glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
	}

	BufferHint UniformBufferGL::get_hint() const
	{
		return m_buffer_object->get_hint();
	}

	unsigned int UniformBufferGL::get_handle() const
	{
		return m_buffer_object->get_handle();
	}

}