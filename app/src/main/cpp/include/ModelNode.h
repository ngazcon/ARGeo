#pragma once

#include "INode.h"
#include "FrameState.h"
#include "ModelMesh.h"
#include "IteratorWrapper.h"

#include <assimp/scene.h>
#include <unordered_set>

namespace argeo
{
	class Model;

	class ModelNode : public INode<ModelNode>
	{
	public:
		ModelNode(
			Model* model,
			const aiNode* ai_node,
			std::string name);

		~ModelNode();

		void add_mesh(ModelMesh* mesh);

		ConstVectorIterator<std::unordered_set<ModelNode*>> get_children_iterator();
		ConstVectorIterator<std::vector<ModelMesh*>> get_meshes_iterator();

		ModelNode* get_parent();
		void set_parent(ModelNode* parent);
		bool add_child(ModelNode* child);
		bool remove_child(ModelNode* child);

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

		bool get_show() const;
		void set_show(bool value);

		const aiNode* get_ai_node() const;
		std::string get_name() const;

	protected:

		void update(FrameState* frame_state);
		void update_local_matrix();
		void update_global_matrix();

		void model_matrix_changed();
		void parent_model_matrix_changed();

	private:
		std::string m_name;
		bool m_show;

		bool  m_parent_model_matrix_changed;
		bool  m_model_matrix_changed;
		mat4d m_local_matrix;
		mat4d m_global_matrix;

		std::vector<ModelMesh*> m_meshes;
		std::unordered_set<ModelNode*> m_children;
		
		ModelNode* m_parent;
		Model* m_model;
		const aiNode* m_ai_node;

		quaternion m_local_rotation;
		vec3d m_local_translation;
		vec3d m_local_scale;
	};
}