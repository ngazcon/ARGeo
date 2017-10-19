#pragma once

namespace argeo
{
	enum class ClearBuffers : unsigned int
	{
		ColorBuffer = 1,
		DepthBuffer = 2,
		StencilBuffer = 4,
		ColorAndDepthBuffer   = ColorBuffer | DepthBuffer,
		DepthAndStencilBuffer = DepthBuffer | StencilBuffer,
		ColorAndStencilBuffer = ColorBuffer | StencilBuffer,
		All = ColorBuffer | DepthBuffer | StencilBuffer
	};

	inline ClearBuffers operator&(ClearBuffers lhs, ClearBuffers rhs)
	{
		return (ClearBuffers)(static_cast<int>(lhs)& static_cast<int>(rhs));
	}

	inline ClearBuffers operator|(ClearBuffers lhs, ClearBuffers rhs)
	{
		return (ClearBuffers)(static_cast<int>(lhs) | static_cast<int>(rhs));
	}

	inline ClearBuffers& operator|=(ClearBuffers& lhs, ClearBuffers rhs)
	{
		lhs = (ClearBuffers)(static_cast<int>(lhs) | static_cast<int>(rhs));
		return lhs;
	}
}
		