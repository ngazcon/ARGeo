#pragma once

namespace argeo
{
	///
	/// Buffer target for storage.
	///
	enum class BufferTarget : unsigned int
	{
		VertexArray		  = 0,
		ElementArray	  = 1,
		Uniform			  = 2,
		AtomicCounter	  = 3,
		CopyRead		  = 4,
		CopyWrite		  = 5,
		DrawIndirect      = 6,
		DispatchIndirect  = 7,
		PixelPack		  = 8,
		PixelUnPack		  = 9,
		Query			  = 10,
		ShaderStorage	  = 11,
		Texture			  = 12,
		TransformFeedback = 13,
	};
}