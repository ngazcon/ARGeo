#include "ViewportUniformFactory.h"

#include "ViewportUniform.h"

namespace argeo
{
	std::string ViewportUniformFactory::get_name() const
	{
		return "og_viewport";
	}

	std::unique_ptr<DrawAutomaticUniform> ViewportUniformFactory::create(IUniform* uniform)
	{
		return std::unique_ptr<DrawAutomaticUniform>(
			new ViewportUniform(uniform));
	}
}
