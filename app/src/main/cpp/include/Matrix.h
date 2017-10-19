
#pragma once

#include <string.h>
#include <cmath>
#include <stdexcept>
#include <sstream>

#include "Vector.h"
#include "Rectangle.h"
#include "Point.h"

namespace argeo
{
	struct mat3d
	{
		union
		{
			struct
			{
				double row0_column0, row0_column1, row0_column2;
				double row1_column0, row1_column1, row1_column2;
				double row2_column0, row2_column1, row2_column2;
			};

			double data[9];
		};
		

		mat3d(const double& s = 0.0) 
		{
			for (unsigned int i = 0; i < 9; i++) data[i] = s; 
		}

		mat3d(const double& c00, const double& c01, const double& c02,
			  const double& c10, const double& c11, const double& c12,
			  const double& c20, const double& c21, const double& c22)
		{
			data[0] = c00; data[1] = c01; data[2] = c02;
			data[3] = c10; data[4] = c11; data[5] = c12;
			data[6] = c20; data[7] = c21; data[8] = c22;
		}

		mat3d(const double* mat)
		{
			memcpy(data, mat, 9 * sizeof(double)); 
		}

		static mat3d identity()
		{
			return mat3d(
				1, 0, 0,
				0, 1, 0,
				0, 0, 1);
		}

		enum { matrix_size = 9 };
		double& operator() (unsigned row, unsigned col);      
		double  operator() (unsigned row, unsigned col) const;
	};

	struct mat3
	{
		union
		{
			struct
			{
				float row0_column0, row0_column1, row0_column2;
				float row1_column0, row1_column1, row1_column2;
				float row2_column0, row2_column1, row2_column2;
			};

			float data[9];
		};

		mat3(const float& s = 0.0f)
		{
			for (unsigned int i = 0; i < 9; i++) data[i] = s; 
		}

		mat3(const float& c00, const float& c01, const float& c02,
			 const float& c10, const float& c11, const float& c12,
			 const float& c20, const float& c21, const float& c22)
		{
			data[0] = c00; data[1] = c01; data[2] = c02;
			data[3] = c10; data[4] = c11; data[5] = c12;
			data[6] = c20; data[7] = c21; data[8] = c22;
		}

		mat3(const float* mat) 
		{
			memcpy(data, mat, 9 * sizeof(float));
		}

		static mat3 identity()
		{
			return mat3(
				1, 0, 0,
				0, 1, 0,
				0, 0, 1);
		}

		enum { matrix_size = 9 };

		float& operator() (unsigned row, unsigned col);        
		float  operator() (unsigned row, unsigned col) const;  

	};

	struct mat42
	{
		union
		{
			struct
			{
				float row0_column0, row0_column1, row0_column2, row0_column3;
				float row1_column0, row1_column1, row1_column2, row1_column3;
				float row2_column0, row2_column1, row2_column2, row2_column3;
			};

			float data[8];
		};

		mat42(const float& s = 0.0f)
		{
			for (unsigned int i = 0; i < 8; i++) data[i] = s; 
		}

		mat42(const float& c00, const float& c01, const float& c02, const float& c03,
			  const float& c10, const float& c11, const float& c12, const float& c13)
		{
			data[0] = c00; data[1] = c01; data[2] = c02; data[3] = c03;
			data[4] = c10; data[5] = c11; data[6] = c12; data[7] = c13;
		}

		mat42(const float* mat)
		{ 
			memcpy(data, mat, 8 * sizeof(float)); 
		}

		enum { matrix_size = 8 };

		float& operator() (unsigned row, unsigned col);       
		float  operator() (unsigned row, unsigned col) const;
	};

	struct mat4d
	{
		union
		{
			struct
			{
				double row0_column0, row0_column1, row0_column2, row0_column3;
				double row1_column0, row1_column1, row1_column2, row1_column3;
				double row2_column0, row2_column1, row2_column2, row2_column3;
				double row3_column0, row3_column1, row3_column2, row3_column3;

			};

			double data[16];
		};

		mat4d(const double& s = 0.0)
		{
			for (unsigned int i = 0; i < 16; i++) data[i] = s; 
		}

		mat4d(const double& c00, const double& c01, const double& c02, const double& c03,
			  const double& c10, const double& c11, const double& c12, const double& c13,
			  const double& c20, const double& c21, const double& c22, const double& c23,
			  const double& c30, const double& c31, const double& c32, const double& c33)
		{
			data[0] = c00;  data[1] = c01;  data[2] = c02;  data[3] = c03;
			data[4] = c10;  data[5] = c11;  data[6] = c12;  data[7] = c13;
			data[8] = c20;  data[9] = c21;  data[10] = c22; data[11] = c23;
			data[12] = c30; data[13] = c31; data[14] = c32; data[15] = c33;
		}

		mat4d(const mat3d& mat)
		{
			data[0] = mat.data[0]; data[1] = mat.data[1]; data[2] = mat.data[2]; data[3] = 0;
			data[4] = mat.data[3]; data[5] = mat.data[4]; data[6] = mat.data[5]; data[7] = 0;
			data[8] = mat.data[6]; data[9] = mat.data[7]; data[10] = mat.data[8]; data[11] = 0;
			data[12] = 0; data[13] = 0; data[14] = 0; data[15] = 1;
		}

		mat4d(const double* mat) 
		{ 
			memcpy(data, mat, 16 * sizeof(double));
		}

		static mat4d identity()
		{
			return mat4d(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1);
		}

		enum { matrix_size = 16 };

		double& operator() (unsigned row, unsigned col);       
		double  operator() (unsigned row, unsigned col) const;  	

	};

	struct mat4
	{
		union
		{
			struct
			{
				float row0_column0, row0_column1, row0_column2, row0_column3;
				float row1_column0, row1_column1, row1_column2, row1_column3;
				float row2_column0, row2_column1, row2_column2, row2_column3;
				float row3_column0, row3_column1, row3_column2, row3_column3;

			};

			float data[16];
		};

		mat4(const float& s = 0.0f)
		{
			for (unsigned int i = 0; i < 16; i++) data[i] = s; 
		}

		mat4(const float& c00, const float& c01, const float& c02, const float& c03,
			 const float& c10, const float& c11, const float& c12, const float& c13,
			 const float& c20, const float& c21, const float& c22, const float& c23,
			 const float& c30, const float& c31, const float& c32, const float& c33)
		{
			data[0] = c00; data[1] = c01; data[2] = c02; data[3] = c03;
			data[4] = c10; data[5] = c11; data[6] = c12; data[7] = c13;
			data[8] = c20; data[9] = c21; data[10] = c22; data[11] = c23;
			data[12] = c30; data[13] = c31; data[14] = c32; data[15] = c33;
		}

