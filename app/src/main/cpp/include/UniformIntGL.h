#pragma once

#include "Uniform.h"

namespace argeo
{
	class UniformIntGL : public Uniform < int >
	{
	public:

		UniformIntGL(
			std::string name,
			int location,
			UniformType type, 
			ICleanableObserver* observer);

		~UniformIntGL();

		void clean();
	};
}
