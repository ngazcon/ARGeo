#pragma once

#include "UniformArray.h"

#include <string>

namespace argeo
{
	class UniformFloatArrayGL : public UniformArray < float >
	{
	public:

		UniformFloatArrayGL(std::string name, int location, int resource_size, ICleanableObserver* observer);
		~UniformFloatArrayGL();

		void clean();
	};
}
