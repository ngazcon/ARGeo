#pragma once

#include "UniformType.h"
#include "ICleanable.h"

#include <string>

namespace argeo
{
	/*
	 * Uniform variables are a way to provide data to any shader stage that vary only up to every primitive.
	 */
	class IUniform : public ICleanable
	{
	public:

		// The name of this uniform.
		// It's also the uniform name in the shader program.
		virtual std::string get_name() const = 0;
		

		virtual void copy_value(IUniform* to_copy) = 0;

		// The data type of this uniform.
		virtual UniformType get_type() const = 0;

		// The location of this uniform within the shader program.
		virtual int get_location() const = 0;
	};
}
