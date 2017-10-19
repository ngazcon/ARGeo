#pragma once

#include "Vector.h"

#include <cmath>

static const unsigned int SIXTY_FOUR_KILOBYTES = 65536;

// Constants used to calculate proximity of values.
static const double EPSILON0  = 0.1;
static const double EPSILON1  = 0.01;
static const double EPSILON2  = 0.001;
static const double EPSILON3  = 0.0001;
static const double EPSILON4  = 0.00001;
static const double EPSILON5  = 0.000001;
static const double EPSILON6  = 0.0000001;
static const double EPSILON7  = 0.00000001;
static const double EPSILON8  = 0.000000001;
static const double EPSILON9  = 0.0000000001;
static const double EPSILON10 = 0.00000000001;
static const double EPSILON11 = 0.000000000001;
static const double EPSILON12 = 0.0000000000001;
static const double EPSILON13 = 0.00000000000001;
static const double EPSILON14 = 0.000000000000001;
static const double EPSILON15 = 0.0000000000000001;

// Trigonometry constants.
static const double PI = 3.141592653;
static const double EULER = 2.71828182846;
static const double ONE_OVER_PI = 1.0 / PI;
static const double PI_OVER_TWO = PI * 0.5;
static const double PI_OVER_THREE = PI / 3.0;
static const double PI_OVER_FOUR = PI / 4.0;
static const double PI_OVER_SIX = PI / 6.0;
static const double THREE_PI_OVER_TWO = (3.0 * PI) * 0.5;
static const double TWO_PI = 2.0 * PI;
static const double ONE_OVER_TWO_PI = 1.0 / (2.0 * PI);
static const double DEGREES_PER_RADIAN = 180.0 / PI;
static const double RADIANS_PER_DEGREE = PI / 180.0;

namespace argeo
{
	/// 
	/// Helper class that defines mathematical methods that are not part of std c++11.
	/// 
	class ArgeoMath
	{
	public:

		static double sign(const double& n)
        {
            return ::copysign(1.0, n);
        }

		static double lerp(
			const double& p,
			const double& q,
			const double& time)
		{
			return ((1.0 - time) * p) + (time * q);
		};

		///Constraint a value to lie between two values.
		/// @param {Number} value The value to constrain.
		/// @param {Number} min The minimum value.
		/// @param {Number} max The maximum value.
		/// @returns {Number} The value clamped so that min <= value <= max.
		///
		static double clamp(
			const double& value,
			const double& min,
			const double& max)
		{
			return value < min ? min : value > max ? max : value;
		}

		///
		/// Computes <code>Math.acos(value)</acode>, but first clamps <code>value</code> to the range [-1.0, 1.0]
		/// so that the function will never return NaN.
		/// @param {Number} value The value for which to compute acos.
		/// @returns {Number} The acos of the value if the value is in the range [-1.0, 1.0], or the acos of -1.0 or 1.0,
		///         whichever is closer, if the value is outside the range.
		///
		static double acos_clamped(const double& value) 
		{
			return std::acos(clamp(value, -1.0, 1.0));
		};

		///
		/// The modulo operation that also works for negative dividends.
		/// @param {double} dividend The dividend.
		/// @param {double} divisor The divisor.
		/// @returns {double} The remainder.
		///
		static double mod(
			const double& dividend,
			const double& divisor)
		{
			return std::fmod((std::fmod(dividend, divisor) + divisor), divisor);
		}

		///
		/// Produces an angle in the range 0 <= angle <= 2Pi which is equivalent to the provided angle.
		/// @param {double} angle in radians
		/// @returns {double} The angle in the range [0, <code>CesiumMath.TWO_PI</code>].
		//
		static double zero_to_two_pi(const double& value)
		{
			double mod = ArgeoMath::mod(value, TWO_PI);

			if (std::abs(mod) < EPSILON14 && std::abs(value) > EPSILON14) 
			{
				return TWO_PI;
			}

			return mod;
		}

		///
		/// Produces an angle in the range -Pi <= angle <= Pi which is equivalent to the provided angle.
		/// @param {Number} angle in radians
		/// @returns {Number} The angle in the range [<code>-CesiumMath.PI</code>, <code>CesiumMath.PI</code>].
		///
		static double negative_pi_to_pi(const double& value)
		{
			return ArgeoMath::zero_to_two_pi(value + PI) - PI;
		}

		static vec4 float_to_vec4(const float& value)
		{
			double integral;
			vec4 enc = vec4(1.0, 255.0, 65025.0, 160581375.0) * value;
			enc  = vec4(
				::modf(enc.x, &integral), 
				::modf(enc.y, &integral), 
				::modf(enc.z, &integral), 
				::modf(enc.w, &integral));

			enc -= 
				vec4(enc.y, enc.z, enc.w, enc.w) * 
				vec4(1.0 / 255.0, 1.0 / 255.0, 1.0 / 255.0, 0.0);

			return enc;
		}

		static float float_from_vec4(const vec4& value)
		{
			return vec_dot(value, vec4(1.0, 1.0 / 255.0, 1.0 / 65025.0, 1.0 / 160581375.0));
		}

		static float float_from_vec4(const vec4ub& value)
		{
			vec4 valuef = vec4(
				static_cast<float>(value.x) / 255.0f,
				static_cast<float>(value.y) / 255.0f,
				static_cast<float>(value.z) / 255.0f,
				static_cast<float>(value.w) / 255.0f
			);

			return vec_dot(valuef, vec4(1.0, 1.0 / 255.0, 1.0 / 65025.0, 1.0 / 160581375.0));
		}

		static float normalize_float(
			const float& value, 
			const float& max,
			const float& min)
		{
			return (value - min) / (max - min);
		}

		static float denormalize_float(
			const float& value,
			const float& max,
			const float& min)
		{
			return value * (max - min) + min;
		}

		/// 
		/// Increment in one the number.
		/// @param {unsigned int} n The number to incremet.
		/// @param {unsigned int} mimimum The mimimun value when wrapping.
		/// @param {unsigned int} maximum The maximum value before wrapping.
		/// @return {unsigned int} If is lesser than maximum then returns n + 1. 
		/// Else, in the case that it surpass the maximum value, then wrap around and return mimimum.
		/// 
		static unsigned int increment_wrap(
			const unsigned int& n,
			const unsigned int& minimum,
			const unsigned int& maximum)
		{
			unsigned int wrap = n + 1; 
			if (wrap > maximum)
			{
				wrap = minimum;
			}
			return wrap;
		}

		/// 
		/// Converts from degrees to radians.
		/// @param {double} angle The angle to convert.
		/// @return {double} the angle in radians.
		/// 
		static double to_radians(const double& angle)
		{
			return angle * RADIANS_PER_DEGREE;
		}

		/// 
		/// Converts from radians to degrees.
		/// @param {double} angle The angle to convert.
		/// @return {double} the angle in degrees.
		/// 
		static double to_degrees(const double& angle)
		{
			return angle * DEGREES_PER_RADIAN;
		}

		/// 
		/// Compare if two values are equals by taking a epsilon interval of tolerance.
		/// @param {double} left Value to compare.
		/// @param {double} right Value to compare.
		/// @param {double} epsilon Interval of tolerance.
		/// @return {bool} true if |left - right| < epsilon.
		/// 
		static bool equals_epsilon(
			const double& left,
			const double& right,
			const double& epsilon)
		{
			return std::abs(left - right) < epsilon;
		}
	};
}
