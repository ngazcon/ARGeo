#include "ModelZToClipCoordinatesUniformFactory.h"

#include "ModelZToClipCoordinatesUniform.h"

namespace argeo
{
	std::string ModelZToClipCoordinatesUniformFactory::get_name() const
	{
		return "og_modelZToClipCoordinates";
	}

	std::unique_ptr<DrawAutomaticUniform> ModelZToClipCoordinatesUniformFactory::create(IUniform* uniform)
	{
		return std::unique_ptr<DrawAutomaticUniform>(
			new ModelZToClipCoordinatesUniform(uniform));
	}
}
