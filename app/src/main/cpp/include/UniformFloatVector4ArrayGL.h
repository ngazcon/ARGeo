#pragma once

#include "UniformArray.h"
#include "Vector.h"

namespace argeo
{
	class UniformFloatVector4ArrayGL : public UniformArray < vec4 >
	{
	public:

		UniformFloatVector4ArrayGL(std::string name, int location, int resource_size, ICleanableObserver* observer);
		~UniformFloatVector4ArrayGL();

		void clean();
	};
}
