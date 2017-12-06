#include "PlanePrimitive.h"
#include "Device.h"
#include "PlaneRenderShaders.h"

#include "VertexAttributeDoubleVector3.h"
#include "VertexAttributeFloatVector2.h"
#include "VertexAttributeFloatVector3.h"

#include "IndicesUnsignedShort.h"
#include "Scene.h"

#include <stdexcept>
#include <cmath>

namespace argeo
{
	const ShaderVertexAttributeLocations PlanePrimitive::attribute_locations =
			{
					{ "positionHigh",   0 },
					{ "positionLow",    1 },
					{ "normal",           2 },
			};

	PlanePrimitive::PlanePrimitive(
			Scene* scene,
			EllipsoidTangentPlane tangent_plane,
			const bool& flag)
			: PlanePrimitive(
			scene,
			tangent_plane.get_origin().to_vec3d(),
			tangent_plane.get_x_axis().to_vec3d(),
			tangent_plane.get_y_axis().to_vec3d(),
			tangent_plane.get_normal().to_vec3d(),
			flag)
	{ }


	PlanePrimitive::PlanePrimitive(
			Scene* scene,
			vec3d origin,
			vec3d x_axis,
			vec3d y_axis,
			vec3d normal,
			const bool& flag)
			: m_origin(origin)
			, m_color(flag ? Color(255, 0, 0, 255) : Color(0, 0, 255, 255))
			, m_x_axis(x_axis)
			, m_y_axis(y_axis)
			, m_normal(normal)
			, m_has_outline(true)
			, m_allow_picking(true)
			, m_outline_stroke(1.1)
			, m_width(100.0)
			, m_height(20.0)
			, m_show(true)
			, m_scene(scene)
			, m_model_matrix(mat4d::identity())
			, m_orientation(quaternion::identity())
			, m_scale(vec3d(1.0, 1.0, 1.0))
			, m_model_matrix_changed(false)
			, m_height_clamping(HeightClamping::Ground)
			, m_orientation_clamping(OrientationClamping::None)
			, m_height_changed(true)
	{
		m_render_parameters.blending.enabled   = true;
		m_render_parameters.depth_test.enabled  = true;
		m_render_parameters.depth_test.function = DepthTestFunction::LessOrEqual;

		m_render_parameters.facet_culling = FacetCulling(
				true,
				CullFace::Back,
				WindingOrder::CounterClockwise);
	}

	PlanePrimitive::~PlanePrimitive()
	{ }

