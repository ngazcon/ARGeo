#pragma once

#include "Uniform.h"

namespace argeo
{
	class UniformFloatGL : public Uniform < float >
	{
	public:

		UniformFloatGL(std::string name, int location, ICleanableObserver* observer);

		~UniformFloatGL();

		void clean();
	};
}
