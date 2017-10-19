#pragma once

#include "Mesh.h"
#include "Vector.h"
#include "IndicesUnsignedShort.h"
#include "VertexAttributeDoubleVector3.h"

#include <memory>
#include <vector>

namespace argeo
{
	class BoxTessellator
	{
	public:

		static std::unique_ptr<Mesh> compute(vec3d length)
		{
			if (length.x < 0 || length.y < 0 || length.z < 0)
			{
				throw std::out_of_range("length");
			}

			Mesh* mesh = new Mesh();
			mesh->set_primitive_type(PrimitiveType::Triangles);
			mesh->set_front_face_winding_order(WindingOrder::CounterClockwise);

			VertexAttributeDoubleVector3* positions_attribute = new VertexAttributeDoubleVector3("position", 8);
			mesh->add_attribute(positions_attribute);

			IndicesUnsignedShort* indices = new IndicesUnsignedShort(36);
			mesh->set_indices(indices);

			//
			// 8 corner points
			//
			std::vector<vec3d> positions;

			vec3d corner = length * 0.5;
			positions.push_back(vec3d(-corner.x, -corner.y, -corner.z));
			positions.push_back(vec3d(corner.x, -corner.y, -corner.z));
			positions.push_back(vec3d(corner.x, corner.y, -corner.z));
			positions.push_back(vec3d(-corner.x, corner.y, -corner.z));
			positions.push_back(vec3d(-corner.x, -corner.y, corner.z));
			positions.push_back(vec3d(corner.x, -corner.y, corner.z));
			positions.push_back(vec3d(corner.x, corner.y, corner.z));
			positions.push_back(vec3d(-corner.x, corner.y, corner.z));

			positions_attribute->set_values(positions);

			//
			// 6 faces, 2 triangles each
			//
			indices->add_triangle(TriangleIndicesUnsignedShort((unsigned short)4, (unsigned short)5, (unsigned short)6));    // Top: plane z = corner.Z
			indices->add_triangle(TriangleIndicesUnsignedShort((unsigned short)4, (unsigned short)6, (unsigned short)7));
			indices->add_triangle(TriangleIndicesUnsignedShort((unsigned short)1, (unsigned short)0, (unsigned short)3));    // Bottom: plane z = -corner.Z
			indices->add_triangle(TriangleIndicesUnsignedShort((unsigned short)1, (unsigned short)3, (unsigned short)2));
			indices->add_triangle(TriangleIndicesUnsignedShort((unsigned short)1, (unsigned short)6, (unsigned short)5));    // Side: plane x = corner.X
			indices->add_triangle(TriangleIndicesUnsignedShort((unsigned short)1, (unsigned short)2, (unsigned short)6));
			indices->add_triangle(TriangleIndicesUnsignedShort((unsigned short)2, (unsigned short)3, (unsigned short)7));    // Side: plane y = corner.Y
			indices->add_triangle(TriangleIndicesUnsignedShort((unsigned short)2, (unsigned short)7, (unsigned short)6));
			indices->add_triangle(TriangleIndicesUnsignedShort((unsigned short)3, (unsigned short)0, (unsigned short)4));    // Side: plane x = -corner.X
			indices->add_triangle(TriangleIndicesUnsignedShort((unsigned short)3, (unsigned short)4, (unsigned short)7));
			indices->add_triangle(TriangleIndicesUnsignedShort((unsigned short)0, (unsigned short)1, (unsigned short)5));    // Side: plane y = -corner.Y
			indices->add_triangle(TriangleIndicesUnsignedShort((unsigned short)0, (unsigned short)5, (unsigned short)4));

			return std::unique_ptr<Mesh>(mesh);
		}
	};
}

