#pragma once

#include <string.h>
#include <cmath>
#include <limits>

#include <sstream>

//////////////////////////////////////////////////////////////////
//						STRUCTS
//////////////////////////////////////////////////////////////////

struct vec2i
{
	union
	{
		struct
		{
			int x, y;
		};
		
		int data[2];
	};

	static vec2i zero()   { return vec2i(0, 0); }
	static vec2i unit_x() { return vec2i(1, 0); }
	static vec2i unit_y() { return vec2i(0, 1); }

	enum { vector_size = 2 };

	vec2i(const int& s = 0) 
		: x(s)
		, y(s)
	{ }

	vec2i(
		const int& x, 
		const int& y) 
		: x(x)
		, y(y)
	{ }

	vec2i(const int* vec) 
	{ 
		memcpy(data, vec, 2 * sizeof(int));
	}
};

struct vec2
{
	union
	{
		struct
		{
			float x, y;
		};
		float data[2];
	};

	static vec2 zero()   { return vec2(0, 0); }
	static vec2 unit_x() { return vec2(1, 0); }
	static vec2 unit_y() { return vec2(0, 1); }

	enum { vector_size = 2 };

	vec2(const float& s = 0)
		: x(s)
		, y(s)
	{ }

	vec2(
		const float& x,
		const float& y)
		: x(x)
		, y(y)
	{ }

	vec2(const vec2i& v)
		: x(v.x)
		, y(v.y)
	{ }

	vec2(const float* vec)
	{
		memcpy(data, vec, 2 * sizeof(float));
	}
};

struct vec2d
{
	union
	{
		struct
		{
			float x, y;
		};
		float data[2];
	};

	static vec2d zero()   { return vec2d(0, 0); }
	static vec2d unit_x() { return vec2d(1, 0); }
	static vec2d unit_y() { return vec2d(0, 1); }

	enum { vector_size = 2 };

	vec2d(const double& s = 0)
		: x(s)
		, y(s)
	{ }

	vec2d(
		const double& x,
		const double& y)
		: x(x)
		, y(y)
	{ }

	vec2d(const double* vec)
	{
		memcpy(data, vec, 2 * sizeof(double));
	}
};

struct vec3
{
	union
	{
		struct
		{
			float x, y, z;
		};

		float data[3];
	};
	enum { vector_size = 3 };

	static vec3 zero()   { return vec3(0, 0, 0); }
	static vec3 unit_x() { return vec3(1, 0, 0); }
	static vec3 unit_y() { return vec3(0, 1, 0); }
	static vec3 unit_z() { return vec3(0, 0, 1); }

	vec3(const float& s = 0) 
		: x(s)
		, y(s)
		, z(s)
	{ }

	vec3(
		const float& x,
		const float& y,
		const float& z)
		: x(x)
		, y(y)
		, z(z)
	{ }

	vec3(const float* vec)
	{ 
		memcpy(data, vec, 3 * sizeof(float)); 
	}

	vec3(
		const vec2& xy,
		const float& z)
		: x(xy.x)
		, y(xy.y)
		, z(z)
	{ }

	vec3(
		const float& x, 
		const vec2& yz)
		: x(x)
		, y(yz.x)
		, z(yz.y)
	{ }
};

struct vec3d
{
	union
	{
		struct
		{
			double x, y, z;
		};

		double data[3];
	};
	enum { vector_size = 3 };

	static vec3d zero()   { return vec3d(0, 0, 0); }
	static vec3d unit_x() { return vec3d(1, 0, 0); }
	static vec3d unit_y() { return vec3d(0, 1, 0); }
	static vec3d unit_z() { return vec3d(0, 0, 1); }

	vec3d(const double& s = 0)
		: x(s)
		, y(s)
		, z(s)
	{ }

	vec3d(
		const double& x,
		const double& y,
		const double& z)
		: x(x)
		, y(y)
		, z(z)
	{ }

	vec3d(const double* vec)
	{
		memcpy(data, vec, 3 * sizeof(double));
	}

	vec3d(
		const vec2d& xy,
		const double& z)
		: x(xy.x)
		, y(xy.y)
		, z(z)
	{ }

	vec3d(
		const double& x,
		const vec2d& yz)
		: x(x)
		, y(yz.x)
		, z(yz.y)
	{ }

    static vec3d undefined()
    {
        return vec3d(std::numeric_limits<double>::quiet_NaN());
    }

};

