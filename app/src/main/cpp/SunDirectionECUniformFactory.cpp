#include "SunDirectionECUniformFactory.h"

#include "SunDirectionECUniform.h"

namespace argeo
{
	std::string SunDirectionECUniformFactory::get_name() const
	{
		return "og_sunDirectionEC";
	}

	std::unique_ptr<DrawAutomaticUniform> SunDirectionECUniformFactory::create(IUniform* uniform)
	{
		return std::unique_ptr<DrawAutomaticUniform>(
			new SunDirectionECUniform(uniform));
	}
}
