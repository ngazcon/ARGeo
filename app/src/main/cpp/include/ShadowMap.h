#pragma once

#include "FrameBuffer.h"
#include "Matrix.h"
#include "BoundingSphere.h"
#include "Frustum.h"
#include "Camera.h"
#include "DrawCommand.h"
#include "ClearCommand.h"

namespace argeo
{
	struct ShadowMapCamera
	{
		ShadowMapCamera()
			: position_wc(vec3d::zero())
			, direction_wc(vec3d::unit_z())
			, up_wc(vec3d::unit_y())
			, right_wc(vec3d::unit_x())
			, frustum(nullptr)
		{ }

		ShadowMapCamera(const ShadowMapCamera& other)
			: position_wc(other.position_wc)
			, direction_wc(other.direction_wc)
			, up_wc(other.up_wc)
			, right_wc(other.right_wc)
			, position_geodetic(other.position_geodetic)
			, view_matrix(other.view_matrix)
			, inverse_view_matrix(other.inverse_view_matrix)
			, frustum(other.frustum != nullptr ? other.frustum->clone() : nullptr)
		{ }

		ShadowMapCamera& operator=(const ShadowMapCamera& other)
		{
			position_wc = other.position_wc;
			direction_wc = other.direction_wc;
			up_wc = other.up_wc;
			right_wc = other.right_wc;
			position_geodetic = other.position_geodetic;
			view_matrix = other.view_matrix;
			inverse_view_matrix = other.inverse_view_matrix;
			frustum = std::unique_ptr<Frustum>(other.frustum->clone());
			return *this;
		}

		mat4d view_matrix;
		mat4d inverse_view_matrix;

		std::unique_ptr<Frustum> frustum;
		  
		Geodetic3D  position_geodetic;
		vec3d position_wc;
		vec3d direction_wc;
		vec3d up_wc;
		vec3d right_wc;

		mat4d get_view_projection_matrix()
		{
			static const mat4d scale_bias_matrix(
				0.5, 0.0, 0.0, 0.0, 
				0.0, 0.5, 0.0, 0.0,
				0.0, 0.0, 0.5, 0.0, 
				0.5, 0.5, 0.5, 1.0);

			mat4d view_projection_matrix = frustum->get_projection() * view_matrix;
			view_projection_matrix       = scale_bias_matrix * view_projection_matrix;
			return view_projection_matrix;
		}
	};

	struct ShadowPass
	{
		ShadowPass(Context* context)
			: pass_state(context)
			, frame_buffer(nullptr)
			, texture_offsets(0.0f)
		{ }

		ShadowMapCamera camera;
		PassState		pass_state;
		FrameBuffer*    frame_buffer;
		mat4d		    texture_offsets;
		std::unique_ptr<CullingVolume>  culling_volume;
		CommandQueue command_queue;
	};

	struct Bias
	{
		Bias(
			const bool&  polygon_offset = false,
			const float& polygon_offset_factor = 0.0,
			const float& polygon_offset_units = 0.0,
			const bool&  normal_offset = false,
			const float& normal_offset_scale = 0.0,
			const bool&  normal_shading = false,
			const float& normal_shading_smooth = 0.0,
			const float& depth_bias = 0.0)
			: polygon_offset(polygon_offset)
			, polygon_offset_factor(polygon_offset_factor)
			, polygon_offset_units(polygon_offset_units)
			, normal_offset(normal_offset)
			, normal_offset_scale(normal_offset_scale)
			, normal_shading(normal_shading)
			, normal_shading_smooth(normal_shading_smooth)
			, depth_bias(depth_bias)
		{ }

		bool  polygon_offset;
		float polygon_offset_factor;
		float polygon_offset_units;
		
		bool  normal_offset;
		float normal_offset_scale;
		bool  normal_shading;
		float normal_shading_smooth;
		float depth_bias;
	};

	class ShadowMap
	{
	public:
		ShadowMap(
			Context* context,
			Camera* light_camera,
			const bool& enabled = true,
			const bool& from_light_source = true,
			const bool& is_point_light = true,
			const double& point_light_radius = 100.0,
			const bool& cascades_enabled = true,
			const unsigned int& number_of_cascades = 4,
			const double& maximum_distance = 5000.0,
			const std::size_t& size = 2048,
			const bool& soft_shadows = false,
			const double& darkness = 0.3);