	void PlanePrimitive::create_vertex_array(Context* context)
	{
		m_render_resources.create_vertex_array = false;

		std::vector<vec3d> positions =
				{
						// Bottom face
						vec_normalize(- m_x_axis - m_y_axis) * m_width * 0.5,
						vec_normalize(  m_x_axis - m_y_axis) * m_width * 0.5,
						vec_normalize(  m_x_axis + m_y_axis) * m_width * 0.5,
						vec_normalize(- m_x_axis + m_y_axis) * m_width * 0.5,


						vec_normalize(- m_x_axis - m_y_axis) * m_width * 0.5 + m_normal * m_height,
						vec_normalize(  m_x_axis - m_y_axis) * m_width * 0.5 + m_normal * m_height,
						vec_normalize(  m_x_axis + m_y_axis) * m_width * 0.5 + m_normal * m_height,
						vec_normalize(- m_x_axis + m_y_axis) * m_width * 0.5 + m_normal * m_height,
				};

		std::vector<vec3> normals =
				{
						to_vec3(-m_normal),
						to_vec3(-m_normal),
						to_vec3(-m_normal),
						to_vec3(-m_normal),

						to_vec3(m_normal),
						to_vec3(m_normal),
						to_vec3(m_normal),
						to_vec3(m_normal),
				};

		std::vector<vec3> positions_and_normals;
		for (unsigned int i = 0; i < positions.size(); i++)
		{
			evec3d eposition(positions[i]);

			positions_and_normals.push_back(eposition.high);
			positions_and_normals.push_back(eposition.low);
			positions_and_normals.push_back(normals[i]);
		}

		m_render_resources.positions_and_normals_buffer = Device::create_vertex_buffer(
				positions_and_normals,
				BufferHint::StaticDraw);

		VertexBufferAttribute* position_high_attribute = new VertexBufferAttribute(
				m_render_resources.positions_and_normals_buffer.get(),
				ComponentDataType::Float,
				3,
				false,
				0,
				3 * sizeof(vec3));

		VertexBufferAttribute* position_low_attribute = new VertexBufferAttribute(
				m_render_resources.positions_and_normals_buffer.get(),
				ComponentDataType::Float,
				3,
				false,
				sizeof(vec3),
				3 * sizeof(vec3));

		VertexBufferAttribute* normal_attribute = new VertexBufferAttribute(
				m_render_resources.positions_and_normals_buffer.get(),
				ComponentDataType::Float,
				3,
				false,
				2 * sizeof(vec3),
				3 * sizeof(vec3));

		std::vector<unsigned short> indices =
				{
						// Top Face
						(unsigned short)(4),
						(unsigned short)(5),
						(unsigned short)(6),
						(unsigned short)(6),
						(unsigned short)(7),
						(unsigned short)(4),

						// Bottom Face
						(unsigned short)(2),
						(unsigned short)(1),
						(unsigned short)(0),
						(unsigned short)(2),
						(unsigned short)(0),
						(unsigned short)(3),

						// Left Face
						(unsigned short)(4),
						(unsigned short)(7),
						(unsigned short)(3),
						(unsigned short)(3),
						(unsigned short)(0),
						(unsigned short)(4),


						// Right Face
						(unsigned short)(2),
						(unsigned short)(6),
						(unsigned short)(5),
						(unsigned short)(5),
						(unsigned short)(1),
						(unsigned short)(2),

						// Front Face
						(unsigned short)(0),
						(unsigned short)(1),
						(unsigned short)(5),
						(unsigned short)(5),
						(unsigned short)(4),
						(unsigned short)(0),


						// Back Face
						(unsigned short)(3),
						(unsigned short)(7),
						(unsigned short)(6),
						(unsigned short)(6),
						(unsigned short)(2),
						(unsigned short)(3),
				};

		IndexBuffer* index_buffer = Device::create_index_buffer(
				BufferHint::StaticDraw,
				indices.size() * sizeof(unsigned short)).release();
		index_buffer->copy_from_system(indices);

		m_render_resources.vertex_array = context->create_vertex_array(
				VertexBufferAttributes
						{
								{ attribute_locations.at("positionLow"),  position_low_attribute },
								{ attribute_locations.at("positionHigh"), position_high_attribute },
								{ attribute_locations.at("normal"),         normal_attribute },
						},
				index_buffer);

		m_render_resources.vertex_array->set_dispose_buffers(true);
		m_base_bounding_sphere = BoundingSphere::from_corner_points(positions[0], positions[7]);
	}

	void PlanePrimitive::create_render_states()
	{
		RenderStateParameters render_params = m_render_parameters;

		if (false)//m_has_outline)
		{
			render_params.stencil_mask = 0xFF;
			render_params.stencil_test.enabled = true;
			render_params.stencil_test.front_face = StencilTestFace(
					StencilOperation::Keep,
					StencilOperation::Keep,
					StencilOperation::Replace,
					StencilTestFunction::Always,
					1,
					0xFF);

			RenderStateParameters outline_render_parameters = render_params;

			outline_render_parameters.depth_test.enabled = false;
			outline_render_parameters.stencil_mask = 0x00;
			outline_render_parameters.stencil_test.front_face.mask = 0xFF;
			outline_render_parameters.stencil_test.front_face.function = StencilTestFunction::NotEqual;

			m_render_resources.outline_render_state = RenderState::from_cache(outline_render_parameters);
		}

		m_render_resources.render_state = RenderState::from_cache(render_params);

		m_render_resources.create_render_states = false;
	}

