#pragma once

#include "Vector.h"
#include "Matrix.h"

namespace argeo
{
	struct quaternion
	{
	public:
		union
		{
			struct
			{
				double x, y, z, w;
			};

			double data[4];
		};

		enum { quaternion_size = 4 };


		quaternion(
			double const & w = 0.0, 
			double const & x = 0.0,
			double const & y = 0.0, 
			double const & z = 0.0)
			: x(x)
			, y(y)
			, z(z)
			, w(w)
		{ }

		quaternion(quaternion const & q)
		{
			memcpy(data, q.data, 4 * sizeof(double));
		}

		///
		/// Create a quaternion from two normalized axis
		/// @param u A first normalized axis
		/// @param v A second normalized axis
		/// @see http://lolengine.net/blog/2013/09/18/beautiful-maths-quaternion-from-vectors
		///
		static quaternion from_two_vectors(
			vec3d const & u,
			vec3d const & v)
		{
			vec3d local_w = vec_cross(u, v);
			double dot    = vec_dot(u, v);

			return quaternion(
				1.0 + dot,
				local_w.x,
				local_w.y, 
				local_w.z);
		}

		/// 
		/// Computes a rotation from the given heading, pitch and roll angles. Heading is the rotation about the
		/// negative z axis. Pitch is the rotation about the negative y axis. Roll is the rotation about
		/// the positive x axis.
		/// @param {Number} yaw The yaw angle in radians.
		/// @param {Number} pitch The pitch angle in radians.
		/// @param {Number} roll The roll angle in radians.
		/// @param {Quaternion} [result] The object onto which to store the result.
		/// @returns {Quaternion} The modified result parameter or a new Quaternion instance if none was provided.
		/// 
		static quaternion from_yaw_pitch_roll(
			const double& yaw,
			const double& pitch,
			const double& roll)
		{
			quaternion roll_quaternion  = quaternion::from_axis_angle(vec3d::unit_x(),  roll);
			quaternion pitch_quaternion = quaternion::from_axis_angle(vec3d::unit_y(), -pitch);
			quaternion yaw_quaternion   = quaternion::from_axis_angle(vec3d::unit_z(), -yaw);
			
			quaternion result = pitch_quaternion.multiply(roll_quaternion);
			return yaw_quaternion.multiply(result);
		}


		static quaternion identity()
		{
			return quaternion(1, 0, 0, 0);
		}

		/// 
		/// Build a Quaternion from the given axis and angle
		/// @param {vec3d} axis The axis to rotate about.
		/// @param {double} angle The rotation angle in radians.
		/// @returns {quaternion} The quaternion.
		///
		static quaternion from_axis_angle(
			const vec3d& axis, 
			const double& angle)
		{
			double half_angle = angle * 0.5;
			double sin_angle  = std::sin(half_angle);
			vec3d vec_norm    = vec_normalize(axis);

			return quaternion(
				std::cos(half_angle),
				vec_norm.x * sin_angle,
				vec_norm.y * sin_angle,
				vec_norm.z * sin_angle);
		}


		//////////////////////////////////////
		// Operators

		double dot(quaternion const & q) const
		{
			return x * q.x + y * q.y + z * q.z + w * q.w;
		}

		double length() const
		{
			return ::sqrt(dot(*this));
		}

		quaternion normalize() const
		{
			double one_over_length = 1 / length();
			return quaternion(
				w * one_over_length,
				x * one_over_length,
				y * one_over_length,
				z * one_over_length);
		}


		quaternion multiply(const quaternion& q) const
		{
			double left_x = x;
			double left_y = y;
			double left_z = z;
			double left_w = w;

			double right_x = q.x;
			double right_y = q.y;
			double right_z = q.z;
			double right_w = q.w;

			double x = left_w * right_x + left_x * right_w + left_y * right_z - left_z * right_y;
			double y = left_w * right_y - left_x * right_z + left_y * right_w + left_z * right_x;
			double z = left_w * right_z + left_x * right_y - left_y * right_x + left_z * right_w;
			double w = left_w * right_w - left_x * right_x - left_y * right_y - left_z * right_z;

			return quaternion(w, x, y, z);
		}
	};


	inline quaternion quat_conjugate(quaternion const & q)
	{
		return quaternion(q.w, -q.x, -q.y, -q.z);
	}

	inline double quat_dot(quaternion const & x, quaternion const & y)
	{
		return x.x * y.x + x.y * y.y + x.z * y.z + x.w * y.w;
	}

	inline double quat_length(quaternion const & q)
	{
		return ::sqrt(quat_dot(q, q));
	}

	inline quaternion quat_normalize(const quaternion&  q)
	{
		double length = quat_length(q);

		double one_over_length = 1 / length;
		return quaternion(
			q.w * one_over_length,
			q.x * one_over_length,
			q.y * one_over_length,
			q.z * one_over_length);
	}

