#pragma once

#include "Scene.h"
#include "Vector.h"
#include "Surface.h"

namespace argeo
{
	class SceneTransforms
	{
	public:

		/**
		* @private
		*/
		static vec2d window_to_drawing_buffer(
			Scene* scene,
			const vec2d& window_position)
		{
			Surface* surface = scene->get_context()->get_surface();
			double   x_scale = scene->get_context()->get_drawing_surface_width()  / surface->get_width();
			double   y_scale = scene->get_context()->get_drawing_surface_height() / surface->get_height();
			return vec2d(window_position.x * x_scale, window_position.y * y_scale);
		};


		static vec3d compute_actual_wgs84_position(
                const vec3d& position,
                FrameState* frame_state)
		{
			SceneMode mode = frame_state->get_mode();

			if (mode == SceneMode::Scene3D)
            {
				return position;
			}

			MapProjection* projection = frame_state->get_map_projection();
			Geodetic3D cartographic   = projection->get_ellipsoid().to_geodetic3D(position);

            vec3d projected_position = projection->project(cartographic).to_vec3d();
			if (mode == SceneMode::SceneCV)
            {
				return vec3d(
                        projected_position.z,
                        projected_position.x,
                        projected_position.y);
			}

			if (mode == SceneMode::Scene2D)
            {
                return vec3d(
                        0.0,
                        projected_position.x,
                        projected_position.y);
			}

			double morph_time = frame_state->get_morph_time();

			return vec3d(
					ArgeoMath::lerp(projected_position.z, position.x, morph_time),
					ArgeoMath::lerp(projected_position.x, position.y, morph_time),
					ArgeoMath::lerp(projected_position.y, position.z, morph_time));
		}


		/**
		* Transforms a position in the drawing buffer to cartesian coordinates.
		*/
		static vec3d drawing_buffer_to_cartesian_coordinates(
			Scene* scene, 
			const vec2d& surface_position,
			const float& depth)
		{
			Context* context		     = scene->get_context();
			UniformState* uniform_state  = context->get_uniform_state();
			Rectangle* viewport		     = scene->get_pass_state()->get_viewport();

			vec4d ndc(
				(surface_position.x - viewport->get_left())   / viewport->get_width()  * 2.0 - 1.0,
				(surface_position.y - viewport->get_bottom()) / viewport->get_height() * 2.0 - 1.0,
				(depth * 2.0) - 1.0,
				1.0);

			vec4d wc = uniform_state->get_inverse_view_projection_matrix() * ndc;

			// Reverse perspective divide
			double w = 1.0 / wc.w;
			wc       = wc * w;

			return to_vec3d(wc);
		};

	};

}