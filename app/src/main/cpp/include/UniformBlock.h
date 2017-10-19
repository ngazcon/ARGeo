#pragma once

#include "UniformBlockMemberCollection.h"
#include "UniformBuffer.h"

#include <string>

namespace argeo
{
	class UniformBlock
	{
	public:

		// The name of the uniform block.
		virtual std::string get_name() const = 0;

		// The binding point of the uniform block.
		// It's works in the same way of texture units.
		// For example you have to establish a binding within the location of the texture sampler (uniform block index) and the texture unit (binding point).
		virtual int  get_binding_point() const = 0;
		virtual void set_binding_point(int value) = 0;

		// The location of the block in the shader layout.
		virtual int get_location() const = 0;

		// The size of the uniform block data in machine units.
		virtual int get_data_size() const = 0;

		// Bind the uniform buffer to this block.
		virtual void bind(UniformBuffer* buffer) = 0;

		virtual void bind(
			UniformBuffer* buffer,
			int offset_in_bytes,
			int size_in_bytes) = 0;

		// The members collection of this block.
		virtual UniformBlockMemberCollection* get_members() = 0;
	};
}