struct vec4d
{
	union
	{
		struct
		{
			double x, y, z, w;
		};

		double data[4];
	};

	enum { vector_size = 4 };

	vec4d(const double& s = 0) 
		: x(s)
		, y(s)
		, z(s)
		, w(s)
	{ }

	vec4d(
		const double& x, 
		const double& y,
		const double& z, 
		const double& w)
		: x(x)
		, y(y)
		, z(z)
		, w(w)
	{ }

	vec4d(const double *vec)
	{ 
		memcpy(data, vec, 4 * sizeof(double)); 
	}

	vec4d(
		const vec2d& xy, 
		const vec3d& zw)
		: x(xy.x)
		, y(xy.y)
		, z(zw.x)
		, w(zw.y)
	{ }

	vec4d(
		const vec3d& xyz, 
		const double& w)
		: x(xyz.x)
		, y(xyz.y)
		, z(xyz.z)
		, w(w)
	{ }

	vec4d(
		const double& x,
		const vec3d& yzw)
		: x(x)
		, y(yzw.x)
		, z(yzw.y)
		, w(yzw.z)
	{ }


	static vec4d zero()   { return vec4d(0.0); }
	static vec4d unit_x() { return vec4d(1.0, 0.0, 0.0, 0.0); }
    static vec4d unit_y() { return vec4d(0.0, 1.0, 0.0, 0.0); }
    static vec4d unit_z() { return vec4d(0.0, 0.0, 1.0, 0.0); }
	static vec4d unit_w() { return vec4d(0.0, 0.0, 0.0, 1.0); }
};

struct vec4
{
	union
	{
		struct
		{
			float x, y, z, w;
		};

		float data[4];
	};

	enum { vector_size = 4 };

	vec4(const float& s = 0)
		: x(s)
		, y(s)
		, z(s)
		, w(s)
	{ }

	vec4(
		const float& x,
		const float& y,
		const float& z,
		const float& w)
		: x(x)
		, y(y)
		, z(z)
		, w(w)
	{ }

	vec4(const float *vec)
	{
		memcpy(data, vec, 4 * sizeof(float));
	}

	vec4(
		const vec2& xy,
		const vec2& zw)
		: x(xy.x)
		, y(xy.y)
		, z(zw.x)
		, w(zw.y)
	{ }

	vec4(
		const vec3& xyz,
		const float& w)
		: x(xyz.x)
		, y(xyz.y)
		, z(xyz.z)
		, w(w)
	{ }

	vec4(
		const float& x,
		const vec3& yzw)
		: x(x)
		, y(yzw.x)
		, z(yzw.y)
		, w(yzw.z)
	{ }

	vec4(
		const float& x,
		const float& y,
		const vec2&  zw)
		: x(x)
		, y(y)
		, z(zw.x)
		, w(zw.y)
	{ }
};

struct vec4ub
{
	union
	{
		struct
		{
			unsigned char x, y, z, w;
		};

		unsigned char data[4];
	};

	enum { vector_size = 4 };

	vec4ub(const unsigned char& s = 0) 
		: x(s)
		, y(s)
		, z(s)
		, w(s)
	{ }

	vec4ub(
		const unsigned char& x, 
		const unsigned char& y, 
		const unsigned char& z, 
		const unsigned char& w) 
		: x(x)
		, y(y)
		, z(z)
		, w(w)
	{ }

	vec4ub(const unsigned char* vec)
	{ 
		memcpy(data, vec, 4 * sizeof(unsigned char));
	}
};

struct vec4ui
{
	union
	{
		struct
		{
			unsigned int x, y, z, w;
		};

		unsigned int data[4];
	};

	enum { vector_size = 4 };

	vec4ui(const unsigned int& s = 0)
		: x(s)
		, y(s)
		, z(s)
		, w(s)
	{ }

	vec4ui(
		const unsigned int& x,
		const unsigned int& y,
		const unsigned int& z,
		const unsigned int& w)
		: x(x)
		, y(y)
		, z(z)
		, w(w)
	{ }

	vec4ui(const unsigned int* vec)
	{
		memcpy(data, vec, 4 * sizeof(unsigned int));
	}
};

struct vec4b
{
	union
	{
		struct
		{
			char x, y, z, w;
		};

		char data[4];
	};
	enum { vector_size = 4 };
	
