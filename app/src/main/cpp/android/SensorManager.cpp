#include "include/JNIHelper.h"
#include "include/SensorManager.h"

namespace argeo
{
	SensorManager::SensorManager()
	{
		m_sensor_manager = ASensorManager_getInstance();

		ASensorList sensor_list;
		int available_sensor_count = ASensorManager_getSensorList(m_sensor_manager, &sensor_list);
		if (available_sensor_count > 0)
		{
			for (int i = 0; i < available_sensor_count; i++)
			{
				Sensor::SensorType type = static_cast<Sensor::SensorType>(
					ASensor_getType(sensor_list[i]));

				m_sensors[type] = new Sensor(type);
			}
		}
	}
	
	SensorManager::~SensorManager()
	{
		for (auto entry : m_sensors)
		{
			delete entry.second;
		}
	}

	bool SensorManager::add_sensor_listener(
		SensorListener* listener,
		Sensor* sensor,
		int sampling_period_us)
	{
		return sensor->register_listener(
			listener,
			sampling_period_us,
			JNIHelper::get_instance().get_main_looper());
	}

	bool SensorManager::remove_sensor_listener(SensorListener* listener)
	{
		bool success = false;

		for (auto entry : m_sensors)
		{
			if (entry.second->unregister_listener(listener))
			{
				success = true;
			}
		}

		return success;
	}

	Sensor* SensorManager::get_default_sensor(Sensor::SensorType type)
	{
		return m_sensors[type];
	}

	SensorManager& SensorManager::get_instance()
	{
		static SensorManager instance;
		return instance;
	}

	bool SensorManager::remap_coordinate_system(
		float* in_rotation_matrix,
		int X,
		int Y,
		float* out_rotation_matrix,
		int matrix_size)
	{
		if (in_rotation_matrix == out_rotation_matrix)
		{
			float* temp = new float[matrix_size];

			// we don't expect to have a lot of contention 
			if (remap_coordinate_system_impl(in_rotation_matrix, X, Y, temp, matrix_size))
			{
				for (int i = 0; i < matrix_size; i++)
				{
					out_rotation_matrix[i] = temp[i];
				}

				delete[] temp;
				return true;
			}
		}

		return remap_coordinate_system_impl(
			in_rotation_matrix,
			X,
			Y,
			out_rotation_matrix, matrix_size);
	}

	bool SensorManager::remap_coordinate_system_impl(
		float* in_rotation_matrix,
		int X,
		int Y,
		float* out_rotation_matrix,
		int matrix_size)
	{
		/*
		 * X and Y define a rotation matrix 'r':
		  *
		  *  (X==1)?((X&0x80)?-1:1):0    (X==2)?((X&0x80)?-1:1):0    (X==3)?((X&0x80)?-1:1):0
		  *  (Y==1)?((Y&0x80)?-1:1):0    (Y==2)?((Y&0x80)?-1:1):0    (Y==3)?((X&0x80)?-1:1):0
		  *                              r[0] ^ r[1]
		  *
		  * where the 3rd line is the vector product of the first 2 lines
		  *
		  */

		if ((X & 0x7C) != 0 || (Y & 0x7C) != 0)
			return false;   // invalid parameter 
		if (((X & 0x3) == 0) || ((Y & 0x3) == 0))
			return false;   // no axis specified 
		if ((X & 0x3) == (Y & 0x3))
			return false;   // same axis specified 

		// Z is "the other" axis, its sign is either +/- sign(X)*sign(Y) 
		// this can be calculated by exclusive-or'ing X and Y; except for 
		// the sign inversion (+/-) which is calculated below. 
		int Z = X ^ Y;

		// extract the axis (remove the sign), offset in the range 0 to 2. 
		int x = (X & 0x3) - 1;
		int y = (Y & 0x3) - 1;
		int z = (Z & 0x3) - 1;

		// compute the sign of Z (whether it needs to be inverted) 
		int axis_y = (z + 1) % 3;
		int axis_z = (z + 2) % 3;
		if (((x^axis_y) | (y^axis_z)) != 0)
			Z ^= 0x80;

		bool sx = (X >= 0x80);
		bool sy = (Y >= 0x80);
		bool sz = (Z >= 0x80);

		// Perform R * r, in avoiding actual muls and adds. 
		int row_length = ((matrix_size == 16) ? 4 : 3);

		for (int j = 0; j < 3; j++)
		{
			int offset = j * row_length;
			for (int i = 0; i < 3; i++)
			{
				if (x == i)   out_rotation_matrix[offset + i] = sx ? -in_rotation_matrix[offset + 0] : in_rotation_matrix[offset + 0];
				if (y == i)   out_rotation_matrix[offset + i] = sy ? -in_rotation_matrix[offset + 1] : in_rotation_matrix[offset + 1];
				if (z == i)   out_rotation_matrix[offset + i] = sz ? -in_rotation_matrix[offset + 2] : in_rotation_matrix[offset + 2];
			}
		}

		if (matrix_size == 16)
		{
			out_rotation_matrix[3] = out_rotation_matrix[7] = out_rotation_matrix[11] = out_rotation_matrix[12] = out_rotation_matrix[13] = out_rotation_matrix[14] = 0;
			out_rotation_matrix[15] = 1;
		}

		return true;
	}