		mat4(const float* mat)
		{ 
			memcpy(data, mat, 16 * sizeof(float));
		}

		static mat4 identity()
		{
			return mat4(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1);
		}

		enum { matrix_size = 16 };

		float& operator() (unsigned row, unsigned col);       
		float  operator() (unsigned row, unsigned col) const; 
	};

	////////////////////////////////////////////////////////////////////////////////
	//						OVERLOADED OPERATORS
	////////////////////////////////////////////////////////////////////////////////

	inline bool mat4d_equals_epsilon(
		const mat4d& mat0,
		const mat4d& mat1,
		const double& epsilon)
	{
		bool res = true;
		for (unsigned int i = 0; i < mat4d::matrix_size; i++)
		{
			res = res && (std::abs(mat0.data[i] - mat1.data[i]) < epsilon);
		}
		return res;
	}

	inline mat4d& operator*=(mat4d& a, const mat4d& b)
	{
		mat4d cached = a;
		for (unsigned int r = 0; r < 4; r++)
		{
			for (unsigned int c = 0; c < 4; c++)
			{
				double sum = 0.0f;
				for (unsigned int k = 0; k < 4; k++)
					sum += cached.data[r + 4 * k] * b.data[4 * c + k];
				a.data[r + 4 * c] = sum;
			}
		}
		return a;
	}

	inline mat4d operator*(const mat4d& a, const mat4d& b)
	{
		mat4d res;
		for (unsigned int r = 0; r < 4; r++)
		{
			for (unsigned int c = 0; c < 4; c++)
			{
				double sum = 0.0f;
				for (unsigned int k = 0; k < 4; k++)
					sum += a.data[r + 4 * k] * b.data[4 * c + k];
				res.data[r + 4 * c] = sum;
			}
		}
		return res;
	}

    inline mat4d operator*(const mat4d& a, const mat3d& b)
    {
        mat4d res;

        const double left0 =  a.data[0];
        const double left1 =  a.data[1];
        const double left2 =  a.data[2];
        const double left4 =  a.data[4];
        const double left5 =  a.data[5];
        const double left6 =  a.data[6];
        const double left8 =  a.data[8];
        const double left9 =  a.data[9];
        const double left10 = a.data[10];

        const double right0 = b.data[0];
        const double right1 = b.data[1];
        const double right2 = b.data[2];
        const double right4 = b.data[3];
        const double right5 = b.data[4];
        const double right6 = b.data[5];
        const double right8 = b.data[6];
        const double right9 = b.data[7];
        const double right10 = b.data[8];

        const double column0Row0 = left0 * right0 + left4 * right1 + left8 * right2;
        const double column0Row1 = left1 * right0 + left5 * right1 + left9 * right2;
        const double column0Row2 = left2 * right0 + left6 * right1 + left10 * right2;

        const double column1Row0 = left0 * right4 + left4 * right5 + left8 * right6;
        const double column1Row1 = left1 * right4 + left5 * right5 + left9 * right6;
        const double column1Row2 = left2 * right4 + left6 * right5 + left10 * right6;

        const double column2Row0 = left0 * right8 + left4 * right9 + left8 * right10;
        const double column2Row1 = left1 * right8 + left5 * right9 + left9 * right10;
        const double column2Row2 = left2 * right8 + left6 * right9 + left10 * right10;

        res.data[0] = column0Row0;
        res.data[1] = column0Row1;
        res.data[2] = column0Row2;
        res.data[3] = 0.0;
        res.data[4] = column1Row0;
        res.data[5] = column1Row1;
        res.data[6] = column1Row2;
        res.data[7] = 0.0;
        res.data[8] = column2Row0;
        res.data[9] = column2Row1;
        res.data[10] = column2Row2;
        res.data[11] = 0.0;
        res.data[12] = a.data[12];
        res.data[13] = a.data[13];
        res.data[14] = a.data[14];
        res.data[15] = a.data[15];

        return res;
    }


    inline mat4& operator*=(mat4& a, const mat4& b)
	{
		mat4 cached = a;
		for (unsigned int r = 0; r < 4; r++)
		{
			for (unsigned int c = 0; c < 4; c++)
			{
				double sum = 0.0f;
				for (unsigned int k = 0; k < 4; k++)
					sum += cached.data[r + 4 * k] * b.data[4 * c + k];
				a.data[r + 4 * c] = sum;
			}
		}
		return a;
	}

	inline mat4 operator*(const mat4& a, const mat4& b)
	{
		mat4 res;
		for (unsigned int r = 0; r < 4; r++)
		{
			for (unsigned int c = 0; c < 4; c++)
			{
				float sum = 0.0f;
				for (unsigned int k = 0; k < 4; k++)
					sum += a.data[r + 4 * k] * b.data[4 * c + k];
				res.data[r + 4 * c] = sum;
			}
		}
		return res;
	}

	inline mat4d& operator*=(mat4d& mat, double v)
	{
		for (unsigned int i = 0; i < 16; i++)
			mat.data[i] *= v;
		return mat;
	}

	inline mat4& operator*=(mat4& mat, float v)
	{
		for (unsigned int i = 0; i < 16; i++)
			mat.data[i] *= v;
		return mat;
	}

	inline mat4d mat_look_at(
		const vec3d& eye,
		const vec3d& center,
		const vec3d& up)
	{
		vec3d zaxis = vec_normalize(center - eye);
		vec3d xaxis = vec_normalize(vec_cross(zaxis, up));
		vec3d yaxis = vec_cross(xaxis, zaxis);

		return mat4d(
			xaxis.x, yaxis.x, -zaxis.x, 0.0f,
			xaxis.y, yaxis.y, -zaxis.y, 0.0f,
			xaxis.z, yaxis.z, -zaxis.z, 0.0f,
			-vec_dot(xaxis, eye), -vec_dot(yaxis, eye), -vec_dot(-zaxis, eye), 1.0f);
	}

