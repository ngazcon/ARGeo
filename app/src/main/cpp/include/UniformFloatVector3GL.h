#pragma once

#include "Uniform.h"
#include "Vector.h"

namespace argeo
{
	class UniformFloatVector3GL : public Uniform < vec3 >
	{
	public:

		UniformFloatVector3GL(std::string name, int location, ICleanableObserver* observer);
		~UniformFloatVector3GL();

		void clean();
	};
}