	 float* SensorManager::get_orientation(
		 float* rotation_matrix,
		 float* orientation,
		 int matrix_size)
	 {
		 /*
		  * 4x4 (length=16) case:
		  *   /  R[ 0]   R[ 1]   R[ 2]   0  \
		  *   |  R[ 4]   R[ 5]   R[ 6]   0  |
		  *   |  R[ 8]   R[ 9]   R[10]   0  |
		  *   \      0       0       0   1  /
		  *
		  * 3x3 (length=9) case:
		  *   /  R[ 0]   R[ 1]   R[ 2]  \
		  *   |  R[ 3]   R[ 4]   R[ 5]  |
		  *   \  R[ 6]   R[ 7]   R[ 8]  /
		  *
		  */

		 if (matrix_size == 9)
		 {
			 orientation[0] = (float)atan2(rotation_matrix[1], rotation_matrix[4]);
			 orientation[1] = (float)asin(-rotation_matrix[7]);
			 orientation[2] = (float)atan2(-rotation_matrix[6], rotation_matrix[8]);
		 }
		 else
		 {
			 orientation[0] = (float)atan2(rotation_matrix[1], rotation_matrix[5]);
			 orientation[1] = (float)asin(-rotation_matrix[9]);
			 orientation[2] = (float)atan2(-rotation_matrix[8], rotation_matrix[10]);
		 }

		 return orientation;
	 }

	 void SensorManager::get_rotation_matrix_from_vector(
		 float* rotation_matrix,
		 float* rotation_vector,
		 int quaternion_size,
		 int matrix_size)
	 {
		 float q0;
		 float q1 = rotation_vector[0];
		 float q2 = rotation_vector[1];
		 float q3 = rotation_vector[2];

		 if (quaternion_size == 4)
		 {
			 q0 = rotation_vector[3];
		 }
		 else
		 {
			 q0 = 1 - q1*q1 - q2*q2 - q3*q3;
			 q0 = (q0 > 0) ? (float)sqrt(q0) : 0;
		 }

		 float sq_q1 = 2 * q1 * q1;
		 float sq_q2 = 2 * q2 * q2;
		 float sq_q3 = 2 * q3 * q3;
		 float q1_q2 = 2 * q1 * q2;
		 float q3_q0 = 2 * q3 * q0;
		 float q1_q3 = 2 * q1 * q3;
		 float q2_q0 = 2 * q2 * q0;
		 float q2_q3 = 2 * q2 * q3;
		 float q1_q0 = 2 * q1 * q0;

		 if (matrix_size == 9)
		 {
			 rotation_matrix[0] = 1 - sq_q2 - sq_q3;
			 rotation_matrix[1] = q1_q2 - q3_q0;
			 rotation_matrix[2] = q1_q3 + q2_q0;

			 rotation_matrix[3] = q1_q2 + q3_q0;
			 rotation_matrix[4] = 1 - sq_q1 - sq_q3;
			 rotation_matrix[5] = q2_q3 - q1_q0;

			 rotation_matrix[6] = q1_q3 - q2_q0;
			 rotation_matrix[7] = q2_q3 + q1_q0;
			 rotation_matrix[8] = 1 - sq_q1 - sq_q2;
		 }
		 else if (matrix_size == 16)
		 {
			 rotation_matrix[0] = 1 - sq_q2 - sq_q3;
			 rotation_matrix[1] = q1_q2 - q3_q0;
			 rotation_matrix[2] = q1_q3 + q2_q0;
			 rotation_matrix[3] = 0.0f;

			 rotation_matrix[4] = q1_q2 + q3_q0;
			 rotation_matrix[5] = 1 - sq_q1 - sq_q3;
			 rotation_matrix[6] = q2_q3 - q1_q0;
			 rotation_matrix[7] = 0.0f;

			 rotation_matrix[8] = q1_q3 - q2_q0;
			 rotation_matrix[9] = q2_q3 + q1_q0;
			 rotation_matrix[10] = 1 - sq_q1 - sq_q2;
			 rotation_matrix[11] = 0.0f;

			 rotation_matrix[12] = rotation_matrix[13] = rotation_matrix[14] = 0.0f;
			 rotation_matrix[15] = 1.0f;
		 }
	 }


}