	void PlanePrimitive::update(FrameState* frame_state)
	{
		Context* context = frame_state->get_context();

		if (m_render_resources.create_render_states)
		{
			create_render_states();
		}

		if (m_render_resources.create_vertex_array)
		{
			create_vertex_array(context);
		}

		if (m_show)
		{
			if (frame_state->get_pass() == Pass::Render)
			{
				if (m_render_resources.shader_program == nullptr)
				{
					m_render_resources.shader_program = context->get_shader_cache()->get_shader_program(
							plane_vs,
							plane_fs,
							attribute_locations);
				}

				if (m_render_resources.uniform_map == nullptr)
				{
					m_render_resources.uniform_map = std::unique_ptr<UniformMap>(new UniformMap
							{
									{
											"u_color",
											[&](IUniform* uniform)
											{ static_cast<Uniform<vec4>*>(uniform)->set_value(ColorHelper::color_to_vec4(m_color)); }
									}
							});
				}

				if (m_draw_command == nullptr)
				{
					m_draw_command = std::unique_ptr<DrawCommand>(
							new DrawCommand(
									PrimitiveType::Triangles,
									m_render_resources.vertex_array.get(),
									m_render_resources.shader_program,
									m_render_resources.render_state,
									nullptr,
									m_render_resources.uniform_map.get()));
					m_draw_command->set_pass(CommandPass::OverlayCommand);
				}

				const bool mode_changed = m_mode != frame_state->get_mode();
				m_mode = frame_state->get_mode();

				bool model_matrix_hierarchy_changed =
						m_model_matrix_changed ||
						m_height_changed ||
						mode_changed;

				if (model_matrix_hierarchy_changed)
				{
					update_clamping();

					mat4d model_matrix = m_height_clamping == HeightClamping::Ground ?
										 m_clamped_model_matrix :
										 m_model_matrix;

					m_computed_model_matrix = model_matrix;

					m_height_changed = false;
				}

				m_draw_command->set_model_matrix(m_computed_model_matrix);
				m_draw_command->set_bounding_volume(
						m_base_bounding_sphere->transform(m_computed_model_matrix)->clone(m_draw_command->get_bounding_volume()));
				m_draw_command->set_cull(true);

				frame_state->get_command_queue().push_back(
						m_draw_command.get());

				if (m_has_outline && get_is_picked())
				{
					if (m_render_resources.outline_shader_program == nullptr)
					{
						m_render_resources.outline_shader_program = context->get_shader_cache()->get_shader_program(
								plane_outline_vs,
								plane_outline_fs,
								attribute_locations);
					}

					if (m_render_resources.outline_uniform_map == nullptr)
					{
						m_render_resources.outline_uniform_map = std::unique_ptr<UniformMap>(new UniformMap());
						(*m_render_resources.outline_uniform_map)["og_outlineColor"] = [&](IUniform* uniform)
						{
							static_cast<Uniform<vec3>*>(uniform)->set_value(
									to_vec3(ColorHelper::color_to_vec4(ColorHelper::black_color())));
						};
					}

					if (m_outline_command == nullptr)
					{
						m_outline_command = std::unique_ptr<DrawCommand>(
								new DrawCommand(
										PrimitiveType::Triangles,
										m_render_resources.vertex_array.get(),
										m_render_resources.outline_shader_program,
										m_render_resources.outline_render_state,
										nullptr,
										m_render_resources.outline_uniform_map.get()));
					}


					mat4d scale_matrix = mat4d::identity();

					if (m_x_axis.x != 0.0)
					{
						scale_matrix *= mat_scale(
								m_x_axis.x * m_outline_stroke,
								1.0,
								1.0);
					}
					if (m_x_axis.y != 0.0)
					{
						scale_matrix *= mat_scale(
								1.0,
								m_x_axis.y * m_outline_stroke,
								1.0);
					}
					if (m_x_axis.z != 0.0)
					{
						scale_matrix *= mat_scale(
								1.0,
								1.0,
								m_x_axis.z * m_outline_stroke);
					}

					if (m_y_axis.x != 0.0)
					{
						scale_matrix *= mat_scale(
								m_y_axis.x * m_outline_stroke,
								1.0,
								1.0);
					}
					if (m_y_axis.y != 0.0)
					{
						scale_matrix *= mat_scale(
								1.0,
								m_y_axis.y * m_outline_stroke,
								1.0);
					}
					if (m_y_axis.z != 0.0)
					{
						scale_matrix *= mat_scale(
								1.0,
								1.0,
								m_y_axis.z * m_outline_stroke);
					}

					m_outline_command->set_model_matrix(m_computed_model_matrix * scale_matrix);

					// TODO: OUTLINE
					/*
                    frame_state->get_command_queue().push_back(
                       m_outline_command.get());
                     */
				}
			}
			else
			{
				if (m_render_resources.pick_shader_program == nullptr)
				{
					m_render_resources.pick_shader_program = context->get_shader_cache()->get_shader_program(
							plane_vs,
							Device::create_fragment_pick_shader_source(plane_fs, "uniform"),
							attribute_locations);
				}

				if (m_render_resources.pick_id == nullptr)
				{
					m_render_resources.pick_id = context->create_pick_id(this).release();
				}

				if(m_render_resources.pick_shader_program == nullptr)
				{
					m_render_resources.pick_uniform_map = std::unique_ptr<UniformMap>(new UniformMap());
					(*m_render_resources.pick_uniform_map)["og_pickColor"] = [&](IUniform* uniform)
					{
						static_cast<Uniform<vec4>*>(uniform)->set_value(
								ColorHelper::color_to_vec4(m_render_resources.pick_id->color));
					};
				}

				if (m_pick_command == nullptr)
				{
					m_pick_command = std::unique_ptr<DrawCommand>(
							new DrawCommand(
									PrimitiveType::Triangles,
									m_render_resources.vertex_array.get(),
									m_render_resources.pick_shader_program,
									m_render_resources.render_state,
									nullptr,
									m_render_resources.pick_uniform_map.get()));
				}

				frame_state->get_command_queue().push_back(
						m_pick_command.get());
			}
		}
	}

