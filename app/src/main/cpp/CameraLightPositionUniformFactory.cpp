#include "CameraLightPositionUniformFactory.h"

#include "CameraLightPositionUniform.h"

namespace argeo
{
	std::string CameraLightPositionUniformFactory::get_name() const
	{
		return "og_cameraLightPosition";
	}

	std::unique_ptr<DrawAutomaticUniform> CameraLightPositionUniformFactory::create(IUniform* uniform)
	{
		return std::unique_ptr<DrawAutomaticUniform>(
			new CameraLightPositionUniform(uniform));
	}
}
