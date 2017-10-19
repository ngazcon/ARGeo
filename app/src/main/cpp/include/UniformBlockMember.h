#pragma once

#include "UniformType.h"

#include <string>

namespace argeo
{
	class UniformBlockMember
	{
	public:

		UniformBlockMember(std::string name, int offset, UniformType type)
			: m_name(name)
			, m_offset(offset)
			, m_type(type)
		{ }

		~UniformBlockMember()
		{ }

		std::string get_name() const
		{
			return m_name;
		}

		// For uniforms in a named uniform block, the returned value will be its offset, in basic machine units, relative to the beginning of the uniform block in the buffer object data store.
		int offset() const
		{
			return m_offset;
		}

	private:

		std::string m_name;

		UniformType m_type;

		int m_offset;
	};
}
