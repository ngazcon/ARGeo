#pragma once

#include "IPickable.h"

namespace argeo
{
	class  Model;
	class  ModelNode;
	struct ModelMesh;

	class PickableNode : public IPickable
	{
	public:
		PickableNode(
			Model* model,
			ModelMesh* mesh,
			ModelNode* node);

		~PickableNode();

		Model* model;
		ModelMesh* mesh;
		ModelNode* node;

		void on_picked();
	};
}