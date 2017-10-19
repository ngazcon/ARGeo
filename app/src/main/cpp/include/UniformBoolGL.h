#pragma once
#include "Uniform.h"

namespace argeo
{
	class UniformBoolGL : public Uniform<bool>
	{
	public:

		UniformBoolGL(std::string name, int location, ICleanableObserver* observer);
		~UniformBoolGL();

		void clean();
	};
}