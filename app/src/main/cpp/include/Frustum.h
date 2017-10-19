#pragma once

#include "Matrix.h"
#include "CullingVolume.h"

#include <memory>

namespace argeo
{
	class CullingVolume;

	class Frustum
	{
		///
		/// Represent a frustum used by a camera.
		///

	public:

        virtual ~Frustum() {}

		///
		/// Returns the pixel's width and height in meters.
		/// @param {double} surface_width The width of the drawing buffer.
		/// @param {double} surface_height The height of the drawing buffer.
		/// @param {double} distance The distance to the near plane in meters.
		/// @returns {vec2d} The result with the pixel's width and height in the x and y properties, respectively.
		/// 
		/// @exception {std::out_of_range} surface_width must be greater than zero.
		/// @exception {std::out_of_range} surface_height must be greater than zero.
		///
		virtual std::unique_ptr<CullingVolume> get_culling_volume(
			const vec3d& position,
			const vec3d& direction,
			const vec3d& up) = 0;

		///
		/// Returns the pixel's width and height in meters.
		/// @param {double} surface_width The width of the drawing buffer.
		/// @param {double} surface_height The height of the drawing buffer.
		/// @param {double} distance The distance to the near plane in meters.
		/// @returns {vec2d} The result with the pixel's width and height in the x and y properties, respectively.
		/// 
		/// @exception {std::out_of_range} surface_width must be greater than zero.
		/// @exception {std::out_of_range} surface_height must be greater than zero.
		///
		virtual vec2d get_pixel_dimension(
			const int& surface_width,
			const int& surface_height,
			const double& distance) = 0;


		///
		/// Get the projection matrix.
		/// @returns {mat4d} The projection matrix of this frustum.
		///
		virtual mat4d get_projection() = 0;
		
		///
		/// Get the near plane.
		/// @returns {double} The near plane of this frustum.
		///
		virtual double get_near() = 0;

		///
		/// Set the near plane.
		/// @param {double} The near plane of this frustum.
		///
		virtual void set_near(const double& value) = 0;

		///
		/// Get the far plane.
		/// @returns {double} The far plane of this frustum.
		///
		virtual double get_far() = 0;

        virtual double get_right() = 0;
        virtual void set_right(const double& value) = 0;

        virtual double get_left() = 0;
        virtual void set_left(const double& value) = 0;

        virtual double get_bottom() = 0;
        virtual void set_bottom(const double& value) = 0;

        virtual double get_top() = 0;
        virtual void set_top(const double& value) = 0;
		
		///
		/// Get the field of view in the y plane plane.
		/// @returns {double} The field of view in y of this frustum.
		///
		virtual double get_field_of_view_y() = 0;
        virtual void set_field_of_view_y(const double& value) = 0;


		virtual double get_field_of_view() = 0;


        virtual void set_field_of_view(const double& value) = 0;

		///
		/// Get the aspect ratio.
		/// @returns {double} The aspect ratio of this frustum.
		///
		virtual double get_aspect_ratio() = 0;

		///
		/// Set the far plane.
		/// @param {double} value The far plane of this frustum.
		///
		virtual void set_far(const double& value) = 0;

		virtual Frustum* clone(Frustum* result = nullptr) = 0;
	};
}