	void PlanePrimitive::update_clamping()
	{
		if (m_remove_update_height_callback = nullptr)
		{
			m_remove_update_height_callback();
			m_remove_update_height_callback = nullptr;
		}

		ITerrain* terrain   = m_scene->get_terrain();
		Ellipsoid ellipsoid = terrain->get_ellipsoid();

		if (m_height_clamping == HeightClamping::None)
		{
			return;
		}


		Geodetic3D geodetic_position = ellipsoid.to_geodetic3D(m_origin);
		m_update_height_function = [&, terrain, geodetic_position](vec3d& position)
		{
			/*
            mat4d normalize_center_orientate_matrix = mat4d::identity();

            if (m_height_clamping == HeightClamping::Ground)
            {
                Ellipsoid ellipsoid = terrain->get_ellipsoid();
                Geodetic3D geodetic = ellipsoid.to_geodetic3D(position);
                geodetic = Geodetic3D(
                        geodetic.get_longitude(),
                        geodetic.get_latitude(),
                        geodetic.get_height() + geodetic_position.get_height());
                m_clamped_origin = ellipsoid.to_geocentric3D(geodetic).to_vec3d();

                if (m_orientation_clamping != OrientationClamping::None && m_mode == SceneMode::Scene3D)
                {
                    Geocentric3D normal = ellipsoid.geodetic_surface_normal(geodetic);
                    m_orientation = quaternion::from_two_vectors(
                            vec3d::unit_y(),
                            normal.to_vec3d());

                    // Orientate the model with respect the reference.
                    normalize_center_orientate_matrix *=
                            mat4d(quat_to_matrix(m_orientation));
                }
            }
            */

			m_clamped_model_matrix = get_model_matrix();
			m_clamped_model_matrix.data[12] = m_origin.x;
			m_clamped_model_matrix.data[13] = m_origin.y;
			m_clamped_model_matrix.data[14] = m_origin.z;

			m_height_changed = true;
		};

		m_remove_update_height_callback = terrain->get_raster_source()->add_listener(
				m_update_height_function,
				geodetic_position);

		// Set the correct height now
		double height = terrain->get_point_height(Geodetic2D(
				geodetic_position.get_longitude(),
				geodetic_position.get_latitude()));

		if (!std::isnan(height))
		{
			// Compute the clamped cartesian and call updateHeight callback
			Geodetic3D geodetic(
					geodetic_position.get_longitude(),
					geodetic_position.get_latitude(),
					height);
			vec3d position = ellipsoid.to_geocentric3D(geodetic).to_vec3d();
			m_update_height_function(position);
		}
	}

