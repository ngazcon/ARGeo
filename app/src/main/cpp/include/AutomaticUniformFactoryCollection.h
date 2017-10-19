#pragma once

#include "AutomaticUniformFactory.h"

#include <map>
#include <memory>

namespace argeo
{
	class draw_automatic_uniform_comparer {
	public:
		bool operator() (const std::string & first, const std::string & second) const {
			return first.compare(second) < 0;
		}
	};


	typedef std::map<std::string, std::unique_ptr<AutomaticUniformFactory>, draw_automatic_uniform_comparer> AutomaticUniformFactoryCollection;
}

