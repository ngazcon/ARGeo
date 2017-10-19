#pragma once

#include "Uniform.h"
#include "Matrix.h"

namespace argeo
{
	class UniformFloatMatrix3GL : public Uniform < mat3 >
	{
	public:

		UniformFloatMatrix3GL(std::string name, int location, ICleanableObserver* observer);
		~UniformFloatMatrix3GL();

		void clean();
	};
}