	inline mat4 mat_look_at(
		const vec3& eye,
		const vec3& center,
		const vec3& up)
	{
		vec3 zaxis = vec_normalize(center - eye);
		vec3 xaxis = vec_normalize(vec_cross(zaxis, up));
		vec3 yaxis = vec_cross(xaxis, zaxis);

		return mat4(
			xaxis.x, yaxis.x, -zaxis.x, 0.0f,
			xaxis.y, yaxis.y, -zaxis.y, 0.0f,
			xaxis.z, yaxis.z, -zaxis.z, 0.0f,
			-vec_dot(xaxis, eye), -vec_dot(yaxis, eye), -vec_dot(-zaxis, eye), 1.0f);
	}

	inline vec4 operator*(const mat4& mat, const vec4& vec)
	{
		vec4 res(0.0f);
		for (unsigned int i = 0; i < 4; i++)
		{
			res += vec4(mat.data + 4 * i) * vec4(vec.data[i]);
		}
		return res;
	}

	inline vec4d operator*=(vec4d& vec, const mat4d& mat)
	{
		vec4d res(0.0f);
		for (unsigned int i = 0; i < 4; i++)
		{
			res += vec4d(mat.data + 4 * i) * vec4d(vec.data[i]);
		}
		vec = res;
		return vec;
	}

	inline vec4d operator*(const vec4d& vec, const mat4d& mat)
	{
		vec4d res(0.0f);
		for (unsigned int i = 0; i < 4; i++)
		{
			res += vec4d(mat.data + 4 * i) * vec4d(vec.data[i]);
		}
		return res;
	}

	inline vec4d operator*(const mat4d& mat, const vec4d& vec)
	{
		vec4d res(0.0f);
		for (unsigned int i = 0; i < 4; i++)
		{
			res += vec4d(mat.data + 4 * i) * vec4d(vec.data[i]);
		}
		return res;
	}


	inline vec3d operator*(const vec3d& vec, const mat4d& matrix)
	{
		double v_x = vec.x;
		double v_y = vec.y;
		double v_z = vec.z;

		return vec3d(
			matrix.data[0] * v_x + matrix.data[4] * v_y + matrix.data[8] * v_z + matrix.data[12],
			matrix.data[1] * v_x + matrix.data[5] * v_y + matrix.data[9] * v_z + matrix.data[13],
			matrix.data[2] * v_x + matrix.data[6] * v_y + matrix.data[10] * v_z + matrix.data[14]
		);
	}

	inline vec3d operator*(const mat4d& matrix, const vec3d& vec)
	{
		double v_x = vec.x;
		double v_y = vec.y;
		double v_z = vec.z;

		return vec3d(
			matrix.data[0] * v_x + matrix.data[4] * v_y + matrix.data[8]  * v_z + matrix.data[12],
			matrix.data[1] * v_x + matrix.data[5] * v_y + matrix.data[9]  * v_z + matrix.data[13],
			matrix.data[2] * v_x + matrix.data[6] * v_y + matrix.data[10] * v_z + matrix.data[14]
		);
	}

	inline vec3 operator*(const mat3& mat, const vec3& vec)
	{
		vec3 res(0.0f);
		for (unsigned int i = 0; i < 3; i++)
		{
			res += vec3(mat.data + 3 * i) * vec3(vec.data[i]);
		}
		return res;
	}

	inline vec3d operator*(const mat3d& mat, const vec3d& vec)
	{
		vec3d res(0.0);
		for (unsigned int i = 0; i < 3; i++)
		{
			res += vec3d(mat.data + 3 * i) * vec3d(vec.data[i]);
		}
		return res;
	}

	inline bool operator==(const mat3& a, const mat3& b)
	{
		for (unsigned int i = 0; i < mat3::matrix_size; i++)
		{
			if (a.data[i] != b.data[i])
				return false;
		}
		return true;
	}

	inline bool operator!=(const mat3& a, const mat3& b)
	{
		for (unsigned int i = 0; i < mat3::matrix_size; i++)
		{
			if (a.data[i] != b.data[i])
				return true;
		}
		return false;
	}

	inline bool operator==(const mat4& a, const mat4& b)
	{
		for (unsigned int i = 0; i < mat4::matrix_size; i++)
		{
			if (a.data[i] != b.data[i])
				return false;
		}
		return true;
	}

	inline bool operator!=(const mat4& a, const mat4& b)
	{
		for (unsigned int i = 0; i < mat4::matrix_size; i++)
		{
			if (a.data[i] != b.data[i])
				return true;
		}
		return false;
	}

	inline bool operator==(const mat4d& a, const mat4d& b)
	{
		for (unsigned int i = 0; i < mat4::matrix_size; i++)
		{
			if (a.data[i] != b.data[i])
				return false;
		}
		return true;
	}

	inline bool operator!=(const mat4d& a, const mat4d& b)
	{
		for (unsigned int i = 0; i < mat4::matrix_size; i++)
		{
			if (a.data[i] != b.data[i])
				return true;
		}
		return false;
	}

	inline bool operator==(const mat42& a, const mat42& b)
	{
		for (unsigned int i = 0; i < mat42::matrix_size; i++)
		{
			if (a.data[i] != b.data[i])
				return false;
		}
		return true;
	}

	inline bool operator!=(const mat42& a, const mat42& b)
	{
		for (unsigned int i = 0; i < mat42::matrix_size; i++)
		{
			if (a.data[i] != b.data[i])
				return true;	
		}
		return false;
	}

	inline float& mat42::operator() (unsigned row, unsigned col)
	{
		if (row > 2 || col > 3)
			throw std::out_of_range("Matrix subscript out of bounds");
		return data[4 * row + col];
	}

	inline float mat42::operator() (unsigned row, unsigned col) const
	{
		if (row > 2 || col > 3)
			throw std::out_of_range("Matrix subscript out of bounds");
		return data[4 * row + col];
	}

	inline double& mat3d::operator() (unsigned row, unsigned col)
	{
		if (row > 2 || col > 2)
			throw std::out_of_range("Matrix subscript out of bounds");
		return data[3 * row + col];
	}

	inline double mat3d::operator() (unsigned row, unsigned col) const
	{
		if (row > 2 || col > 2)
			throw std::out_of_range("Matrix subscript out of bounds");
		return data[3 * row + col];
	}

	inline float& mat3::operator() (unsigned row, unsigned col)
	{
		if (row > 2 || col > 2)
			throw std::out_of_range("Matrix subscript out of bounds");
		return data[3 * row + col];
	}

	inline float mat3::operator() (unsigned row, unsigned col) const
	{
		if (row > 2 || col > 2)
			throw std::out_of_range("Matrix subscript out of bounds");
		return data[3 * row + col];
	}


