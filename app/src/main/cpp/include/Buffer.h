#pragma once

#include "BufferHint.h"
#include "IndexDataType.h"

#include <vector>

namespace argeo
{
	class Buffer
	{
		///
		/// A buffer containing raw data.
		///

	public:

		///
		/// Gets the size in bytes of this buffer.
		/// @return {std::size_t} The size in bytes of this buffer.
		///
		virtual std::size_t get_size_in_bytes() const = 0;

		///
		/// Gets the usage hint for this buffer.
		/// It's a hint for the driver for improve performance.
		/// @return {BufferHint} The buffer hint.
		virtual BufferHint get_hint() const = 0;
	};
}
