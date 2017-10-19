#include "SceneNode.h"

#include "Scene.h"

namespace argeo
{
	SceneNode::SceneNode(
		Scene* scene,
		std::string name)
		: m_needs_update(true)
		, m_scene(scene)
		, m_name(name)
		, m_parent(nullptr)
		, m_local_matrix(mat4d::identity())
	{ }


	SceneNode::~SceneNode()
	{ }

	void SceneNode::set_local_euler_angles(
		double pitch,
		double yaw,
		double roll)
	{
		set_local_rotation(quaternion::from_yaw_pitch_roll(yaw, pitch, roll));
	}

	void SceneNode::set_local_translation(
		vec3d translation,
		bool use_local_rotation)
	{
		set_local_translation(translation.x, translation.y, translation.z, use_local_rotation);
	}

	void SceneNode::local_translate(
		vec3d translation,
		bool use_local_rotation)
	{
		local_translate(translation.x, translation.y, translation.z, use_local_rotation);
	}

	SceneNode* SceneNode::create_child(std::string name)
	{
		// = m_scene->create_scene_node(name);
//		add_child(child);
		return nullptr;
	}

	std::string SceneNode::get_name() const
	{
		return m_name;
	}

	void SceneNode::update(FrameState* frame_state)
	{
		update_local_matrix();

		//Traversal the scene graph and generate the render queue.
		for (IPrimitive* renderable : m_renderables)
		{
			renderable->update(frame_state);
		}

		for (SceneNode* child : m_children)
		{
			child->update(frame_state);
		}
	}

	ConstVectorIterator<std::unordered_set<SceneNode*>> SceneNode::get_child_iterator()
	{
		return ConstVectorIterator<std::unordered_set<SceneNode*>>(m_children);
	}

	ConstVectorIterator<std::unordered_set<IPrimitive*>> SceneNode::get_renderable_iterator()
	{
		return ConstVectorIterator<std::unordered_set<IPrimitive*>>(m_renderables);
	}

	/*
	void SceneNode::render(Context* context, UniformState& uniform_state)
	{
		update_uniform_state(uniform_state);

		for (IPrimitive* renderable : m_renderables)
		{
			renderable->render(context, uniform_state);
		}

		for (SceneNode* child : m_children)
		{
			child->render(context, uniform_state);
		}
	}

	void SceneNode::pre_render(Context* context, UniformState& uniform_state)
	{
		update_uniform_state(uniform_state);

		for (IPrimitive* renderable : m_renderables)
		{
			renderable->pre_render(context, uniform_state);
		}

		for (SceneNode* child : m_children)
		{
			child->pre_render(context, uniform_state);
		}
	}
	*/

	bool SceneNode::attach_renderable(IPrimitive* renderable)
	{
		return m_renderables.insert(renderable).second;
	}

	bool SceneNode::detach_renderable(IPrimitive* renderable)
	{
		return m_renderables.erase(renderable);
	}

	void SceneNode::set_parent(SceneNode* parent)
	{
		m_parent = parent;
	}

	bool SceneNode::add_child(SceneNode* child)
	{
		child->set_parent(this);
		return m_children.insert(child).second;
	}

	bool SceneNode::remove_child(SceneNode* child)
	{
		return m_children.erase(child);
	}

	void SceneNode::set_local_translation(
		double x,
		double y,
		double z,
		bool use_local_rotation)
	{
		vec3d translation = vec3d(x, y, z);
		if (m_local_translation != translation)
		{
			m_needs_update = true;
			m_local_translation = translation;
		}
	}

	void SceneNode::local_translate(
		double x,
		double y,
		double z,
		bool use_local_rotation)
	{
		vec3d translation    = vec3d(x, y, z);
		m_needs_update = true;
		m_local_translation  += translation;
	}

	void SceneNode::set_local_rotation(quaternion rotation)
	{
		if (m_local_rotation != rotation)
		{
			m_needs_update = true;
			m_local_rotation = rotation;
		}
	}

	void SceneNode::local_rotate(quaternion rotation)
	{
		m_needs_update = true;
		m_local_rotation *= rotation;
	}

	void SceneNode::set_local_scale(
		double x,
		double y,
		double z)
	{
		vec3d scale(x, y, z);
		if (m_local_scale != scale)
		{
			m_needs_update = true;
			m_local_scale  = scale;
		}
	}

	quaternion SceneNode::get_local_rotation() const
	{
		return m_local_rotation;
	}

	vec3d SceneNode::get_local_translation() const
	{
		return m_local_translation;
	}

	vec3d SceneNode::get_local_scale() const
	{
		return m_local_scale;
	}

	

	mat4d SceneNode::get_local_matrix()
	{
		if (m_needs_update)
		{
			m_needs_update = false;
			update_local_matrix();		
		}
		return m_local_matrix;	
	}

	mat4d SceneNode::get_global_matrix()
	{
		mat4d global_mat = mat4d::identity();

		if (m_parent != nullptr)
		{
			global_mat *= m_parent->get_global_matrix();
		}

		global_mat *= get_local_matrix();

		return global_mat;
	}

	void SceneNode::update_local_matrix()
	{
		m_local_matrix =
			mat_scale(
				m_local_scale.x,
				m_local_scale.y,
				m_local_scale.z)
			*
			mat_translate(
				m_local_translation.x,
				m_local_translation.y,
				m_local_translation.z) 
			*
			mat4d(quat_to_matrix(m_local_rotation));
	}

}