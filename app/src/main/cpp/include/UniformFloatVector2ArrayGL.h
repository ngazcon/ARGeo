#pragma once

#include "UniformArray.h"
#include "Vector.h"

namespace argeo
{
	class UniformFloatVector2ArrayGL : public UniformArray < vec2 >
	{
	public:

		UniformFloatVector2ArrayGL(std::string name, int location, int resource_size, ICleanableObserver* observer);
		~UniformFloatVector2ArrayGL();

		void clean();
	};
}
