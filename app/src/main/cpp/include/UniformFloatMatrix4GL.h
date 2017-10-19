#pragma once

#include "Uniform.h"
#include "Matrix.h"

namespace argeo
{
	class UniformFloatMatrix4GL : public Uniform < mat4 >
	{
	public:

		UniformFloatMatrix4GL(std::string name, int location, ICleanableObserver* observer);
		~UniformFloatMatrix4GL();

		void clean();
	};
}
