#include "CameraEyeHighModelCoordinatesUniformFactory.h"

#include "CameraEyeHighModelCoordinatesUniform.h"

namespace argeo
{
	std::string CameraEyeHighModelCoordinatesUniformFactory::get_name() const
	{
		return "og_cameraEyeHighMC";
	}

	std::unique_ptr<DrawAutomaticUniform> CameraEyeHighModelCoordinatesUniformFactory::create(IUniform* uniform)
	{
		return std::unique_ptr<DrawAutomaticUniform>(
			new CameraEyeHighModelCoordinatesUniform(uniform));
	}
}
