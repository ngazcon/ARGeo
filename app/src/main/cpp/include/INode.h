#pragma once

#include "Vector.h"
#include "Quaternion.h"
#include "Matrix.h"
#include <type_traits>

#include <string>

namespace argeo
{
	template<typename T>
	class INode
	{
	protected:

		INode()
		{
			static_assert(std::is_base_of<INode, T>::value, "Type parameter of this class must derive from INode");
		}

	public:

		virtual void set_parent(T* parent) = 0;
		virtual bool add_child(T* child) = 0;
		virtual bool remove_child(T* child) = 0;

		virtual void set_local_translation(
			vec3d translation,
			bool use_local_rotation = false) = 0;

		virtual void local_translate(
			vec3d translation,
			bool use_local_rotation = false) = 0;
		
		virtual void set_local_euler_angles(
			double pitch,
			double yaw,
			double roll) = 0;

		virtual void set_local_translation(
			double x,
			double y,
			double z,
			bool use_local_rotation = false) = 0;

		virtual void local_translate(
			double x,
			double y,
			double z,
			bool use_local_rotation = false) = 0;


		virtual void set_local_rotation(quaternion rotation) = 0;
		virtual void local_rotate(quaternion rotation) = 0;


		virtual void set_local_scale(
			double x,
			double y,
			double z) = 0;


		virtual quaternion get_local_rotation() const = 0;
		virtual vec3d get_local_translation() const = 0;
		virtual vec3d get_local_scale() const = 0;

		virtual mat4d get_local_matrix() = 0;
		virtual mat4d get_global_matrix() = 0;

		virtual std::string get_name() const = 0;

	};	
}