	inline double& mat4d::operator() (unsigned row, unsigned col)
	{
		if (row > 3 || col > 3)
			throw std::out_of_range("Matrix subscript out of bounds");
		return data[4 * row + col];
	}


	inline double mat4d::operator() (unsigned row, unsigned col) const
	{
		if (row > 3 || col > 3)
			throw std::out_of_range("Matrix subscript out of bounds");
		return data[4 * row + col];
	}

	inline float mat4::operator() (unsigned row, unsigned col) const
	{
		if (row > 3 || col > 3)
			throw std::out_of_range("Matrix subscript out of bounds");
		return data[4 * row + col];
	}

	inline float& mat4::operator() (unsigned row, unsigned col)
	{
		if (row > 3 || col > 3)
			throw std::out_of_range("Matrix subscript out of bounds");
		return data[4 * row + col];
	}

	////////////////////////////////////////////////////////////////////////////////
	//						HELPER FUNCTIONS
	////////////////////////////////////////////////////////////////////////////////

	/// 
	/// Cast a 3 x 3 matrix with doubles to a 3 x 3 matrix with floats.
	/// @param   {mat3d} mat The matrix.
	/// @result  {mat3} The casted matrix.
	/// 
	inline mat3 to_mat3(const mat3d& mat)
	{
		return mat3(
			(float)mat.data[0], (float)mat.data[1], (float)mat.data[2],
			(float)mat.data[3], (float)mat.data[4], (float)mat.data[5],
			(float)mat.data[6], (float)mat.data[7], (float)mat.data[8]);
	}

	/// 
	/// Cast a 4 x 4 matrix with doubles to a 4 x 4 matrix with floats.
	/// @param   {mat4d} mat The matrix.
	/// @result  {mat4} The casted matrix.
	/// 
	inline mat4 to_mat4(const mat4d& mat)
	{
		return mat4(
			(float)mat.data[0],  (float)mat.data[1],  (float)mat.data[2],  (float)mat.data[3],
			(float)mat.data[4],  (float)mat.data[5],  (float)mat.data[6],  (float)mat.data[7],
			(float)mat.data[8],  (float)mat.data[9],  (float)mat.data[10], (float)mat.data[11],
			(float)mat.data[12], (float)mat.data[13], (float)mat.data[14], (float)mat.data[15]);
	}


	/// 
	/// Calculate the perspective off center matrix from the given values of the frustum:
	/// @param   {float} left The left plane.
	/// @param   {float} right The right plane.
	/// @param   {float} bottom The bottom plane.
	/// @param   {float} top The top plane.
	/// @param   {float} near The near plane.
	/// @param   {float} far The far plane.
	/// @result  {mat4} The perspective projection matrix.
	/// 
	inline mat4 mat_perspective_off_center(
		const float& left,
		const float& right,
		const float& bottom,
		const float& top,
		const float& near,
		const float& far)
	{
		float a = (right + left) / (right - left);
		float b = (top + bottom) / (top - bottom);
		float c = -(far + near) / (far - near);
		float d = -2.0 * far * near / (far - near);

		return mat4(
			2.0 * near / (right - left), 0.0, 0.0, 0.0,
			0.0, 2.0 * near / (top - bottom), 0.0, 0.0,
			a, b, c, -1,
			0.0, 0.0, d, 0);
	}

	/// 
	/// Calculate the perspective off center matrix from the given values of the frustum:
	/// @param   {double} left The left plane.
	/// @param   {double} right The right plane.
	/// @param   {double} bottom The bottom plane.
	/// @param   {double} top The top plane.
	/// @param   {double} near The near plane.
	/// @param   {double} far The far plane.
	/// @result  {mat4d} The perspective projection matrix.
	/// 
	inline mat4d mat_perspective_off_center(
		const double& left,
		const double& right,
		const double& bottom,
		const double& top,
		const double& near,
		const double& far)
	{
		double a = (right + left) / (right - left);
		double b = (top + bottom) / (top - bottom);
		double c = -(far + near) / (far - near);
		double d = -2.0 * far * near / (far - near);

		return mat4d(
			2.0 * near / (right - left), 0.0, 0.0, 0.0,
			0.0, 2.0 * near / (top - bottom), 0.0, 0.0,
			a, b, c, -1,
			0.0, 0.0, d, 0);

	}

	/// 
	/// Calculate the perspective projection matrix from the given values:
	/// @param {double} fovy   The field of view in y expressed in radians.
	/// @param {double} aspect The aspect ratio of the frustum. 
	/// @param {double} zn     The near plane.
	/// @param {double} zf     The far plane.
	/// @result  {mat4d} The perspective projection matrix.
	/// 
	inline mat4d mat_perspective_fov(
		const double& fovy,
		const double& aspect,
		const double& zn,
		const double& zf)
	{
		double y_fac = std::tan(fovy / 2);
		double x_fac = y_fac * aspect;

		return mat4d(
			1.0f / x_fac, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f / x_fac, 0.0f, 0.0f,
			0.0f, 0.0f, -(zf + zn) / (zf - zn), -1.0f,
			0.0f, 0.0f, -(2.0f * zf * zn) / (zf - zn), 0.0f);
	}

	/// 
	/// Calculate the perspective projection matrix from the given values:
	/// @param {float} fovy   The field of view in y expressed in radians.
	/// @param {float} aspect The aspect ratio of the frustum. 
	/// @param {float} zn     The near plane.
	/// @param {float} zf     The far plane.
	/// @result  {mat4d} The perspective projection matrix.
	/// 
	inline mat4 mat_perspective_fov(
		const float& fovy_in_radians,
		const float& aspect,
		const float& zn,
		const float& zf)
	{
		float y_fac = tan(fovy_in_radians / 2);
		float x_fac = y_fac * aspect;

		return mat4(
			1.0f / x_fac, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f / x_fac, 0.0f, 0.0f,
			0.0f, 0.0f, -(zf + zn) / (zf - zn), -1.0f,
			0.0f, 0.0f, -(2.0f * zf * zn) / (zf - zn), 0.0f);
	}

	/// 
	/// Calculate the orthographic off center matrix from the given values of the frustum:
	/// @param   {float} left The left plane.
	/// @param   {float} right The right plane.
	/// @param   {float} bottom The bottom plane.
	/// @param   {float} top The top plane.
	/// @param   {float} near The near plane.
	/// @param   {float} far The far plane.
	/// @result  {mat4} The orthographic projection matrix.
	/// 
	inline mat4 mat_orthographic_off_center(
		const float& left,
		const float& right,
		const float& top,
		const float& bottom,
		const float& near,
		const float& far)
	{
		return mat4(
			2 / (right - left), 0, 0, 0,
			0, 2 / (top - bottom), 0, 0,
			0, 0, -2 / (far - near), 0,
			-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1);

	}

