#pragma once

#include "UniformArray.h"

namespace argeo
{
	class UniformBoolArrayGL : public UniformArray < int >
	{
	public:

		UniformBoolArrayGL(std::string name, int location, int resource_size, ICleanableObserver* observer);
		~UniformBoolArrayGL();

		void clean();
	};
}
