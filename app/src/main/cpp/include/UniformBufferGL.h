#pragma once

#include "UniformBuffer.h"
#include "BufferHint.h"
#include "BufferGL.h"

#include <memory>

namespace argeo
{
	class UniformBufferGL : public UniformBuffer
	{
	public:

		UniformBufferGL(int size_in_bytes, BufferHint hint);

		~UniformBufferGL();

		void copy_from_system(void* buffer, int offset_in_bytes, int size_in_bytes);

		void copy_from_device(void* buffer, int offset_in_bytes, int size_in_bytes);

		void* map_range(
			int offset_in_bytes,
			int size_in_bytes,
			BufferAccess access);

		void unmap();

		int get_size_in_bytes() const;

		BufferHint get_hint() const;

		unsigned int get_handle() const;

	private:		
		std::unique_ptr<BufferGL> m_buffer_object;
	};
}
