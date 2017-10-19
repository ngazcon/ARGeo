#include "DepthPlane.h"
#include "DepthPlaneShaders.h"
#include "VertexAttributeFloatVector3.h"
#include "IndicesUnsignedShort.h"

namespace argeo
{
	DepthPlane::DepthPlane()
		: m_draw_command(nullptr)
		, m_vertex_array(nullptr)
		, m_shader_program(nullptr)
		, m_render_state(nullptr)
	{ }


	DepthPlane::~DepthPlane()
	{
		delete m_render_state;
		delete m_shader_program;
		delete m_draw_command->get_bounding_volume();
	}

	std::vector<vec3> DepthPlane::compute_depth_quad(
		const Ellipsoid& ellipsoid,
		FrameState* frame_state)
	{
		std::vector<vec3> depth_quad;

		const vec3d radii = ellipsoid.get_radii();
		const vec3d p = frame_state->get_camera()->get_position_wc();
		const vec3d q = ellipsoid.get_one_over_radii_squared() * p;

		const double q_magnitude = vec_length(q);
		const vec3d q_unit = vec_normalize(q);

		// Determine the east and north directions at q.
		const vec3d e_unit = vec_normalize(vec_cross(vec3d::unit_z(), q));
		const vec3d n_unit = vec_normalize(vec_cross(q_unit, e_unit));

		// Determine the radius of the 'limb' of the ellipsoid.
		const double w_magnitude = ::sqrt(vec_length_squared(q) - 1.0);

		// Compute the center and offsets.
		const vec3d center  = q_unit * 1.0 / q_magnitude;
		const double scalar = w_magnitude  / q_magnitude;
		const vec3d east_offset  = e_unit * scalar;
		const vec3d north_offset = n_unit * scalar;

		// A conservative measure for the longitudes would be to use the min/max longitudes of the bounding frustum.
		vec3d upper_left = center + north_offset;
		upper_left -= east_offset;
		upper_left *= radii;
		depth_quad.push_back(to_vec3(upper_left));

		vec3d lower_left = center - north_offset;
		lower_left -= east_offset;
		lower_left *= radii;
		depth_quad.push_back(to_vec3(lower_left));

		vec3d upper_right = center + north_offset;
		upper_right += east_offset;
		upper_right *= radii;
		depth_quad.push_back(to_vec3(upper_right));

		vec3d lower_right = center - north_offset;
		lower_right += east_offset;
		lower_right *= radii;
		depth_quad.push_back(to_vec3(lower_right));

		return depth_quad;
	}

	void DepthPlane::update(FrameState* frame_state)
	{
		Context* context    = frame_state->get_context();
        Ellipsoid ellipsoid = frame_state->get_map_projection()->get_ellipsoid();

        if (m_draw_command == nullptr)
		{
			// Write depth, not color
			RenderStateParameters params;
			params.facet_culling.enabled = true;
			params.depth_test.enabled    = true;
			params.depth_test.function   = DepthTestFunction::Always;
			params.color_mask            = ColorMask(false, false, false, false);

			m_render_state = RenderState::from_cache(params);

			m_shader_program = context->get_shader_cache()->get_shader_program(
				depth_plane_vs,
				depth_plane_fs);

			m_draw_command = std::unique_ptr<DrawCommand>(new DrawCommand(PrimitiveType::Triangles));
			m_draw_command->set_bounding_volume(new BoundingSphere(vec3d::zero(), ellipsoid.get_maximum_radius()));
			m_draw_command->set_shader_program(m_shader_program);
			m_draw_command->set_render_state(m_render_state);
        }

        // update depth plane
        std::vector<vec3> depth_quad = compute_depth_quad(ellipsoid, frame_state);

        // depth plane
        if (m_vertex_array == nullptr) 
		{
			std::unique_ptr<Mesh> mesh(new Mesh());
			mesh->set_primitive_type(PrimitiveType::Triangles);
			mesh->set_front_face_winding_order(WindingOrder::CounterClockwise);

			VertexAttributeFloatVector3* positions_attribute = new VertexAttributeFloatVector3("position", 4);
			positions_attribute->set_values(depth_quad);
			mesh->add_attribute(positions_attribute);

			IndicesUnsignedShort* indices = new IndicesUnsignedShort(6);
			indices->add_triangle(TriangleIndicesUnsignedShort(
				static_cast<unsigned short>(0), 
				static_cast<unsigned short>(1), 
				static_cast<unsigned short>(2)));
			indices->add_triangle(TriangleIndicesUnsignedShort(
				static_cast<unsigned short>(2),
				static_cast<unsigned short>(1),
				static_cast<unsigned short>(3)));
			mesh->set_indices(indices);

			m_vertex_array = context->create_vertex_array(
				mesh.get(),
				m_shader_program->get_attributes(),
				BufferHint::DynamicDraw);

			m_draw_command->set_vertex_array(m_vertex_array.get());
        }
		else
		{
			m_vertex_array->get_attributes()->get_attribute(0)->get_vertex_buffer()->copy_from_system(depth_quad);
        }
	}

	void DepthPlane::execute(
		Context* context,
		PassState* pass_state)
	{
		m_draw_command->execute(
			context, 
			pass_state);
	}
}