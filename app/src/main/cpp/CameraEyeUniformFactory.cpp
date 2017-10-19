#include "CameraEyeUniformFactory.h"

#include "CameraEyeUniform.h"

namespace argeo
{
	std::string CameraEyeUniformFactory::get_name() const
	{
		return "og_cameraEye";
	}

	std::unique_ptr<DrawAutomaticUniform> CameraEyeUniformFactory::create(IUniform* uniform)
	{
		return std::unique_ptr<DrawAutomaticUniform>(
			new CameraEyeUniform(uniform));
	}
}
