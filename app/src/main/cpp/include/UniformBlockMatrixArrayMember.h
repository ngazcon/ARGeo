#pragma once

#include "UniformBlockMatrixMember.h"

namespace argeo
{
	class UniformBlockMatrixArrayMember : public UniformBlockMatrixMember
	{
	public:

		UniformBlockMatrixArrayMember(std::string name, int resource_size, int offset_in_bytes, int matrix_stride, bool row_major, int array_stride, UniformType type)
			: UniformBlockMatrixMember(name, offset_in_bytes, matrix_stride, row_major, type)
			, m_size(resource_size)
			, m_array_stride(array_stride)
		{ }

		~UniformBlockMatrixArrayMember()
		{ }

		// For uniforms in named uniform blocks and for uniforms declared as atomic counters, the stride is the difference, in basic machine units, of consecutive elements in an array, or zero for uniforms not declared as an array.
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
