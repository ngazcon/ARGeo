#pragma once

#include "UniformBlockMember.h"

namespace argeo
{
	class UniformBlockArrayMember : public UniformBlockMember
	{
	public:

		UniformBlockArrayMember(std::string name, int resource_size, int offset_in_bytes, int array_stride, UniformType type)
			: UniformBlockMember(name, offset_in_bytes, type)
			, m_size(resource_size)
			, m_array_stride(array_stride)
		{ }

		~UniformBlockArrayMember()
		{ }

		// For uniforms in named uniform blocks and for uniforms declared as atomic counters, the stride is the difference, 
		// in basic machine units, of consecutive elements in an array, or zero for uniforms not declared as an array.
		int array_stride() const
		{
			return m_array_stride;
		}

		int size() const
		{
			return m_size;
		}

	private:

		int m_array_stride;

		int m_size;
	};
}
