#pragma once

#include "Geodetic2D.h"
#include "Matrix.h"
#include "Vector.h"
#include "ArgeoMath.h"
#include "Ellipsoid.h"
#include "MapProjection.h"

#include <cmath>

namespace argeo
{
	class Transforms
	{
		///
		/// A helper class to handle the transformation between differents frames of references.
		///

	public:

        ///
		/// Coverts a ECEF (Earth Centered - Earth Fixed) Frame position to a ENU (Earth North Up) Frame position.
		/// @param {Geodetic3D} geodetic The geodetic position.
		/// @param {vec3d} ecef The position in ECEF Frame.
		/// @param {vec3d} [reference_ecef = vec3d::zero()] The reference ECEF Frame Position.
		/// @result {vec3d} The ENU Frame position.
		///
		static vec3d ecef_to_enu(
			const Geodetic3D& geodetic,
			const vec3d& ecef,
			const vec3d& reference_ecef = vec3d::zero())
		{
			double lat = geodetic.get_latitude();
			double lon = geodetic.get_longitude();

			double c_lat = std::cos(lat);
			double s_lat = std::sin(lat);
			double c_lon = std::cos(lon);
			double s_lon = std::sin(lon);

			double dx = ecef.x - reference_ecef.x;
			double dy = ecef.y - reference_ecef.y;
			double dz = ecef.z - reference_ecef.z;

			vec3d enu(
				-(s_lon)* dx + (c_lon)* dy,
				-(s_lat * c_lon) * dx - (s_lat * s_lon) * dy + (c_lat)* dz,
				 (c_lat * c_lon) * dx + (c_lat * s_lon) * dy + (s_lat)* dz);

			return enu;
		}

		///
		/// Coverts a ENU (Earth North Up) Frame position to a ECEF (Earth Centered - Earth Fixed) Frame position.
		/// @param {Geodetic3D} geodetic The geodetic position.
		/// @param {vec3d} enu The position in ENU Frame.
		/// @result {vec3d} The ECEF Frame position.
		///
		static vec3d enu_to_ecef(
			const Geodetic3D& geodetic, 
			const vec3d& enu)
		{
			double phi	  = geodetic.get_latitude();
			double lambda = geodetic.get_longitude();

			double c_phi = std::cos(phi);
			double s_phi = std::sin(phi);
			double c_lambda = std::cos(lambda);
			double s_lambda = std::sin(lambda);

			vec3d ecef(
				-(s_lambda)*  enu.x - (s_phi * c_lambda) * enu.y + (c_phi * c_lambda) * enu.z,
				 (c_lambda)*  enu.x - (s_phi * s_lambda) * enu.y + (c_phi * s_lambda) * enu.z,
				 (c_phi)   *  enu.y + (s_phi)* enu.z);

			return ecef;
		}

		///
		/// Get the transformation that convertex from ENU (Earth North Up) Frame position to a ECEF (Earth Centered - Earth Fixed) Frame position.
		/// @result {mat4d} The transformation matrix.
		///
		static mat4d enu_to_fixed_frame(
			const vec3d& origin,
			const Ellipsoid& ellipsoid)
		{
			if (ArgeoMath::equals_epsilon(origin.x, 0.0, EPSILON14) &&
				ArgeoMath::equals_epsilon(origin.y, 0.0, EPSILON14))
			{
				double sign = ::copysign(1.0, origin.z);
				return mat4d(
					0.0,       1.0,		0.0,      0.0,
					-sign,     0.0,		0.0,      0.0,
					0.0,       0.0,		sign,     0.0,
					origin.x, origin.y, origin.z, 1.0);
			}

			vec3d tangent(
				-origin.y,
				 origin.x,
				 0.0);
			tangent = vec_normalize(tangent);

			vec3d normal    = ellipsoid.geodetic_surface_normal(origin).to_vec3d();
			vec3d bitangent = vec_cross(normal, tangent);

			return mat4d(
				tangent.x, tangent.y, tangent.z, 0.0,
				bitangent.x, bitangent.y, bitangent.z, 0.0,
				normal.x, normal.y, normal.z, 0.0,
				origin.x, origin.y, origin.z, 1.0);

		}


		static vec2d point_to_gl_window_coordinates(
				const mat4d& model_view_projection_matrix,
				const mat4d& viewport_transformation,
				const vec3d& point)
		{
			vec4d tmp = model_view_projection_matrix * vec4d(point, 1.0);
			tmp		 *= (1.0 / tmp.w);
			tmp  	  = viewport_transformation * tmp;
			return vec2d(tmp.x, tmp.y);
		};

        static mat4d basis_to_2D(
            MapProjection* projection,
            const mat4d& matrix)
        {
            static const mat4d swizzle_matrix = mat4d(
                    0.0, 1.0, 0.0, 0.0,
                    0.0, 0.0, 1.0, 0.0,
                    1.0, 0.0, 0.0, 0.0,
                    0.0, 0.0, 0.0, 1.0);

            vec3d rtc_center = mat_get_translation(matrix);
            Ellipsoid ellipsoid = projection->get_ellipsoid();

            // Get the 2D Center
            Geodetic3D geodetic = ellipsoid.to_geodetic3D(rtc_center);
            vec3d projected_position = projection->project(geodetic).to_vec3d();
            projected_position = vec3d(
                    projected_position.z,
                    projected_position.x,
                    projected_position.y);

            // Assuming the instance are positioned in WGS84, invert the WGS84 transform to get the local transform and then convert to 2D
            mat4d from_enu = Transforms::enu_to_fixed_frame(rtc_center, ellipsoid);
            mat4d to_enu   = mat_inverse_transformation(from_enu);
            mat3d rotation = mat_get_rotation(matrix);
            mat4d local    = to_enu * rotation;

            // Swap x, y, z for 2D
            mat4d result = swizzle_matrix * local;
            result = mat_set_translation(result, projected_position);

            return result;
        }

        static mat4d wgs84_to_2D_model_matrix(
                MapProjection* projection,
                const vec3d& center)
        {
            static const mat4d swizzle_matrix = mat4d(
                    0.0, 1.0, 0.0, 0.0,
                    0.0, 0.0, 1.0, 0.0,
                    1.0, 0.0, 0.0, 0.0,
                    0.0, 0.0, 0.0, 1.0);

            Ellipsoid ellipsoid = projection->get_ellipsoid();

            mat4d from_enu = Transforms::enu_to_fixed_frame(center, ellipsoid);
            mat4d to_enu   = mat_inverse_transformation(from_enu);

            Geodetic3D geodetic = ellipsoid.to_geodetic3D(center);
            vec3d projected_position = projection->project(geodetic).to_vec3d();
            projected_position = vec3d(
                    projected_position.z,
                    projected_position.x,
                    projected_position.y);

            mat4d translation = mat_translate(projected_position);
            return translation * (swizzle_matrix * to_enu);
        }

	};

}
