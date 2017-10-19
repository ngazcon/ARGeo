#pragma once

#include <string>

#include "ICleanableObserver.h"
#include "Uniform.h"
#include "Vector.h"

namespace argeo
{
	class UniformFloatVector4GL : public Uniform<vec4>
	{
	public:

		UniformFloatVector4GL(std::string name, int location, ICleanableObserver* observer);
		~UniformFloatVector4GL();

		void clean();
	};
}
