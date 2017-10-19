#pragma once

#include "Vector.h"

namespace argeo
{
	class RectangleD
	{
	public:

		RectangleD(
			const double& x      = 0.0,
			const double& y      = 0.0,
			const double& width  = 0.0,
			const double& height = 0.0)
			: m_lower_left(x, y)
			, m_upper_right(x + width, y + height)
		{ }

		RectangleD(
			const vec2d& lower_left,
			const vec2d& upper_right)
			: m_lower_left(lower_left)
			, m_upper_right(upper_right)
		{ }

		double get_top() const
		{
			return m_upper_right.y;
		}

		void set_bottom(const double& value)
		{
			m_lower_left.y = value;
		}

		double get_bottom() const
		{
			return m_lower_left.y;
		}

		void set_left(const double& value)
		{
			m_lower_left.x = value;
		}

		double get_left() const
		{
			return m_lower_left.x;
		}

		double get_right() const
		{
			return m_upper_right.x;
		}

		void set_width(const double& value)
		{
			m_upper_right.x = m_lower_left.x + value;
		}

		double get_width() const
		{
			return m_upper_right.x - m_lower_left.x;
		}

		void set_height(const double& value)
		{
			m_upper_right.y = m_lower_left.y + value;
		}

		double get_height() const
		{
			return m_upper_right.y - m_lower_left.y;
		}
				
		vec2d get_center() const
		{
			return (m_lower_left + m_upper_right) * 0.5;
		}

		vec2d get_lower_left() const
		{ 
			return m_lower_left; 
		}

		vec2d get_upper_right() const
		{
			return m_upper_right;
		}

		bool equals(RectangleD other) const
		{
			return
				(m_lower_left.x  == other.m_lower_left.x) &&
				(m_lower_left.y  == other.m_lower_left.y) &&
				(m_upper_right.x == other.m_upper_right.x) &&
				(m_upper_right.y == other.m_upper_right.y);
		}


	private:
		vec2d m_lower_left;
		vec2d m_upper_right;

	};

	inline bool operator ==(const RectangleD& left, const RectangleD& right)
	{
		return left.equals(right);
	}

	inline bool operator !=(const RectangleD& left, const RectangleD& right)
	{
		return !left.equals(right);
	}
}