	vec4b(const char& s = 0)
		: x(s)
		, y(s)
		, z(s)
		, w(s)
	{ }

	vec4b(
		const char& x,
		const char& y,
		const char& z,
		const char& w)
		: x(x)
		, y(y)
		, z(z)
		, w(w)
	{ }

	vec4b(const char* vec)
	{
		memcpy(data, vec, 4 * sizeof(char));
	}
};



struct evec3d
{
	union
	{
		struct
		{
			/// 
			/// The low bits for each component.  
			/// Bits 7 to 22 store the whole value, and bits 0 to 6 store the fraction.  
			/// Bits 23 to 31 are not used.
			/// 
			vec3 low;

			///
			/// The high bits for each component.  Bits 0 to 22 store the whole value.  Bits 23 to 31 are not used.
			///
			vec3 high;
		};

		vec3 data[2];
	};

	evec3d(
		vec3 low  = vec3(0.0f),
		vec3 high = vec3(0.0f))
		: low(low)
		, high(high)
	{ }


	evec3d(vec3d v)
	{
		encode(v.x, low.x, high.x);
		encode(v.y, low.y, high.y);
		encode(v.z, low.z, high.z);
	}


	/// Encode a 64-bits double in two 32-bits floats.
	/// @param {double} value The 64-bits double to encode.
	/// @param {float} low The bits 7 to 22 store the whole value, and bits 0 to 6 store the fraction. Bits 23 to 31 are not used.
	/// @param {float} high The high bits for each component.  Bits 0 to 22 store the whole value.  Bits 23 to 31 are not used.
	inline static void encode(
		const double& value,
		float& low,
		float& high)
	{
		float double_high;
		if (value >= 0.0)
		{
			double_high = std::floor(value / 65536.0f) * 65536.0f;
			high = double_high;
			low  = value - double_high;
		}
		else
		{
			double_high = std::floor(-value / 65536.0f) * 65536.0f;
			high = -double_high;
			low  = value + double_high;
		}
	}
};



//////////////////////////////////////////////////////////////////
//						OPERATORS
//////////////////////////////////////////////////////////////////


template<typename T>
inline T operator-(const T& a)
{
	T res;
	for (unsigned int i = 0; i < T::vector_size; i++)
		res.data[i] = -a.data[i];
	return res;
}

template<typename T>
inline T operator+(const T& a, const T& b)
{
	T res;
	for (unsigned int i = 0; i < T::vector_size; i++)
		res.data[i] = a.data[i] + b.data[i];
	return res;
}

template<typename T>
inline T operator+(const T& a, const double b)
{
	T res;
	for (unsigned int i = 0; i < T::vector_size; i++)
		res.data[i] = a.data[i] + b;
	return res;
}

template<typename T>
inline T operator-(const T& a, const double b)
{
	T res;
	for (unsigned int i = 0; i < T::vector_size; i++)
		res.data[i] = a.data[i] - b;
	return res;
}

template<typename T>
inline T operator-(const T& a, const T& b)
{
	T res;
	for (unsigned int i = 0; i < T::vector_size; i++)
		res.data[i] = a.data[i] - b.data[i];
	return res;
}	

template<typename T>
inline T operator*(const T& vector, const double& scalar)
{
	T res;
	for (unsigned int i = 0; i < T::vector_size; i++)
		res.data[i] = vector.data[i] * scalar;
	return res;
}

//template<typename T>
//inline T operator*(const double& scalar, const T& vector)
//{
//	T res;
//	for (unsigned int i = 0; i < T::vector_size; i++)
//		res.data[i] = vector.data[i] * scalar;
//	return res;
//}

template<typename T>
inline T operator*(const T& a, const T& b)
{
	T res;
	for (unsigned int i = 0; i < T::vector_size; i++)
		res.data[i] = a.data[i] * b.data[i];
	return res;
}

template<typename T>
inline T operator/(const T& a, const T& b)
{
	T res;
	for (unsigned int i = 0; i < T::vector_size; i++)
		res.data[i] = a.data[i] / b.data[i];
	return res;
}

template<typename T>
inline T operator/(const T& a, const double& b)
{
	T res;
	for (unsigned int i = 0; i < T::vector_size; i++)
		res.data[i] = a.data[i] / b;
	return res;
}

