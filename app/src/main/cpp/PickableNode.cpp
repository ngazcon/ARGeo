#include "PickableNode.h"

#include "Model.h"

namespace argeo
{
	PickableNode::PickableNode(
		Model* model,
		ModelMesh* mesh,
		ModelNode* node)
		: mesh(mesh)
		, node(node)
		, model(model)
	{ }


	PickableNode::~PickableNode()
	{ }

	void PickableNode::on_picked()
	{
		model->on_picked();
		IPickable::on_picked();
	}
}