	inline mat3d quat_to_matrix(const quaternion& q)
	{
		double x2 = q.x * q.x;
        double xy = q.x * q.y;
        double xz = q.x * q.z;
        double xw = q.x * q.w;
        double y2 = q.y * q.y;
        double yz = q.y * q.z;
        double yw = q.y * q.w;
        double z2 = q.z * q.z;
        double zw = q.z * q.w;
        double w2 = q.w * q.w;

		double m00 = x2 - y2 - z2 + w2;
        double m01 = 2.0 * (xy - zw);
        double m02 = 2.0 * (xz + yw);

        double m10 = 2.0 * (xy + zw);
		double m11 = -x2 + y2 - z2 + w2;
        double m12 = 2.0 * (yz - xw);

        double m20 = 2.0 * (xz - yw);
        double m21 = 2.0 * (yz + xw);
		double m22 = -x2 - y2 + z2 + w2;

		return mat3d(
			m00, m10, m20,
			m01, m11, m21,
			m02, m12, m22);
	}

	inline quaternion quat_from_rotation_matrix(mat3d const & m)
	{
		double four_x_squared_minus1 = m(0, 0) - m(1, 1) - m(2, 2);
		double four_y_squared_minus1 = m(1, 1) - m(0, 0) - m(2, 2);
		double four_z_squared_minus1 = m(2, 2) - m(0, 0) - m(1, 1);
		double four_w_squared_minus1 = m(0, 0) + m(1, 1) + m(2, 2);

		int biggest_index = 0;
		double four_biggest_squared_minus1 = four_w_squared_minus1;
		if (four_x_squared_minus1 > four_biggest_squared_minus1)
		{
			four_biggest_squared_minus1 = four_x_squared_minus1;
			biggest_index = 1;
		}
		if (four_y_squared_minus1 > four_biggest_squared_minus1)
		{
			four_biggest_squared_minus1 = four_y_squared_minus1;
			biggest_index = 2;
		}
		if (four_z_squared_minus1 > four_biggest_squared_minus1)
		{
			four_biggest_squared_minus1 = four_z_squared_minus1;
			biggest_index = 3;
		}

		double biggest_val = sqrt(four_biggest_squared_minus1 + 1) * 0.5;
		double mult = 0.25f / biggest_val;

		quaternion result;
		switch (biggest_index)
		{
		case 0:
			result.w = biggest_val;
			result.x = (m(1, 2) - m(2, 1)) * mult;
			result.y = (m(2, 0) - m(0, 2)) * mult;
			result.z = (m(0, 1) - m(1, 0)) * mult;
			break;
		case 1:
			result.w = (m(1, 2) - m(2, 1)) * mult;
			result.x = biggest_val;
			result.y = (m(0, 1) + m(1, 0)) * mult;
			result.z = (m(2, 0) + m(0, 2)) * mult;
			break;
		case 2:
			result.w = (m(2, 0) - m(0, 2)) * mult;
			result.x = (m(0, 1) + m(1, 0)) * mult;
			result.y = biggest_val;
			result.z = (m(1, 2) + m(2, 1)) * mult;
			break;
		case 3:
			result.w = (m(0, 1) - m(1, 0)) * mult;
			result.x = (m(2, 0) + m(0, 2)) * mult;
			result.y = (m(1, 2) + m(2, 1)) * mult;
			result.z = biggest_val;
			break;
		}

		return result;
	}



	////////////////////////////////////////////////////////////////////////////////
	//						OVERLOADED OPERATORS
	////////////////////////////////////////////////////////////////////////////////

	// http://gamedev.stackexchange.com/questions/28395/rotating-vector3-by-a-quaternion
	inline vec3d operator*(quaternion const & q, vec3d const & v)
	{
		// Extract the vector part of the quaternion
		const vec3d u(q.x, q.y, q.z);

		// Extract the scalar part of the quaternion
		double s = q.w;

		vec3d t = vec_cross(u, v) * 2.0;
		return v + t * s + vec_cross(u, t);
	}

	inline vec3d operator*(vec3d const & v, quaternion const & q)
	{
		// Extract the vector part of the quaternion
		const vec3d u(q.x, q.y, q.z);

		// Extract the scalar part of the quaternion
		double s = q.w;

		vec3d t = vec_cross(u, v) * 2.0;
		return v + t * s + vec_cross(u, t);

	}

	inline vec3d operator*=(vec3d & v, quaternion const & q)
	{
		// Extract the vector part of the quaternion
		const vec3d u(q.x, q.y, q.z);

		// Extract the scalar part of the quaternion
		double s = q.w;

		vec3d t = vec_cross(u, v) * 2.0;
		v       = v + t * s + vec_cross(u, t);
		return v;
	}

	inline quaternion operator*(quaternion const & q1, quaternion const & q2)
	{
		return q1.multiply(q2);
	}

	inline quaternion operator*=(quaternion & q1, quaternion const & q2)
	{
		q1 = q1.multiply(q2);
		return q1;
	}
	
	inline bool operator==(quaternion const & q1, quaternion const & q2)
	{
		return q1.x == q2.x &&
			   q1.y == q2.y &&
			   q1.z == q2.z &&
			   q1.w == q2.w;
	}

	inline bool operator!=(quaternion const & q1, quaternion const & q2)
	{
		return q1.x != q2.x || 
			   q1.y != q2.y ||
			   q1.z != q2.z ||
			   q1.w != q2.w;
	}
}