	/// 
	/// Calculate the orthographic off center matrix from the given values of the frustum:
	/// @param   {double} left The left plane.
	/// @param   {double} right The right plane.
	/// @param   {double} bottom The bottom plane.
	/// @param   {double} top The top plane.
	/// @param   {double} near The near plane.
	/// @param   {double} far The far plane.
	/// @result  {mat4d} The orthographic projection matrix.
	/// 
	inline mat4d mat_orthographic_off_center(
		const double& left,
		const double& right,
		const double& top,
		const double& bottom,
		const double& near,
		const double& far)
	{
        double a = 1.0 / (right - left);
        double b = 1.0 / (top - bottom);
        double c = 1.0 / (far - near);

		double tx = -(right + left) * a;
        double ty = -(top + bottom) * b;
        double tz = -(far + near) * c;

		a *= 2.0;
		b *= 2.0;
		c *= -2.0;

        mat4d result;

		result.data[0]  = a;
		result.data[1]  = 0.0;
		result.data[2]  = 0.0;
		result.data[3]  = 0.0;
		result.data[4]  = 0.0;
		result.data[5]  = b;
		result.data[6]  = 0.0;
		result.data[7]  = 0.0;
		result.data[8]  = 0.0;
		result.data[9]  = 0.0;
		result.data[10] = c;
		result.data[11] = 0.0;
		result.data[12] = tx;
		result.data[13] = ty;
		result.data[14] = tz;
		result.data[15] = 1.0;

		return result;
	}

	/// 
	/// Computes the product of a matrix and a {@link Cartesian3}. This is equivalent to calling {@link Matrix4.multiplyByVector}
	/// with a {@link Cartesian4} with a <code>w</code> component of 1, but returns a {@link Cartesian3} instead of a {@link Cartesian4}.
	/// @param {Matrix4} matrix The matrix.
	/// @param {Cartesian3} cartesian The point.
	/// @param {Cartesian3} result The object onto which to store the result.
	/// @returns {Cartesian3} The modified result parameter.
	/// @example
	/// var p = new Cesium.Cartesian3(1.0, 2.0, 3.0);
	/// var result = Cesium.Matrix4.multiplyByPoint(matrix, p, new Cesium.Cartesian3());
	/// 
	inline vec3d mat_multiply_by_point(
		const mat4d& matrix, 
		const vec3d& vec)
	{
		double v_x = vec.x;
		double v_y = vec.y;
		double v_z = vec.z;

		return vec3d(
			matrix.data[0] * v_x + matrix.data[4] * v_y + matrix.data[8] * v_z + matrix.data[12],
			matrix.data[1] * v_x + matrix.data[5] * v_y + matrix.data[9] * v_z + matrix.data[13],
			matrix.data[2] * v_x + matrix.data[6] * v_y + matrix.data[10] * v_z + matrix.data[14]
		);
	}

	///
	/// Computes the product of a matrix and a {@link Cartesian3}.  This is equivalent to calling {@link Matrix4.multiplyByVector}
	/// with a {@link Cartesian4} with a <code>w</code> component of zero.
	/// @param {Matrix4} matrix The matrix.
	/// @param {Cartesian3} cartesian The point.
	/// @param {Cartesian3} result The object onto which to store the result.
	/// @returns {Cartesian3} The modified result parameter.
	/// @example
	/// 
	/// var p = new Cesium.Cartesian3(1.0, 2.0, 3.0);
	/// var result = Cesium.Matrix4.multiplyByPointAsVector(matrix, p, new Cesium.Cartesian3());
	///
	inline vec3d mat_multiply_by_point_as_vector(
		const mat4d& matrix,
		const vec3d& vec)
	{
		double v_x = vec.x;
		double v_y = vec.y;
		double v_z = vec.z;

		return vec3d(
			matrix.data[0] * v_x + matrix.data[4] * v_y + matrix.data[8] *  v_z,
			matrix.data[1] * v_x + matrix.data[5] * v_y + matrix.data[9] *  v_z,
			matrix.data[2] * v_x + matrix.data[6] * v_y + matrix.data[10] * v_z
		);
	}

	///
	/// Calculate the scale matrix for the given values:
	/// @param {double} x The scale in x.
	/// @param {double} y The scale in y.
	/// @param {double} z The scale in z.
	/// @result {mat4d} The scale matrix.
	///
	inline mat4d mat_scale(
		const double& x,
		const double& y, 
		const double& z)
	{
		return mat4d(
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1);
	}

	inline vec3d mat_get_translation(const mat4d& mat)
	{
		return vec3d(
			mat.data[12],
			mat.data[13], 
			mat.data[14]);
	}

    inline mat3d mat_get_rotation(const mat4d& mat)
    {
        return mat3d(
                mat.data[0], mat.data[1], mat.data[2],
                mat.data[4], mat.data[5], mat.data[6],
                mat.data[8], mat.data[9], mat.data[10]);
    }

	///
	/// Calculate the translation matrix for the given values:
	/// @param {double} x The translation in x.
	/// @param {double} y The translation in y.
	/// @param {double} z The translation in z.
	/// @result {mat4d} The translation matrix.
	///
	inline mat4d mat_translate(
		const vec3d& translation)
	{
		return mat4d(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			translation.x, translation.y, translation.z, 1);
	}

	///
	/// Calculate the translation matrix for the given values:
	/// @param {double} x The translation in x.
	/// @param {double} y The translation in y.
	/// @param {double} z The translation in z.
	/// @result {mat4d} The translation matrix.
	///
	inline mat4d mat_translate(
		const double& x, 
		const double& y, 
		const double& z)
	{
		return mat4d(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			x, y, z, 1);
	}

