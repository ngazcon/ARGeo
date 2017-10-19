#include "CurrentFrustumUniformFactory.h"

#include "CurrentFrustumUniform.h"

namespace argeo
{
	std::string CurrentFrustumUniformFactory::get_name() const
	{
		return "og_currentFrustum";
	}

	std::unique_ptr<DrawAutomaticUniform> CurrentFrustumUniformFactory::create(IUniform* uniform)
	{
		return std::unique_ptr<DrawAutomaticUniform>(
			new CurrentFrustumUniform(uniform));
	}
}
