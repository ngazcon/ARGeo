#include "LightPropertiesUniformFactory.h"

#include "LightPropertiesUniform.h"

namespace argeo
{
	std::string LightPropertiesUniformFactory::get_name() const
	{
		return "og_diffuseSpecularAmbientShininess";
	}

	std::unique_ptr<DrawAutomaticUniform> LightPropertiesUniformFactory::create(IUniform* uniform)
	{
		return std::unique_ptr<DrawAutomaticUniform>(
			new LightPropertiesUniform(uniform));
	}
}
