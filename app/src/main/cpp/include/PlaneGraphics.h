#pragma once

#include "Bitmap.h"
#include "Vector.h"
#include "Billboard.h"
#include "IGraphics.h"
#include "PlanePrimitive.h"
#include "EllipsoidTangentPlane.h"

#include <string>

namespace argeo
{
	class PlaneGraphics
		: public IGraphics
	{
	public:
		PlaneGraphics(
			vec3d origin = vec3d::zero(),
			vec3d x_axis = vec3d::unit_x(),
			vec3d y_axis = vec3d::unit_y(),
			vec3d normal = vec3d::unit_z(),
			bool flag = false);

		PlaneGraphics(
			EllipsoidTangentPlane tangent_plane,
			bool flag);

		~PlaneGraphics();

		void set_show(const bool& value);
		bool get_show() const;

		void   set_width(double value);
		double get_width() const;

		bool get_depth_test_enabled();
		void set_depth_test_enabled(bool value);

		bool get_depth_write();
		void set_depth_write(bool value);

		bool get_allow_picking() const;
		void set_allow_picking(bool value);

		double get_outline_stroke() const;
		void set_outline_stroke(const double& value);

		vec3d get_origin() const;
		vec3d get_x_axis() const;
		vec3d get_y_axis() const;
		vec3d get_normal() const;

		bool get_flag() const;

	private:
		bool  m_flag;

		vec3d  m_origin;
		vec3d  m_x_axis;
		vec3d  m_y_axis;
		vec3d  m_normal;

		bool   m_show;
		double m_width;

		bool m_depth_write;
		bool m_depth_test;
		bool m_allow_picking;
		bool m_has_outline;

		double m_outline_stroke;
	};
}
