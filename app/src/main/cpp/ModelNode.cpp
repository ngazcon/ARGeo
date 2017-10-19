#include "ModelNode.h"

#include "Model.h"

#include <stdexcept>

namespace argeo
{

	ModelNode::ModelNode(
		Model* model,
		const aiNode* ai_node,
		std::string name)
		: m_model(model)
		, m_ai_node(ai_node)
		, m_name(name)
		, m_parent(nullptr)
		, m_model_matrix_changed(true)
		, m_parent_model_matrix_changed(true)
	    , m_show(true)
		, m_global_matrix(mat4d::identity())
		, m_local_matrix(mat4d::identity())
		, m_local_rotation(quaternion::identity())
		, m_local_scale(vec3d(1.0, 1.0, 1.0))
		, m_local_translation(vec3d(0.0, 0.0, 0.0))
	{ }

	ModelNode::~ModelNode()
	{ }

	const aiNode* ModelNode::get_ai_node() const
	{
		return m_ai_node;
	}

	std::string ModelNode::get_name() const
	{
		return m_name;
	}

	void ModelNode::add_mesh(ModelMesh* mesh)
	{
		if (mesh == nullptr)
		{
			throw std::invalid_argument(
				"Mesh cannot be null.");
		}

		m_meshes.push_back(mesh);
	}

	ConstVectorIterator<std::unordered_set<ModelNode*>> ModelNode::get_children_iterator()
	{
		return ConstVectorIterator<std::unordered_set<ModelNode*>>(m_children);
	}

	ConstVectorIterator<std::vector<ModelMesh*>> ModelNode::get_meshes_iterator()
	{
		return ConstVectorIterator<std::vector<ModelMesh*>>(m_meshes);
	}

	bool ModelNode::get_show() const
	{
		return m_show;
	}

	void ModelNode::set_show(bool value)
	{
		m_show = value;
	}

	ModelNode* ModelNode::get_parent()
	{
		return m_parent;
	}

	void ModelNode::set_parent(ModelNode* parent)
	{
		m_parent = parent;
	}

	bool ModelNode::add_child(ModelNode* child)
	{
		child->set_parent(this);
		return m_children.insert(child).second;
	}

	bool ModelNode::remove_child(ModelNode* child)
	{
		return m_children.erase(child);
	}

	void ModelNode::set_local_euler_angles(
		double pitch,
		double yaw,
		double roll)
	{
		set_local_rotation(quaternion::from_yaw_pitch_roll(yaw, pitch, roll));
	}

	void ModelNode::set_local_translation(
		vec3d translation,
		bool use_local_rotation)
	{
		set_local_translation(translation.x, translation.y, translation.z, use_local_rotation);
	}

	void ModelNode::local_translate(
		vec3d translation,
		bool use_local_rotation)
	{
		local_translate(translation.x, translation.y, translation.z, use_local_rotation);
	}

	void ModelNode::set_local_translation(
		double x,
		double y,
		double z,
		bool use_local_rotation)
	{
		vec3d translation = vec3d(x, y, z);
		if (m_local_translation != translation)
		{
			model_matrix_changed();
			m_local_translation = translation;
		}
	}

	void ModelNode::local_translate(
		double x,
		double y,
		double z,
		bool use_local_rotation)
	{
		vec3d translation = vec3d(x, y, z);
		model_matrix_changed();
		m_local_translation += translation;
	}

	void ModelNode::set_local_rotation(quaternion rotation)
	{
		if (m_local_rotation != rotation)
		{
			model_matrix_changed();
			m_local_rotation	   = rotation;
		}
	}

	void ModelNode::local_rotate(quaternion rotation)
	{
		model_matrix_changed();
		m_local_rotation *= rotation;
	}

	void ModelNode::set_local_scale(
		double x,
		double y,
		double z)
	{
		vec3d scale(x, y, z);
		if (m_local_scale != scale)
		{
			model_matrix_changed();
			m_local_scale = scale;
		}	
	}

	quaternion ModelNode::get_local_rotation() const
	{
		return m_local_rotation;
	}

	vec3d ModelNode::get_local_translation() const
	{
		return m_local_translation;
	}

	vec3d ModelNode::get_local_scale() const
	{
		return m_local_scale;
	}

	mat4d ModelNode::get_local_matrix()
	{
		update_local_matrix();
		return m_local_matrix;
	}

	mat4d ModelNode::get_global_matrix()
	{
		update_global_matrix();
		return m_global_matrix;
	}

	void ModelNode::update(FrameState* frame_state)
	{ }

	void ModelNode::parent_model_matrix_changed()
	{
		m_parent_model_matrix_changed = true;
	}

	void ModelNode::model_matrix_changed()
	{
		m_model_matrix_changed = true;

		if (m_parent != nullptr)
		{
			m_parent->model_matrix_changed();
		}
		else
		{
			m_model->model_matrix_changed();
		}

		for (ModelNode* child : m_children)
		{
			child->parent_model_matrix_changed();
		}
	}

	void ModelNode::update_global_matrix()
	{
		if (m_parent_model_matrix_changed)
		{
			if (m_parent != nullptr)
			{
				m_global_matrix = 
					m_parent->get_global_matrix() *
					get_local_matrix();
			}
			else
			{
				m_global_matrix = get_local_matrix();
			}
			
			m_parent_model_matrix_changed = false;
		}
	}

	void ModelNode::update_local_matrix()
	{
		if (m_model_matrix_changed)
		{
			m_local_matrix =
				mat_translate(
					m_local_translation.x,
					m_local_translation.y,
					m_local_translation.z)
				*
				mat4d(quat_to_matrix(m_local_rotation))
				*
				mat_scale(
					m_local_scale.x,
					m_local_scale.y,
					m_local_scale.z);

			m_model_matrix_changed = false;
		}
	}
}