	void PlanePrimitive::update_model_matrix()
	{
		if (m_model_matrix_changed)
		{
			m_model_matrix =
					mat_translate(
							m_translation.x,
							m_translation.y,
							m_translation.z)
					* mat4d(quat_to_matrix(m_rotation))
					* mat_scale(
							m_scale.x,
							m_scale.y,
							m_scale.z);

			m_model_matrix_changed = false;
		}
	}

	mat4d PlanePrimitive::get_model_matrix()
	{
		update_model_matrix();
		return m_model_matrix;
	}

	void PlanePrimitive::set_euler_angles(
			double pitch,
			double yaw,
			double roll)
	{
		set_rotation(quaternion::from_yaw_pitch_roll(yaw, pitch, roll));

	}
	void PlanePrimitive::set_translation(vec3d translation)
	{
		set_translation(
				translation.x,
				translation.y,
				translation.z);
	}

	void PlanePrimitive::translate(vec3d translation)
	{
		translate(
				translation.x,
				translation.y,
				translation.z);
	}

	void PlanePrimitive::set_translation(
			double x,
			double y,
			double z)
	{
		vec3d translation = vec3d(x, y, z);
		if (m_translation != translation)
		{
			m_model_matrix_changed = true;
			m_translation = translation;
		}
	}

	void PlanePrimitive::translate(
			double x,
			double y,
			double z)
	{
		vec3d translation = vec3d(x, y, z);
		m_model_matrix_changed = true;
	}

	void PlanePrimitive::set_rotation(quaternion rotation)
	{
		if (m_rotation != rotation)
		{
			m_model_matrix_changed = true;
			m_rotation = rotation;
		}
	}

	void PlanePrimitive::rotate(quaternion rotation)
	{
		m_model_matrix_changed = true;
		m_rotation *= rotation;
	}

	void PlanePrimitive::set_scale(
			const double& x,
			const double& y,
			const double& z)
	{
		vec3d scale(x, y, z);
		if (m_scale != scale)
		{
			m_model_matrix_changed = true;
			m_scale = scale;
		}
	}

	quaternion PlanePrimitive::get_rotation() const
	{
		return m_rotation;
	}

	vec3d PlanePrimitive::get_translation() const
	{
		return m_translation;
	}

	vec3d PlanePrimitive::get_scale() const
	{
		return m_scale;
	}

	double PlanePrimitive::get_outline_stroke() const
	{
		return m_outline_stroke;
	}

	void PlanePrimitive::set_outline_stroke(const double& value)
	{
		m_outline_stroke = value;
	}

	bool PlanePrimitive::get_allow_picking() const
	{
		return m_allow_picking;
	}

	void PlanePrimitive::set_allow_picking(bool value)
	{
		m_allow_picking = value;
	}

	void PlanePrimitive::set_show(bool value)
	{
		m_show = value;
	}

	bool PlanePrimitive::get_show() const
	{
		return m_show;
	}

	void PlanePrimitive::set_width(double value)
	{
		if (m_width != value)
		{
			m_render_resources.create_vertex_array = true;
			m_width = value;
		}
	}

	double PlanePrimitive::get_width() const
	{
		return m_width;
	}

	void PlanePrimitive::set_height(double value)
	{
		if (m_height != value)
		{
			m_render_resources.create_vertex_array = true;
			m_height = value;
		}
	}

	double PlanePrimitive::get_height() const
	{
		return m_height;
	}

	bool PlanePrimitive::get_depth_test_enabled()
	{
		return m_draw_command->get_render_state()->get_depth_test().enabled;
	}

	void PlanePrimitive::set_depth_test_enabled(bool value)
	{
		m_render_parameters.depth_test = DepthTest(value);
		m_draw_command->set_render_state(
				RenderState::from_cache(m_render_parameters));
	}

	bool PlanePrimitive::get_depth_write()
	{
		return m_draw_command->get_render_state()->get_depth_mask();
	}

	void PlanePrimitive::set_depth_write(bool value)
	{
		m_render_parameters.depth_mask = value;
		m_draw_command->set_render_state(
				RenderState::from_cache(m_render_parameters));
	}


}