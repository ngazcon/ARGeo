#pragma once

#include "VertexAttributeType.h"

#include <string>

namespace argeo
{
	class IVertexAttribute
	{
		///
		/// Describes a vertex's attribute.
		///

	public:

		///
		/// Get the name of this uniform.
		/// It's also the uniform name in the shader program.
		/// @return {std::string} The name of this uniform.
		///
		virtual std::string get_name() const = 0;

		///
		/// Get the data type of this attribute.
		/// @return {VertexAttributeType} The data type of this uniform.
		/// 
		virtual VertexAttributeType get_type() const = 0;
	};
}
