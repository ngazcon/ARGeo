#pragma once

#include "INode.h"
#include "Matrix.h"

#include "IteratorWrapper.h"
#include "IPrimitive.h"
#include "FrameState.h"

#include <unordered_set>

namespace argeo
{
	class Scene;

	class SceneNode : public INode<SceneNode>
	{
		friend class Scene;

	public:
		~SceneNode();

		ConstVectorIterator<std::unordered_set<IPrimitive*>> get_renderable_iterator();
		ConstVectorIterator<std::unordered_set<SceneNode*>> get_child_iterator();

		bool attach_renderable(IPrimitive* renderable);
		bool detach_renderable(IPrimitive* renderable);

		SceneNode* create_child(std::string name);

		void set_parent(SceneNode* parent);
		bool add_child(SceneNode* child);
		bool remove_child(SceneNode* child);

		void set_local_translation(
			vec3d translation,
			bool use_local_rotation = false);

		void local_translate(
			vec3d translation,
			bool use_local_rotation = false);

		void set_local_euler_angles(
			double pitch,
			double yaw,
			double roll);

		void set_local_translation(
			double x,
			double y,
			double z,
			bool use_local_rotation = false);

		void local_translate(
			double x,
			double y,
			double z,
			bool use_local_rotation = false);

		void set_local_rotation(quaternion rotation);
		void local_rotate(quaternion rotation);
		
		void set_local_scale(
			double x,
			double y,
			double z);

		quaternion get_local_rotation() const;
		vec3d get_local_translation() const;
		vec3d get_local_scale() const;

		mat4d get_local_matrix();
		mat4d get_global_matrix();

		std::string get_name() const;

	protected:
		SceneNode(
			Scene* scene,
			std::string name);

		void update(FrameState* frame_state);
		void update_local_matrix();

	private:
		std::string m_name;

		bool m_needs_update;
		mat4d m_local_matrix;

		std::unordered_set<SceneNode*> m_children;
		SceneNode* m_parent;
		Scene* m_scene;

		std::unordered_set<IPrimitive*> m_renderables;

		quaternion m_local_rotation;
		vec3d m_local_translation;
		vec3d m_local_scale;
	};
	
}
