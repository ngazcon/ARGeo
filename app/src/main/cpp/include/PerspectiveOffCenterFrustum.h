#pragma once

#include "Matrix.h"
#include "Frustum.h"

namespace argeo
{
	class PerspectiveOffCenterFrustum : public Frustum
	{
	public:
		PerspectiveOffCenterFrustum(
			const double& left   = 0.0,
			const double& right  = 0.0,
			const double& bottom = 0.0,
			const double& top    = 0.0,
			const double& near   = 1.0,
			const double& far    = 500000000.0);

		~PerspectiveOffCenterFrustum();

		///
		/// @copydoc Frustum::get_culling_volume(const vec3d&, const vec3d&, const vec3d&)
		///
		std::unique_ptr<CullingVolume> get_culling_volume(
			const vec3d& position,
			const vec3d& direction,
			const vec3d& up);

		///
		/// @copydoc Frustum::get_pixel_dimension(const int&, const int&, const double&)
		///
		vec2d get_pixel_dimension(
			const int& surface_width,
			const int& surface_height,
			const double& distance);

		///
		/// @copydoc Frustum::get_projection()
		///
		mat4d get_projection();

		///
		/// @copydoc Frustum::get_near()
		///
		double get_near();

		///
		/// @copydoc Frustum::set_near(const double&)
		///
		void set_near(const double& value);

		///
		/// @copydoc Frustum::get_far()
		///
		double get_far();

		///
		/// @copydoc Frustum::set_far(const double&)
		///
		void set_far(const double& value);

		///
		/// Get the left plane.
		/// @returns {double} The left plane of this frustum.
		///
		double get_left();

		///
		/// Get the near plane.
		/// @returns {double} The near plane of this frustum.
		///
		void set_left(const double& value);

		///
		/// Get the right plane.
		/// @returns {double} The right plane of this frustum.
		///
		double get_right();

		///
		/// Set the right plane.
		/// @param {double} The right plane of this frustum.
		///
		void set_right(const double& value);

		///
		/// Get the top plane.
		/// @returns {double} The top plane of this frustum.
		///
		double get_top();

		///
		/// Set the top plane.
		/// @param {double} The top plane of this frustum.
		///
		void set_top(const double& value);

		///
		/// Get the bottom plane.
		/// @returns {double} The bottom plane of this frustum.
		///
		double get_bottom();

		///
		/// Set the bottom plane.
		/// @param {double} The bottom plane of this frustum.
		///
		void set_bottom(const double& value);

		///
		/// @copydoc Frustum::get_field_of_view_y()
		///
		double get_field_of_view_y();

        /// Set the field of view in the y axis.
        /// @param {double} The field of view in y of this frustum.
        ///
        void set_field_of_view_y(const double& value);

		///
		/// @copydoc Frustum::get_field_of_view()
		///
		double get_field_of_view();

		///
		/// Set the field of view in the x axis.
		/// @param {double} The field of view in x of this frustum.
		///
		void set_field_of_view(const double& value);

		///
		/// @copydoc Frustum::get_aspect_ratio()
		///
		double get_aspect_ratio();

		Frustum* clone(Frustum* result = nullptr);

	private:

		///
		/// Update the matrix with the new parameters.
		///
		void clean();

	private:
		mat4d  m_perspective_matrix;

		bool   m_dirty;
		double m_left;
		double m_right;
		double m_bottom;
		double m_top;
		double m_near;
		double m_far;
	};
}
