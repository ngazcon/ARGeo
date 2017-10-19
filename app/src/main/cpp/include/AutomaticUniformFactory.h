#pragma once

#include "DrawAutomaticUniform.h"
#include "IUniform.h"

#include <string>
#include <memory>

namespace argeo
{
	class AutomaticUniformFactory
	{
	public:

		virtual std::string get_name() const = 0;
		virtual std::unique_ptr<DrawAutomaticUniform> create(IUniform* uniform) = 0;
	};
}
