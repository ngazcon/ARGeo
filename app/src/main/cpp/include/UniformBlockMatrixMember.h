#pragma once
#include "UniformBlockMember.h"

namespace argeo
{
	class UniformBlockMatrixMember : public UniformBlockMember
	{
	public:

		UniformBlockMatrixMember(std::string name, int offset_in_bytes, int matrix_stride, bool row_major, UniformType type)
			: UniformBlockMember(name, offset_in_bytes, type),
			  m_matrix_stride(matrix_stride),
			  m_row_major(row_major)
		{ }

		~UniformBlockMatrixMember()
		{ }

		// Identify the stride between columns of a column-major matrix or rows of a row-major matrix, in basic machine units.
		int matrix_stride() const
		{
			return m_matrix_stride;
		}

		bool row_major() const
		{
			return m_row_major;
		}

	private:

		int m_matrix_stride;

		bool m_row_major;
	};
}