#include "MorphTimeUniform.h"
#include "MorphTimeUniformFactory.h"

namespace argeo
{
	std::string MorphTimeUniformFactory::get_name() const
	{
		return "og_morphTime";
	}

	std::unique_ptr<DrawAutomaticUniform> MorphTimeUniformFactory::create(IUniform* uniform)
	{
		return std::unique_ptr<DrawAutomaticUniform>(
			new MorphTimeUniform(uniform));
	}
}
