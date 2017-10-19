#pragma once

#include "UniformBlockMember.h"

#include <string>
#include <map>
#include <memory>

namespace argeo
{
	typedef std::map< std::string, std::unique_ptr<UniformBlockMember>> UniformBlockMemberCollection;
}
