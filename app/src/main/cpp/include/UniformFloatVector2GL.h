#pragma once

#include "Uniform.h"
#include "Vector.h"

namespace argeo
{
	class UniformFloatVector2GL : public Uniform < vec2 >
	{

	public:
		UniformFloatVector2GL(std::string name, int location, ICleanableObserver* observer);

		~UniformFloatVector2GL();

		void clean();
	};
}
