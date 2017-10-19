#pragma once

#include "StencilTestFunction.h"
#include "StencilOperation.h"

namespace argeo
{
	class StencilTestFace
	{

	public:

		StencilTestFace(
			StencilOperation stencil_fail_operation			   = StencilOperation::Keep,
			StencilOperation depth_fail_stencil_pass_operation = StencilOperation::Keep,
			StencilOperation depth_pass_stencil_pass_operation = StencilOperation::Keep,
			StencilTestFunction function					   = StencilTestFunction::Always,
			int reference_value								   = 0,
			unsigned int mask								   = 0xFF)
			: stencil_fail_operation(stencil_fail_operation)
			, depth_fail_stencil_pass_operation(depth_fail_stencil_pass_operation)
			, depth_pass_stencil_pass_operation(depth_pass_stencil_pass_operation)
			, function(function)
			, reference_value(reference_value)
			, mask(mask)
		{ }

		StencilOperation stencil_fail_operation;
		StencilOperation depth_fail_stencil_pass_operation;
		StencilOperation depth_pass_stencil_pass_operation;

		StencilTestFunction function;
		int reference_value;
		unsigned int mask;
	};
}