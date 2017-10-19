#pragma once

#include "IUniform.h"

#include <map>
#include <string>
#include <memory>

namespace argeo
{
	typedef std::map<std::string, IUniform*> UniformCollection;
}
