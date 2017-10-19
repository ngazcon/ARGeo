#pragma once

#include "UniformArray.h"
#include "Matrix.h"

namespace argeo
{
	class UniformFloatMatrix4ArrayGL : public UniformArray < mat4 >
	{
	public:

		UniformFloatMatrix4ArrayGL(std::string name, int location, int resource_size, ICleanableObserver* observer);
		~UniformFloatMatrix4ArrayGL();

		void clean();
	};
}
