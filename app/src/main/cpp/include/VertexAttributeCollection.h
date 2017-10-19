#pragma once

#include "IVertexAttribute.h"

#include <string>
#include <map>

namespace argeo
{
	typedef std::map<std::string, IVertexAttribute*> VertexAttributeCollection;
}