template<typename T>
inline T& operator*=(T& a, const T& b)
{
	for (unsigned int i = 0; i < T::vector_size; i++)
		a.data[i] *= b.data[i];
	return a;
}

template<typename T>
inline T& operator*=(T& a, const double& b)
{
	for (unsigned int i = 0; i < T::vector_size; i++)
		a.data[i] *= b;
	return a;
}

template<typename T>
inline T& operator*=(T& a, const float& b)
{
	for (unsigned int i = 0; i < T::vector_size; i++)
		a.data[i] *= b;
	return a;
}

template<typename T>
inline T& operator/=(T& a, const T& b)
{
	for (unsigned int i = 0; i < T::vector_size; i++)
		a.data[i] /= b.data[i];
	return a;
}

template<typename T>
inline T& operator/=(T& a, const double& b)
{
	for (unsigned int i = 0; i < T::vector_size; i++)
		a.data[i] /= b;
	return a;
}

template<typename T>
inline T& operator/=(T& a, const float& b)
{
	for (unsigned int i = 0; i < T::vector_size; i++)
		a.data[i] /= b;
	return a;
}

template<typename T>
inline T& operator+=(T& a, const T& b)
{
	for (unsigned int i = 0; i < T::vector_size; i++)
		a.data[i] += b.data[i];
	return a;
}

template<typename T>
inline T& operator-=(T& a, const T& b)
{
	for (unsigned int i = 0; i < T::vector_size; i++)
		a.data[i] -= b.data[i];
	return a;
}

inline bool operator==(const vec2i& a, const vec2i& b)
{
	for (unsigned int i = 0; i < vec2i::vector_size; i++)
	{
		if (a.data[i] != b.data[i])
			return false;
	}
	return true;
}

inline bool operator==(const vec2& a, const vec2& b)
{
	for (unsigned int i = 0; i < vec2::vector_size; i++)
	{
		if (a.data[i] != b.data[i])
			return false;
	}
	return true;
}

inline bool operator==(const vec3d& a, const vec3d& b)
{
	for (unsigned int i = 0; i < vec3d::vector_size; i++)
	{
		if (a.data[i] != b.data[i])
			return false;
	}
	return true;
}

inline bool operator==(const vec3& a, const vec3& b)
{
	for (unsigned int i = 0; i < vec3::vector_size; i++)
	{
		if (a.data[i] != b.data[i])
			return false;
	}
	return true;
}

inline bool operator==(const vec4& a, const vec4& b)
{
	for (unsigned int i = 0; i < vec4::vector_size; i++)
	{
		if (a.data[i] != b.data[i])
			return false;
	}
	return true;
}

inline bool operator==(const vec4d& a, const vec4d& b)
{
	for (unsigned int i = 0; i < vec4d::vector_size; i++)
	{
		if (a.data[i] != b.data[i])
			return false;
	}
	return true;
}

inline bool operator!=(const vec2i& a, const vec2i& b)
{
	for (unsigned int i = 0; i < vec2i::vector_size; i++)
	{
		if (a.data[i] != b.data[i])
			return true;
	}
	return false;
}

inline bool operator!=(const vec2& a, const vec2& b)
{
	for (unsigned int i = 0; i < vec2::vector_size; i++)
	{
		if (a.data[i] != b.data[i])
			return true;
	}
	return false;
}

inline bool operator!=(const vec3d& a, const vec3d& b)
{
	for (unsigned int i = 0; i < vec3d::vector_size; i++)
	{
		if (a.data[i] != b.data[i])
			return true;
	}
	return false;
}

inline bool operator!=(const vec3& a, const vec3& b)
{
	for (unsigned int i = 0; i < vec3::vector_size; i++)
	{
		if (a.data[i] != b.data[i])
			return true;
	}
	return false;
}

inline bool operator!=(const vec4& a, const vec4& b)
{
	for (unsigned int i = 0; i < vec4::vector_size; i++)
	{
		if (a.data[i] != b.data[i])
			return true;
	}
	return false;
}

inline bool operator!=(const vec4d& a, const vec4d& b)
{
	for (unsigned int i = 0; i < vec4d::vector_size; i++)
	{
		if (a.data[i] != b.data[i])
			return true;
	}
	return false;
}

template<typename T>
inline bool operator<(const T& a, const T& b)
{
    for (unsigned int i = 0; i < T::vector_size; i++)
        if(a.data[i] >= b.data[i])
            return false;
    return true;
}

