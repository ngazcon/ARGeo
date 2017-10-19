#pragma once
#include "UniformArray.h"

namespace argeo
{
	class UniformIntArrayGL : public UniformArray < int >
	{
	public:
		UniformIntArrayGL(std::string name, int location, int resource_size, UniformType type, ICleanableObserver* observer);
		~UniformIntArrayGL();

		void clean();
	};
}
