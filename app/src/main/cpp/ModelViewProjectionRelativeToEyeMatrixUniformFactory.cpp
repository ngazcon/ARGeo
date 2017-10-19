#include "ModelViewProjectionRelativeToEyeMatrixUniformFactory.h"

#include "ModelViewProjectionRelativeToEyeMatrixUniform.h"

namespace argeo
{
	std::string ModelViewProjectionRelativeToEyeMatrixUniformFactory::get_name() const
	{
		return "og_modelViewProjectionRelativeToEyeMatrix";
	}

	std::unique_ptr<DrawAutomaticUniform> ModelViewProjectionRelativeToEyeMatrixUniformFactory::create(IUniform* uniform)
	{
		return std::unique_ptr<DrawAutomaticUniform>(
			new ModelViewProjectionRelativeToEyeMatrixUniform(uniform));
	}
}