		~ShadowMap();

	public:
		bool get_is_point_light() const;
		bool get_is_spot_light() const;
		unsigned int get_number_of_cascades() const;
		bool get_soft_shadows() const;
		bool get_uses_depth_texture() const;
		bool get_polygon_offset_supported() const;
		bool get_is_out_of_view() const;
		Bias get_terrain_bias() const;
		Bias get_primitive_bias() const;
		Bias get_point_bias() const;
		std::vector<ShadowPass>& get_passes();
		CullingVolume* get_culling_volume();

		static void create_derived_commands(
			std::vector<ShadowMap*> shadow_maps,
			std::vector<ShadowMap*> light_shadow_maps,
			DrawCommand* command,
			const bool& shadows_dirty,
			Context* context,
			DerivedCommands::ShadowCommand& derived_commands);

		
		
		void update_pass(
			Context* context,
			const unsigned int& shadow_pass);

	private: 
		void destroy_frame_buffer();
		
		TextureSampler* create_sampler();
		
		void create_frame_buffer(Context* context);
		void create_frame_buffer_color(Context* context);
		void create_frame_buffer_depth(Context* context);
		void create_frame_buffer_cube(Context* context);
		void check_frame_buffer(Context* context);
		void update_frame_buffer(Context* context);
		void clear_frame_buffer(
			Context* context,
			const unsigned int& shadow_pass = 0);

		void resize(const std::size_t& size);

		void create_render_states();
		RenderState* create_render_state(
			const bool& color_mask,
			const Bias& bias);

		UniformMap* combine_uniforms(
			UniformMap* uniform_map,
			const bool& is_terrain);

		void compute_cascades(FrameState* frame_state);
		void fit_shadow_map_to_scene(FrameState* frame_state);
		void compute_omnidirectional(FrameState* frame_state);
		void check_visibility(FrameState* frame_state);
		void update_cameras(FrameState* frame_state);		
		void update(FrameState* frame_state);


		void create_cast_derived_command(
			const bool& shadows_dirty,
			DrawCommand* command,
			Context* context,
			const unsigned int& old_shader_id,
			DrawCommand* cast_command);

	private:
		static const std::vector<vec3d> g_rights;
		static const std::vector<vec3d> g_ups;
		static const std::vector<vec3d> g_directions;
		static const std::vector<vec4d> g_frustum_corners_ndc;

		bool m_enabled;
		bool m_soft_shadows;
		bool m_dirty;

		bool m_needs_update;
		bool m_out_of_view;
		bool m_out_of_view_previous;

		double m_darkness;
		double m_maximum_distance;

		// Framebuffer resources
		RenderBuffer* m_depth_render_buffer_attachment;
		Texture*    m_depth_texture_attachment;
		Texture*    m_color_attachment;

		// Uniforms
		mat4d  m_shadow_map_matrix;
		vec3d  m_light_direction_ec;
		vec4d  m_light_position_ec;
		double m_distance;

		// The external camera representing the light source
		Camera* m_light_camera;

		// Clone of camera, with clamped near and far planes
		Camera* m_scene_camera;

		ShadowMapCamera m_shadow_map_camera;
		std::unique_ptr<BoundingSphere> m_bounding_sphere;
		std::unique_ptr<CullingVolume>  m_shadow_map_culling_volume;
		Texture* m_shadow_map_texture;

		bool  m_from_light_source;
		bool  m_is_point_light;
		double m_point_light_radius;

		bool m_is_spot_light;
			
		bool m_cascades_enabled;
		bool m_fit_near_far;
		unsigned int m_number_of_cascades;
		std::vector<double> m_maximum_cascade_distance;
		std::vector<vec4d>  m_cascade_splits;
		std::vector<mat4d>  m_cascade_matrices;
		vec4d  m_cascade_distances;

		std::size_t m_texture_size;
		bool m_uses_depth_texture;

		std::vector<ShadowPass> m_passes;

		RenderState* m_primitive_render_state;
		RenderState* m_terrain_render_state;
		RenderState* m_point_render_state;

		Bias m_primitive_bias;
		Bias m_terrain_bias;
		Bias m_point_bias;

		ClearCommand m_clear_command;
		PassState m_clear_pass_state;

		std::size_t m_size;
	};
}
