#pragma once

#include "UniformArray.h"
#include "Vector.h"

namespace argeo
{
	class UniformFloatVector3ArrayGL : public UniformArray < vec3 >
	{
	public:

		UniformFloatVector3ArrayGL(std::string name, int location, int resource_size, ICleanableObserver* observer);
		~UniformFloatVector3ArrayGL();

		void clean();
	};
}
