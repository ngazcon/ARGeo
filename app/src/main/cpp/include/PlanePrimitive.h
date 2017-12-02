#pragma once


#include "Uniform.h"
#include "DrawCommand.h"
#include "VertexArray.h"
#include "ShaderProgram.h"
#include "EllipsoidTangentPlane.h"
#include "Quaternion.h"
#include "HeightClamping.h"
#include "OrientationClamping.h"

#include "IPrimitive.h"
#include "PickId.h"

#include <memory>

namespace argeo
{
    class Scene;
	class PlanePrimitive : 
		public IPrimitive
	{
	public:
		PlanePrimitive(
            Scene* scene,
			vec3d origin = vec3d::zero(),
			vec3d x_axis = vec3d::unit_x(),
			vec3d y_axis = vec3d::unit_y(),
			vec3d normal = vec3d::unit_z(),
			const bool& flag = false);

		PlanePrimitive(
            Scene* scene,
			EllipsoidTangentPlane tangent_plane,
			const bool& flag = false);

		~PlanePrimitive();

		void update(FrameState* frame_state);

		mat4d get_model_matrix();
		void update_model_matrix();
		
		void create_render_states();
		void create_vertex_array(Context* context);

		void set_show(bool value);
		bool get_show() const;

		void   set_width(double value);
		double get_width() const;

        void   set_height(double value);
        double get_height() const;

		bool get_depth_test_enabled();
		void set_depth_test_enabled(bool value);

		bool get_depth_write();
		void set_depth_write(bool value);
		
		bool get_allow_picking() const;
		void set_allow_picking(bool value);

		double get_outline_stroke() const;
		void set_outline_stroke(const double& value);

		void set_translation(vec3d position);

		void translate(vec3d translation);

		void set_euler_angles(
			double pitch,
			double yaw,
			double roll);

		void set_translation(
			double x,
			double y,
			double z);

		void translate(
			double x,
			double y,
			double z);

		void set_rotation(quaternion rotation);
		void rotate(quaternion rotation);

		void set_scale(
			const double& x,
			const double& y,
			const double& z);

		quaternion get_rotation() const;
		vec3d      get_translation() const;
		vec3d      get_scale() const;

	private:
        void update_clamping();

	private:
		
		Color  m_color;

		vec3d  m_origin;
        vec3d  m_clamped_origin;
		vec3d  m_x_axis;
		vec3d  m_y_axis;
		vec3d  m_normal;

        Scene* m_scene;

		bool   m_show;

		double m_height;
		double m_width;

		bool m_allow_picking;
		bool m_has_outline;
        bool m_height_changed;

		vec3d m_translation;
		vec3d m_scale;
        quaternion m_rotation;
		quaternion m_orientation;

        SceneMode m_mode;
        HeightClamping m_height_clamping;
        OrientationClamping m_orientation_clamping;

        std::function<void()> m_remove_update_height_callback;
        std::function<void(vec3d&)> m_update_height_function;

		bool  m_model_matrix_changed;
		mat4d m_model_matrix;
		mat4d m_clamped_model_matrix;
        mat4d m_computed_model_matrix;

		double m_outline_stroke;

		RenderStateParameters m_render_parameters;

		std::unique_ptr<DrawCommand> m_draw_command;
		std::unique_ptr<DrawCommand> m_pick_command;
		std::unique_ptr<DrawCommand> m_outline_command;

        std::unique_ptr<BoundingSphere> m_base_bounding_sphere;
		static const ShaderVertexAttributeLocations attribute_locations;

		struct RenderResources
		{
			RenderResources()
				: create_vertex_array(true)
				, create_render_states(true)
				, render_state(nullptr)
				, outline_render_state(nullptr)
				, pick_id(nullptr)
				, vertex_array(nullptr)
				, shader_program(nullptr)
				, pick_shader_program(nullptr)
				, outline_shader_program(nullptr)
				, uniform_map(nullptr)
				, pick_uniform_map(nullptr)
				, outline_uniform_map(nullptr)
			{ }

			bool create_render_states;
			RenderState*				   render_state;
			RenderState*				   outline_render_state;

			PickId*						   pick_id;
			
			bool create_vertex_array;
			std::unique_ptr<VertexArray>   vertex_array;
			
			ShaderProgram* shader_program;			
			ShaderProgram* pick_shader_program;
			ShaderProgram* outline_shader_program;

			std::unique_ptr<VertexBuffer>  positions_and_normals_buffer;

			std::unique_ptr<UniformMap>    uniform_map;
			std::unique_ptr<UniformMap>    pick_uniform_map;
			std::unique_ptr<UniformMap>    outline_uniform_map;
		} m_render_resources;
	};
}
