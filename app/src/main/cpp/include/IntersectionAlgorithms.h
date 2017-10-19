#pragma once

#include "Ray.h"
#include "Vector.h"
#include "Plane.h"
#include "Interval.h"
#include "BoundingSphere.h"

#include <tuple>
#include <climits>

namespace argeo
{
	class IntersectionAlgorithms
	{
	public:

		static vec3d ray_plane(
			const Ray& ray,
			const Plane& plane)
		{
			vec3d origin       = ray.origin;
			vec3d direction    = ray.direction;
			vec3d normal       = plane.get_normal();
			double denominator = vec_dot(normal, direction);

			if (fabs(denominator) < EPSILON15) 
			{
				// Ray is parallel to plane.  The ray may be in the polygon's plane.
				return vec3d(std::numeric_limits<double>::quiet_NaN());
			}

			double t = (-plane.get_distance() - vec_dot(normal, origin)) / denominator;

			if (t < 0) 
			{
				return vec3d(std::numeric_limits<double>::quiet_NaN());
			}

			return direction * t + origin;

		}


        static std::tuple<double, double> solve_quadratic(
                const double& a,
                const double& b,
                const double& c)
        {
            double det = b * b - 4.0 * a * c;

            if (det < 0.0)
            {
                return std::tuple<double,double >(
                        std::numeric_limits<double>::quiet_NaN(),
                        std::numeric_limits<double>::quiet_NaN());
            }
            else if (det > 0.0)
            {
                double denom = 1.0 / (2.0 * a);
                double disc  = ::sqrt(det);
                double root0 = (-b + disc) * denom;
                double root1 = (-b - disc) * denom;

                if (root0 < root1)
                {
                    return std::tuple<double, double >(
                            root0,
                            root1);
                }
                else
                {
                    return std::tuple<double, double >(
                            root1,
                            root0);
                }
            }

            double root = -b / (2.0 * a);

            if (root == 0.0)
            {
                return std::tuple<double, double >(
                        std::numeric_limits<double>::quiet_NaN(),
                        std::numeric_limits<double>::quiet_NaN());
            }

            return std::tuple<double, double >(
                    root,
                    root);
        }


        static Interval ray_sphere(
                const Ray& ray,
                BoundingSphere* sphere)
        {
            Interval result = _ray_sphere(ray, sphere);

            if (std::isnan(result.start) ||
                std::isnan(result.stop) ||
                result.stop < 0.0)
            {
                return Interval(
                        std::numeric_limits<double>::quiet_NaN(),
                        std::numeric_limits<double>::quiet_NaN());
            }

            result.start = ::fmax(result.start, 0.0);

            return result;
        }


        static vec3d ray_triangle(
                const Ray& ray,
                const vec3d& p0,
                const vec3d& p1,
                const vec3d& p2,
                const bool& cull_back_faces = false)
        {
            const double t = ray_triangle_parametric(ray, p0, p1, p2, cull_back_faces);

            if (std::isnan(t) || (cull_back_faces && t < 0.0))
            {
                return vec3d::undefined();
            }

            return ray.direction * t + ray.origin;
        }

    private:

        static double ray_triangle_parametric(
                const Ray& ray,
                const vec3d& p0,
                const vec3d& p1,
                const vec3d& p2,
                const bool& cull_back_faces = false)
        {
            const vec3d origin = ray.origin;
            const vec3d direction = ray.direction;

            const vec3d edge0 = p1 - p2;
            const vec3d edge1 = p2 - p0;

            const vec3d p = vec_cross(direction, edge1);
            const double det = vec_dot(edge0, p);

            vec3d tvec;
            vec3d q;

            double u;
            double v;
            double t;

            if (cull_back_faces)
            {
                if (det < EPSILON6)
                {
                    return std::numeric_limits<double>::quiet_NaN();
                }

                tvec = origin - p0;
                u = vec_dot(tvec, p);

                if (u < 0.0 || u > det)
                {
                    return std::numeric_limits<double>::quiet_NaN();
                }

                q = vec_cross(tvec, edge0);
                v = vec_dot(direction, q);

                if (v < 0.0 || u + v > det)
                {
                    return std::numeric_limits<double>::quiet_NaN();
                }

                t = vec_dot(edge1, q) / det;

            }
            else
            {
                if (::fabs(det) < EPSILON6)
                {
                    return std::numeric_limits<double>::quiet_NaN();
                }

                const double inv_det = 1.0 / det;

                tvec = origin - p0;
                u = vec_dot(tvec, p) * inv_det;

                if (u < 0.0 || u > 1.0)
                {
                    return std::numeric_limits<double>::quiet_NaN();
                }

                q = vec_cross(tvec, edge0);
                v = vec_dot(direction, q) * inv_det;

                if (v < 0.0 || u + v > 1.0)
                {
                    return std::numeric_limits<double>::quiet_NaN();
                }

                t = vec_dot(edge1, q) * inv_det;
            }

            return t;
        }

        static Interval _ray_sphere(
                const Ray& ray,
                BoundingSphere* sphere)
        {
            const vec3d direction = ray.direction;
            const vec3d origin    = ray.origin;

            const vec3d center = sphere->get_center();
            const double radiusSquared = sphere->get_radius() * sphere->get_radius();

            const vec3d diff = origin - center;

            double a = vec_dot(direction, direction);
            double b = 2.0 * vec_dot(direction, diff);
            double c = vec_length_squared(diff) - radiusSquared;

            std::tuple<double, double> roots = solve_quadratic(a, b, c);

            if (std::isnan(std::get<0>(roots))||
                std::isnan(std::get<1>(roots)))
            {
                return Interval(
                        std::numeric_limits<double>::quiet_NaN(),
                        std::numeric_limits<double>::quiet_NaN());
            }


            Interval result;
            result.start = std::get<0>(roots);
            result.stop  = std::get<1>(roots);

            return result;
        }
	};
}