inline bool operator==(const evec3d& a, const evec3d& b)
{
	return a.high == b.high &&
		a.low == b.low;
}

inline bool operator!=(const evec3d& a, const evec3d& b)
{
	return a.high != b.high &&
		a.low != b.low;
}


//////////////////////////////////////////////////////////////////
//						OPERATORS
//////////////////////////////////////////////////////////////////


/// Calculate the cross product between two vectors.
/// @param {vec3d} a First vector.
/// @param {vec3d} b Second vector.
/// @result {vec3d} The resulting cross product.
inline vec3d vec_cross(
	const vec3d& a, 
	const vec3d& b)
{
	return vec3d(
		a.y * b.z - b.y * a.z,
		a.z * b.x - b.z * a.x,
		a.x * b.y - b.x * a.y);
}

///
/// Calculate the cross product between two vectors.
/// @param {vec3} a First vector.
/// @param {vec3} b Second vector.
/// @result {vec3} The resulting cross product.
///
inline vec3 vec_cross(
	const vec3& a, 
	const vec3& b)
{
	return vec3(
		a.y * b.z - b.y * a.z,
		a.z * b.x - b.z * a.x,
		a.x * b.y - b.x * a.y);
}

template<typename T>
inline T vec_lerp(
        const T& start,
        const T& end,
        const double& t)
{
    return (end * t) + start * (1.0 - t);
}

template<typename T>
inline T vec_lerp(
        const T& start,
        const T& end,
        const float& t)
{
    return (end * t) + start * (1.0 - t);
}

///
/// Calculate the dot product between two vectors.
/// @param {T} a First vector.
/// @param {T} a Second vector.
/// @result {double} The resulting dot product.
///
template<typename T>
inline double vec_dot(
	const T& a, 
	const T& b)
{
	double sum = 0.0f;
	for (unsigned int i = 0; i < T::vector_size; i++)
	{
		sum += a.data[i] * b.data[i];
	}
	return sum;
}

///
/// Calculate the distance between two vectors.
/// @param {T} lefth First vector.
/// @param {T} right Second vector.
/// @result {double} The distance.
/// 
template<typename T>
inline double vec_distance(
	const T& left, 
	const T& right)
{
	T distance = left - right;
	return vec_length(distance);
}

///
/// Calculate the magnitude of a vector.
/// @param {T} vec The vector.
/// @result {double} The vector's magnitude.
/// 
template<typename T>
inline double vec_length(const T& vec)
{
	return std::sqrt(vec_dot(vec, vec));
}

///
/// Calculate the magnitude of a vector.
/// @param {T} vec The vector.
/// @result {double} The vector's magnitude.
/// 
template<typename T>
inline double vec_length_squared(const T& vec)
{
	return vec_dot(vec, vec);
}

///
/// Get the maximum component value.
/// @param {T} vec The vector.
/// @result {double} The maximum component value.
/// 
template<typename T>
inline double vec_max_component(const T& vec)
{
	double max = -std::numeric_limits<double>::max();
	for (unsigned int i = 0; i < T::vector_size; i++)
	{
		max = ::fmax(max, vec.data[i]);
	}
	return max;
}

///
/// Get the minimum component value.
/// @param {T} vec The vector.
/// @result {double} The minimum component value.
/// 
template<typename T>
inline double vec_min_component(const T& vec)
{
	double min = std::numeric_limits<double>::max();
	for (unsigned int i = 0; i < T::vector_size; i++)
	{
		min = ::fmin(min, vec.data[i]);
	}
	return min;
}


///
/// Get the minimum component value.
/// @param {T} vec The vector.
/// @result {double} The minimum component value.
/// 
template<typename T>
inline T vec_min_by_component(
	const T& vec1,
	const T& vec2)
{
	T res;
	for (unsigned int i = 0; i < T::vector_size; i++)
	{
		res.data[i] = ::fmin(vec1.data[i], vec2.data[i]);
	}
	return res;
}

template<typename T>
inline T vec_max_by_component(
	const T& vec1,
	const T& vec2)
{
	T res;
	for (unsigned int i = 0; i < T::vector_size; i++)
	{
		res.data[i] = ::fmax(vec1.data[i], vec2.data[i]);
	}
	return res;
}

