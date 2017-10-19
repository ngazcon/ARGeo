#pragma once

#include "ShaderObjectType.h"

#include <string>

namespace argeo
{
	/*
	 * A shader object represents one programmable stage of the pipeline and is a GL renderer implementation detail.
	 * It is not part of the renderer abstraction, which represents all stages together in one shader program.
	 * It can be useful to expose interfaces for individual pipeline stages, especially when using ARB separate shader objects, but we did not have the need
	 */
	class ShaderObject
	{
	public:

		virtual ~ShaderObject() { }

		// The shader type. (e.g. Vertex Shader, Fragment Shader)
		virtual ShaderObjectType get_type() const = 0;

		// The source code that is compiled for this shader.
		virtual std::string get_source() const = 0;
	};

}
