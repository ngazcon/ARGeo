#pragma once

#include "UniformArray.h"
#include "Matrix.h"

namespace argeo
{
	class UniformFloatMatrix3ArrayGL : public UniformArray < mat3 >
	{
	public:
		UniformFloatMatrix3ArrayGL(std::string name, int location, int resource_size, ICleanableObserver* observer);
		~UniformFloatMatrix3ArrayGL();

		void clean();
	};
}
