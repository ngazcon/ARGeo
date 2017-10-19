#include "RectangleTessellator.h"
#include "VertexAttributeFloatVector2.h"
#include "IndicesUnsignedInt.h"

#include <stdexcept>
#include <vector>

namespace argeo
{
	std::unique_ptr<Mesh> RectangleTessellator::compute(
		RectangleD rectangle,
		int number_of_partitions_x,
		int number_of_partitions_y)
	{
		if (number_of_partitions_x < 0)
		{
			throw std::out_of_range("numberOfPartitionsX");
		}

		if (number_of_partitions_x < 0)
		{
			throw std::out_of_range("numberOfPartitionsY");
		}

		Mesh* mesh = new Mesh();
		mesh->set_primitive_type(PrimitiveType::Triangles);
		mesh->set_front_face_winding_order(WindingOrder::CounterClockwise);

		int number_of_positions = (number_of_partitions_x + 1) * (number_of_partitions_y + 1);
		VertexAttributeFloatVector2* positions_attribute = new VertexAttributeFloatVector2("position", number_of_positions);

		std::vector<vec2> positions;
		mesh->add_attribute(positions_attribute);

		int number_of_indices = (number_of_partitions_x * number_of_partitions_y) * 6;
		IndicesUnsignedInt* indices = new IndicesUnsignedInt(number_of_indices);

		mesh->set_indices(indices);

		// Positions
		vec2d lower_left	 = rectangle.get_lower_left();
		vec2d to_upper_right = rectangle.get_upper_right() - lower_left;

		for (int y = 0; y <= number_of_partitions_y; ++y)
		{
			double deltaY = y / (double)number_of_partitions_y;
			double currentY = lower_left.y + (deltaY * to_upper_right.y);

			for (int x = 0; x <= number_of_partitions_x; ++x)
			{
				double deltaX = x / (double)number_of_partitions_x;
				double currentX = lower_left.x + (deltaX * to_upper_right.x);

				positions.push_back(vec2(currentX, currentY));
			}
		}
		positions_attribute->set_values(positions);

		// Indices
		int row_delta = number_of_partitions_x + 1;
		int i = 0;
		for (int y = 0; y < number_of_partitions_y; ++y)
		{
			for (int x = 0; x < number_of_partitions_x; ++x)
			{
				indices->add_triangle(TriangleIndicesUnsignedInt(i, i + 1, row_delta + (i + 1)));
				indices->add_triangle(TriangleIndicesUnsignedInt(i, row_delta + (i + 1), row_delta + i));
				i += 1;
			}
			i += 1;
		}

		return std::unique_ptr<Mesh>(mesh);
	}
}
