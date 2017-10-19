#pragma once

namespace argeo
{
	///
	/// Primitive type of a mesh.
	///
	enum class PrimitiveType : unsigned int
	{
		Points					= 0,
		Triangles				= 1,
		TrianglesAdjacency		= 2,
		TriangleStrip			= 3,
		TriangleStripAdjacency	= 4,
		TriangleFan				= 5,
		Lines					= 6,
		LinesAdjacency			= 7,
		LineStrip				= 8,
		LineStripAdjacency		= 9,
		LineLoop				= 10,
		Quads					= 11,
		QuadStrip				= 12,
	};
}