	///
	/// Calculate the translation matrix for the given values:
	/// @param {float} x The translation in x.
	/// @param {float} y The translation in y.
	/// @param {float} z The translation in z.
	/// @result {mat4} The translation matrix.
	///
	inline mat4 mat_translate(
		const float& x,
		const float& y, 
		const float& z)
	{
		return mat4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			x, y, z, 1);
	}

	///
	/// Calculate the rotation matrix around the x-axis for the given value:
	/// @param angle {double} angle The rotation angle in x expressed in radians.
	/// @result {mat3d} The Rotation matrix.
	///
	inline mat3d mat_rotate_x(const double& angle)
	{
		double s_angle = std::sin(angle);
		double c_angle = std::cos(angle);

		return mat3d(
			1,  0	   , 0,
			0,  c_angle, s_angle,
			0, -s_angle, c_angle);
	}

	///
	/// Calculate the rotation matrix around the y-axis for the given value:
	/// @param angle {double} angle The rotation angle in y expressed in radians.
	/// @result {mat3d} The Rotation matrix.
	///
	inline mat3d mat_rotate_y(const double& angle)
	{
		double s_angle = std::sin(angle);
		double c_angle = std::cos(angle);

		return mat3d(
			c_angle, 0, -s_angle,
			0      , 1,  0,
			s_angle, 0,  c_angle);
	}

	///
	/// Calculate the rotation matrix around the z-axis for the given value:
	/// @param angle {double} angle The rotation angle in z expressed in radians.
	/// @result {mat3d} The Rotation matrix.
	///
	inline mat3d mat_rotate_z(const double& angle)
	{
		double s_angle = std::sin(angle);
		double c_angle = std::cos(angle);

		return mat3d(
			 c_angle, s_angle, 0,
			-s_angle, c_angle, 0,
			 0		, 0      , 1);
	}

	///
	/// Get the transposed matrix from the given matrix.
	/// @param {mat3d} mat The matrix
	/// @result {mat3d} The transposed matrix.
	///
	inline mat3d mat_transpose(const mat3d& mat)
	{
		mat3d trans;
		trans.data[0] = mat.data[0];
		trans.data[1] = mat.data[3];
		trans.data[2] = mat.data[6];
		trans.data[3] = mat.data[1];
		trans.data[4] = mat.data[4];
		trans.data[5] = mat.data[7];
		trans.data[6] = mat.data[2];
		trans.data[7] = mat.data[5];
		trans.data[8] = mat.data[8];
		return trans;
	}

	///
	/// Get the transposed matrix from the given matrix.
	/// @param {mat3} mat The matrix
	/// @result {mat3} The transposed matrix.
	///
	inline mat3 mat_transpose(const mat3& mat)
	{
		mat3 trans;
		trans.data[0] = mat.data[0];
		trans.data[1] = mat.data[3];
		trans.data[2] = mat.data[6];
		trans.data[3] = mat.data[1];
		trans.data[4] = mat.data[4];
		trans.data[5] = mat.data[7];
		trans.data[6] = mat.data[2];
		trans.data[7] = mat.data[5];
		trans.data[8] = mat.data[8];
		return trans;
	}

	///
	/// Get the inverse matrix from the given matrix. 
	/// @param {mat4} mat The matrix
	/// @result {mat4} The inversed matrix.
	///
	inline mat4 mat_inverse(const mat4& mat)
	{
		float a0 = mat.data[0] * mat.data[5] - mat.data[4] * mat.data[1];
		float a1 = mat.data[0] * mat.data[9] - mat.data[8] * mat.data[1];
		float a2 = mat.data[0] * mat.data[13] - mat.data[12] * mat.data[1];
		float a3 = mat.data[4] * mat.data[9] - mat.data[8] * mat.data[5];
		float a4 = mat.data[4] * mat.data[13] - mat.data[12] * mat.data[5];
		float a5 = mat.data[8] * mat.data[13] - mat.data[12] * mat.data[9];
		float b0 = mat.data[2] * mat.data[7] - mat.data[6] * mat.data[3];
		float b1 = mat.data[2] * mat.data[11] - mat.data[10] * mat.data[3];
		float b2 = mat.data[2] * mat.data[15] - mat.data[14] * mat.data[3];
		float b3 = mat.data[6] * mat.data[11] - mat.data[10] * mat.data[7];
		float b4 = mat.data[6] * mat.data[15] - mat.data[14] * mat.data[7];
		float b5 = mat.data[10] * mat.data[15] - mat.data[14] * mat.data[11];

		float det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;
		float inv_det = 1.0f / det;

		mat4 inv;
		inv.data[0] = +mat.data[5] * b5 - mat.data[9] * b4 + mat.data[13] * b3;
		inv.data[1] = -mat.data[1] * b5 + mat.data[9] * b2 - mat.data[13] * b1;
		inv.data[2] = +mat.data[1] * b4 - mat.data[5] * b2 + mat.data[13] * b0;
		inv.data[3] = -mat.data[1] * b3 + mat.data[5] * b1 - mat.data[9] * b0;
		inv.data[4] = -mat.data[4] * b5 + mat.data[8] * b4 - mat.data[12] * b3;
		inv.data[5] = +mat.data[0] * b5 - mat.data[8] * b2 + mat.data[12] * b1;
		inv.data[6] = -mat.data[0] * b4 + mat.data[4] * b2 - mat.data[12] * b0;
		inv.data[7] = +mat.data[0] * b3 - mat.data[4] * b1 + mat.data[8] * b0;
		inv.data[8] = +mat.data[7] * a5 - mat.data[11] * a4 + mat.data[15] * a3;
		inv.data[9] = -mat.data[3] * a5 + mat.data[11] * a2 - mat.data[15] * a1;
		inv.data[10] = +mat.data[3] * a4 - mat.data[7] * a2 + mat.data[15] * a0;
		inv.data[11] = -mat.data[3] * a3 + mat.data[7] * a1 - mat.data[11] * a0;
		inv.data[12] = -mat.data[6] * a5 + mat.data[10] * a4 - mat.data[14] * a3;
		inv.data[13] = +mat.data[2] * a5 - mat.data[10] * a2 + mat.data[14] * a1;
		inv.data[14] = -mat.data[2] * a4 + mat.data[6] * a2 - mat.data[14] * a0;
		inv.data[15] = +mat.data[2] * a3 - mat.data[6] * a1 + mat.data[10] * a0;

		inv *= inv_det;
		return inv;
	}

	///
	/// Computes the inverse of the provided matrix assuming it is
	/// an affine transformation matrix, where the upper left 3x3 elements
	/// are a rotation matrix, and the upper three elements in the fourth
	/// column are the translation.  The bottom row is assumed to be [0, 0, 0, 1].
	/// The matrix is not verified to be in the proper form.
	/// This method is faster than computing the inverse for a general 4x4
	/// matrix using {@link Matrix4.inverse}.
	///
	inline mat4d mat_inverse_transformation(const mat4d& mat)
	{
		double matrix0  = mat.data[0];
		double matrix1  = mat.data[1];
		double matrix2  = mat.data[2];
		double matrix4  = mat.data[4];
		double matrix5  = mat.data[5];
		double matrix6  = mat.data[6];
		double matrix8  = mat.data[8];
		double matrix9  = mat.data[9];
		double matrix10 = mat.data[10];

		double vX = mat.data[12];
		double vY = mat.data[13];
		double vZ = mat.data[14];

		double x = -matrix0 * vX - matrix1 * vY - matrix2 * vZ;
		double y = -matrix4 * vX - matrix5 * vY - matrix6 * vZ;
		double z = -matrix8 * vX - matrix9 * vY - matrix10 * vZ;
		
		return mat4d(
			matrix0, matrix4, matrix8,  0.0,
			matrix1, matrix5, matrix9,  0.0,
			matrix2, matrix6, matrix10, 0.0,
			x, y, z, 1.0);
	}

	///
	/// Get the inverse matrix from the given matrix. 
	/// @param {mat4d} mat The matrix
	/// @result {mat4d} The inversed matrix.
	///
	inline mat4d mat_inverse(const mat4d& mat)
	{
		double a0 = mat.data[0] * mat.data[5] - mat.data[4] * mat.data[1];
		double a1 = mat.data[0] * mat.data[9] - mat.data[8] * mat.data[1];
		double a2 = mat.data[0] * mat.data[13] - mat.data[12] * mat.data[1];
		double a3 = mat.data[4] * mat.data[9] - mat.data[8] * mat.data[5];
		double a4 = mat.data[4] * mat.data[13] - mat.data[12] * mat.data[5];
		double a5 = mat.data[8] * mat.data[13] - mat.data[12] * mat.data[9];
		double b0 = mat.data[2] * mat.data[7] - mat.data[6] * mat.data[3];
		double b1 = mat.data[2] * mat.data[11] - mat.data[10] * mat.data[3];
		double b2 = mat.data[2] * mat.data[15] - mat.data[14] * mat.data[3];
		double b3 = mat.data[6] * mat.data[11] - mat.data[10] * mat.data[7];
		double b4 = mat.data[6] * mat.data[15] - mat.data[14] * mat.data[7];
		double b5 = mat.data[10] * mat.data[15] - mat.data[14] * mat.data[11];

		double det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;
		double inv_det = 1.0f / det;

		mat4d inv;
		inv.data[0] = +mat.data[5] * b5 - mat.data[9] * b4 + mat.data[13] * b3;
		inv.data[1] = -mat.data[1] * b5 + mat.data[9] * b2 - mat.data[13] * b1;
		inv.data[2] = +mat.data[1] * b4 - mat.data[5] * b2 + mat.data[13] * b0;
		inv.data[3] = -mat.data[1] * b3 + mat.data[5] * b1 - mat.data[9] * b0;
		inv.data[4] = -mat.data[4] * b5 + mat.data[8] * b4 - mat.data[12] * b3;
		inv.data[5] = +mat.data[0] * b5 - mat.data[8] * b2 + mat.data[12] * b1;
		inv.data[6] = -mat.data[0] * b4 + mat.data[4] * b2 - mat.data[12] * b0;
		inv.data[7] = +mat.data[0] * b3 - mat.data[4] * b1 + mat.data[8] * b0;
		inv.data[8] = +mat.data[7] * a5 - mat.data[11] * a4 + mat.data[15] * a3;
		inv.data[9] = -mat.data[3] * a5 + mat.data[11] * a2 - mat.data[15] * a1;
		inv.data[10] = +mat.data[3] * a4 - mat.data[7] * a2 + mat.data[15] * a0;
		inv.data[11] = -mat.data[3] * a3 + mat.data[7] * a1 - mat.data[11] * a0;
		inv.data[12] = -mat.data[6] * a5 + mat.data[10] * a4 - mat.data[14] * a3;
		inv.data[13] = +mat.data[2] * a5 - mat.data[10] * a2 + mat.data[14] * a1;
		inv.data[14] = -mat.data[2] * a4 + mat.data[6] * a2 - mat.data[14] * a0;
		inv.data[15] = +mat.data[2] * a3 - mat.data[6] * a1 + mat.data[10] * a0;

		inv *= inv_det;
		return inv;
	}


	///
	/// Extracts the non-uniform scale assuming the matrix is an affine transformation.
	/// @param   {mat4d} matrix The matrix.
	/// @returns {vec3d} The modified result parameter
	/// 
	inline vec3d mat_get_scale(const mat4d& matrix)
	{
		return vec3d(
			vec_length(vec3d(matrix.data[0], matrix.data[4], matrix.data[8])),
			vec_length(vec3d(matrix.data[1], matrix.data[5], matrix.data[9])),
			vec_length(vec3d(matrix.data[2], matrix.data[6], matrix.data[10])));
	}


	///
	/// Extracts the 3 x 3 rotation matrix from the given 4 x 4 affine transformation matrix. 
	/// @param   {mat4d} matrix The matrix.
	/// @returns {mat3d} The modified result parameter
	///
	inline mat3d to_rotation(const mat4d& mat)
	{
		return mat3d(
			mat.data[0], mat.data[1], mat.data[2],
			mat.data[4], mat.data[5], mat.data[6],
			mat.data[8], mat.data[9], mat.data[10]);
	}


	///
	/// Computes the maximum scale assuming the matrix is an affine transformation.
	/// The maximum scale is the maximum length of the column vectors in the upper-left
	/// 3x3 matrix.
	/// @param {mat4d} matrix The matrix.
	/// @returns {double} The maximum scale.
	///
	inline double mat_get_maximum_scale(const mat4d& matrix)
	{
		return vec_max_component(mat_get_scale(matrix));
	}

	///
	/// Scale a mat3d instance using a vec3d representing the scale.
	/// @param {mat3d} matrix The matrix.
	/// @param {vec3d} scale The scale.
	/// @returns {mat3d} The calculated matrix.
	///
	inline mat3d mat_multiply_by_scale(const mat3d& matrix, const vec3d& scale)
	{
		return mat3d(
			scale.x * matrix.data[0], scale.x * matrix.data[1], scale.x * matrix.data[2],
			scale.y * matrix.data[3], scale.y * matrix.data[4], scale.y * matrix.data[5],
			scale.z * matrix.data[6], scale.z * matrix.data[7], scale.z * matrix.data[8]);
	}

	inline mat4d mat_multiply_by_scale(const mat4d& matrix, const vec3d& scale)
	{
		return mat4d(
				scale.x * matrix.data[0], scale.x * matrix.data[1], scale.x * matrix.data[2],  0.0,
				scale.y * matrix.data[4], scale.y * matrix.data[5], scale.y * matrix.data[6],  0.0,
				scale.z * matrix.data[8], scale.z * matrix.data[9], scale.z * matrix.data[10], 0.0,
				matrix.data[12], matrix.data[13], matrix.data[14], 1.0);
	}

	///
	/// Computes a mat4d instance from a mat3d representing the rotation
	/// and a vec3d representing the translation.
	/// @param {mat3d} rotation The upper left portion of the matrix representing the rotation.
	/// @param {vec3d} [translation = vec3d(0.0)] The upper right portion of the matrix representing the translation.
	/// @returns {mat4d} The calculated matrix.
	///
	inline mat4d from_rotation_translation(
		const mat3d& rotation,
		const vec3d& translation = vec3d())
	{
		return mat4d(
			rotation.data[0], rotation.data[1], rotation.data[2], 0.0,
			rotation.data[3], rotation.data[4], rotation.data[5], 0.0,
			rotation.data[6], rotation.data[7], rotation.data[8], 0.0,
			translation.x,    translation.y,    translation.z, 1.0);
	};

	///
	/// The viewport transformation maps Normalized device coordinates into window (screen) coordinates.
	/// @param {Rectangle} The viewport.
	/// @param {double} The near depth range.
	/// @param {double} The far depth range.
	/// @returns {mat4d} The viewport transformation matrix.
	///
	inline mat4d compute_viewport_transformation_matrix(
		const Rectangle& viewport,
		const double& near_depth_range,
		const double& far_depth_range)
	{
		double half_width = viewport.get_width()  * 0.5;
		double half_height = viewport.get_height() * 0.5;
		double half_depth = (far_depth_range - near_depth_range) * 0.5;

		return mat4d(
			half_width, 0.0, 0.0, 0.0,
			0.0, half_height, 0.0, 0.0,
			0.0, 0.0, half_depth, 0.0,
			(viewport.get_left() + viewport.get_right()) * 0.5, (viewport.get_top() + viewport.get_bottom()) * 0.5, near_depth_range + half_depth, 1.0);
	}

	///
	/// Computes the orthographic projection matrix using a viewport as the frustum.
	/// @param {Rectangle} The viewport used as frustum.
	/// @returns {mat4d} The orthographic projection matrix.
	///
	inline mat4d compute_viewport_orthographic_matrix(const Rectangle& viewport)
	{
		return mat_orthographic_off_center(
			viewport.get_left(),
			viewport.get_right(),
			viewport.get_top(),
			viewport.get_bottom(),
			0.0,
			1.0);
	}

	///
	/// Computes a Matrix4 instance that transforms from world space to view space.
	/// @param {Cartesian3} position The position of the camera.
	/// @param {Cartesian3} direction The forward direction.
	/// @param {Cartesian3} up The up direction.
	/// @param {Cartesian3} right The right direction.
	/// @param {Matrix4} result The object in which the result will be stored.
	/// @returns {Matrix4} The modified result parameter.
	///
	inline mat4d compute_view(
		const vec3d& eye,
		const vec3d& direction,
		const vec3d& up,
		const vec3d& right)
	{
		return mat4d(
			right.x, up.x, -direction.x, 0.0,
			right.y, up.y, -direction.y, 0.0,
			right.z, up.z, -direction.z, 0.0,
			-vec_dot(right, eye), -vec_dot(up, eye), vec_dot(direction, eye), 1.0);
	}

	inline mat4d mat_set_translation(
			mat4d& mat,
			const vec3d& values)
	{
		mat.data[12] = values.x;
		mat.data[13] = values.y;
		mat.data[14] = values.z;
        return mat;
	}

	inline mat4d mat_set_column(
			mat4d& mat,
			const unsigned int column,
            const vec4d& values)
	{
		unsigned int start_index = column * 4;
        mat.data[start_index + 0] = values.x;
        mat.data[start_index + 1] = values.y;
        mat.data[start_index + 2] = values.z;
        mat.data[start_index + 3] = values.w;
        return mat;
	}

    inline mat4 mat_set_column(
            mat4& mat,
            const unsigned int column,
            const vec4d& values)
    {
        unsigned int start_index = column * 4;
        mat.data[start_index + 0] = values.x;
        mat.data[start_index + 1] = values.y;
        mat.data[start_index + 2] = values.z;
        mat.data[start_index + 3] = values.w;
        return mat;
    }

    inline mat3 mat_set_column(
            mat3& mat,
            const unsigned int column,
            const vec3d& values)
    {
        unsigned int start_index = column * 3;
        mat.data[start_index + 0] = values.x;
        mat.data[start_index + 1] = values.y;
        mat.data[start_index + 2] = values.z;
        return mat;
    }

	inline vec3d mat_get_column(
		const mat3d& mat,
		const unsigned int column)
	{
		unsigned int start_index = column * 3;
		return vec3d(
			mat.data[start_index + 0],
			mat.data[start_index + 1],
			mat.data[start_index + 2]);
	}

	inline vec3d mat_get_row(
		const mat3d& mat,
		const unsigned int row)
	{
		return vec3d(
			mat.data[row + 0],
			mat.data[row + 3],
			mat.data[row + 6]);
	}

	inline vec4d mat_get_column(
		const mat4d& mat,
		const unsigned int column)
	{
		unsigned int start_index = column * 4;
		return vec4d(
			mat.data[start_index + 0],
			mat.data[start_index + 1],
			mat.data[start_index + 2],
			mat.data[start_index + 3]);
	}

	inline vec4d mat_get_row(
		const mat4d& mat,
		const unsigned int row)
	{
		return vec4d(
			mat.data[row + 0],
			mat.data[row + 3],
			mat.data[row + 6],
			mat.data[row + 9]);
	}

	template<typename T>
	inline std::string mat_string(const T& vec)
	{
		std::ostringstream stream;
		stream << "(";
		unsigned int size = T::matrix_size;
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
		mat3 const &t)
	{
		out << mat_string(t);
		return out;
	}

	inline std::ostream & operator<< (
		std::ostream &out,
		mat4 const &t)
	{
		out << mat_string(t);
		return out;
	}
}