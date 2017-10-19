#include "SunDirectionWCUniformFactory.h"

#include "SunDirectionWCUniform.h"

namespace argeo
{
	std::string SunDirectionWCUniformFactory::get_name() const
	{
		return "og_sunDirectionWC";
	}

	std::unique_ptr<DrawAutomaticUniform> SunDirectionWCUniformFactory::create(IUniform* uniform)
	{
		return std::unique_ptr<DrawAutomaticUniform>(
			new SunDirectionWCUniform(uniform));
	}
}
