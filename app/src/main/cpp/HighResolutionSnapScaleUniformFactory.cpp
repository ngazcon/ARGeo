#include "HighResolutionSnapScaleUniformFactory.h"

#include "HighResolutionSnapScaleUniform.h"

namespace argeo
{
	std::string HighResolutionSnapScaleUniformFactory::get_name() const
	{
		return "og_highResolutionSnapScale";
	}

	std::unique_ptr<DrawAutomaticUniform> HighResolutionSnapScaleUniformFactory::create(IUniform* uniform)
	{
		return std::unique_ptr<DrawAutomaticUniform>(
			new HighResolutionSnapScaleUniform(uniform));
	}
}