///
/// Get the normalized vector.
/// @param {T} vec The vector.
/// @result {T} The normalized vector.
/// 
template<typename T>
inline T vec_normalize(const T& vec)
{
	return vec / vec_length(vec);
}

///
/// Get the a vector with where the components are the floor of the input vector's components.
/// @param {T} vec The vector.
/// @result {T} The resulting vector.
/// 
template<typename T>
inline T vec_floor(const T& vec)
{
	T res;
	for (unsigned int i = 0; i < T::vector_size; i++)
	{
		res.data[i] = std::floor(vec.data[i]);
	}
	return res;
}


template<typename T>
inline bool vec_equals_epsilon(
	const T& vec0, 
	const T& vec1, 
	const double& epsilon)
{
	bool res = true;
	for (unsigned int i = 0; i < T::vector_size; i++)
	{
		res = res && (std::abs(vec0.data[i] - vec1.data[i]) < epsilon);
	}
	return res;
}

template<typename T>
inline bool vec_undefined(const T& v)
{
    bool res = false;
    for (unsigned int i = 0; i < T::vector_size && !res; i++)
    {
        res = std::isnan(v.data[i]);
    }
    return res;
}

///
/// Get the a vector with where the components are the diference between the floor of the input vector's components
/// and the input vector.
/// @param {T} vec The vector.
/// @result {T} The resulting vector.
/// 
template<typename T>
inline T vec_fract(const T& vec)
{
	return vec - vec_floor(vec);
}

///
/// Get the projected vec3 from a vec3.
/// @param {vec4} vec The vector.
/// @result {vec3} The resulting vector.
/// 
inline vec3 vec_project(const vec4& vec)
{
	return vec3(vec.data) / vec3(vec.w);
}

///
/// Cast a vec3d to a vec3.
/// @param {vec3d} vec The vector.
/// @result {vec3} The casted vector.
/// 
inline vec3 to_vec3(const vec3d& vec)
{
	return vec3((float)vec.x, (float)vec.y, (float)vec.z);
}

///
/// Cast a vec3 to a vec3d.
/// @param {vec3} vec The vector.
/// @result {vec3d} The casted vector.
/// 
inline vec3d to_vec3d(const vec3& vec)
{
	return vec3d((double)vec.x, (double)vec.y, (double)vec.z);
}

///
/// Cast a vec2i to a vec2.
/// @param {vec2i} vec The vector.
/// @result {vec2} The casted vector.
/// 
inline vec2 to_vec2(const vec2i& vec)
{
	return vec2(vec.x, vec.y);
}


///
/// Cast a vec3d to a vec3.
/// @param {vec3d} vec The vector.
/// @result {vec3} The casted vector.
///
inline vec4 to_vec4(const vec4d& vec)
{
    return vec4((float)vec.x, (float)vec.y, (float)vec.z, (float)vec.w);
}

inline vec3d to_vec3d(const vec4d& vec)
{
    return vec3d(vec.x, vec.y, vec.z);
}

inline vec3 to_vec3(const vec4& vec)
{
    return vec3(vec.x, vec.y, vec.z);
}

///
/// Rotate a vector in the given value.
/// @param {vec2} v The vector.
/// @param {double} radians The angle to rotate.
/// @result {vec3d} The rotated vector.
/// 
inline vec2 vec_rotate(
	const vec2& v,
	const double& radians)
{
	double ca = cos(radians);
	double sa = sin(radians);
	return vec2(ca * v.x - sa * v.y, sa * v.x + ca * v.y);
}


template<typename T>
inline std::string  vec_string(const T& vec)
{
	std::ostringstream stream;
	stream << "(";
	unsigned int size = T::vector_size;
	for (unsigned int i = 0; i < size; i++)
	{
		stream << vec.data[i];
		if (i != size - 1)
		{
			stream << ",";
		}
	}
	stream << ")";
	return stream.str();
}


inline std::ostream & operator<< (
	std::ostream &out,
	vec2 const &t)
{
	out << vec_string(t);
	return out;
}

inline std::ostream & operator<< (
	std::ostream &out,
	vec3 const &t)
{
	out << vec_string(t);
	return out;
}

inline std::ostream & operator<< (
		std::ostream &out,
		vec3d const &t)
{
	out << vec_string(t);
	return out;
}

inline std::ostream & operator<< (
	std::ostream &out, 
	vec4 const &t)
{
	out << vec_string(t);
	return out;
}
