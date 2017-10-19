#pragma once

#include "UniformBlock.h"

#include <map>
#include <string>
#include <memory>

namespace argeo
{
	typedef std::map<std::string, std::unique_ptr<UniformBlock>> UniformBlockCollection;
}
