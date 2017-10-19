#pragma once

#include "Uniform.h"
#include "Matrix.h"

namespace argeo
{
	class UniformFloatMatrix42GL : public Uniform < mat42 >
	{
	public:

		UniformFloatMatrix42GL(std::string name, int location, ICleanableObserver* observer);
		~UniformFloatMatrix42GL();

		void clean();
	};
}
