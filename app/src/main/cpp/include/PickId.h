#pragma once

#include "Color.h"
#include "Context.h"

namespace argeo
{
	class IPickable;

	
	class PickId
	{
	public:
		PickId(
			unsigned int id,
			Color    color,
			IPickable* object,
			Context* context)
			: id(id)
			, color(color)
			, object(object)
			, context(context)
		{ }

		~PickId()
		{ 
			context->destroy_pick_id(this);
		}

		unsigned int id;
		Color        color;
		IPickable*   object;
		Context*     context;
	};
}
