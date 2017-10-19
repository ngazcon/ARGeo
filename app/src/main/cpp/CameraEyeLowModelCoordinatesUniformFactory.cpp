#include "CameraEyeLowModelCoordinatesUniformFactory.h"

#include "CameraEyeLowModelCoordinatesUniform.h"

namespace argeo
{
	std::string CameraEyeLowModelCoordinatesUniformFactory::get_name() const
	{
		return "og_cameraEyeLowMC";
	}

	std::unique_ptr<DrawAutomaticUniform> CameraEyeLowModelCoordinatesUniformFactory::create(IUniform* uniform)
	{
		return std::unique_ptr<DrawAutomaticUniform>(
			new CameraEyeLowModelCoordinatesUniform(uniform));
	}
}
