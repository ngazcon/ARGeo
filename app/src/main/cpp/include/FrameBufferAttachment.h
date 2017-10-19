#pragma once

namespace argeo
{
	enum class FrameBufferAttachment : int
	{
		ColorAttachment0       = 0,
		ColorAttachment1       = 1,
		ColorAttachment2       = 2,
		ColorAttachment3       = 3,
		ColorAttachment4       = 4,
		ColorAttachment5       = 5,
		ColorAttachment6       = 6,
		ColorAttachment7       = 7,
		ColorAttachment8       = 8,
		ColorAttachment9       = 9,
		ColorAttachment10      = 10,
		ColorAttachment11      = 11,
		ColorAttachment12      = 12,
		ColorAttachment13      = 13,
		ColorAttachment14      = 14,
		ColorAttachment15      = 15,
		DepthAttachment        = 16,
		DepthStencilAttachment = 17,
		StencilAttachment      = 18,
	};

	inline FrameBufferAttachment operator+(FrameBufferAttachment a, int b)
	{
		return static_cast<FrameBufferAttachment>(static_cast<int>(a) + static_cast<int>(b));
	}
}
