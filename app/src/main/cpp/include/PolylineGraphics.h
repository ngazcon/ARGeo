#pragma once

#include "Bitmap.h"
#include "Vector.h"
#include "IGraphics.h"

#include <vector>
#include <string>

namespace argeo
{
	class PolylineGraphics
		: public IGraphics
	{
	public:
		PolylineGraphics(
			const std::vector<vec3d>& points,
			const bool& loop    = false,
			const double& width = 1.0);

		~PolylineGraphics();

		
		std::vector<vec3d> get_points() const;
		void set_points(const std::vector<vec3d>& value);
		void add_point(const vec3d& value);

		void set_loop(const bool& value);
		bool get_loop() const;

		void set_show(const bool& value);
		bool get_show() const;

		void   set_width(const double& value);
		double get_width() const;

	private:
		bool m_show;
		bool m_loop;
		double m_width;

		std::vector<vec3d> m_points;
	};
}
