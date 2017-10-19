#include "ModelViewRelativeToEyeMatrixUniformFactory.h"

#include "ModelViewRelativeToEyeMatrixUniform.h"

namespace argeo
{
	std::string ModelViewRelativeToEyeMatrixUniformFactory::get_name() const
	{
		return "og_modelViewRelativeToEyeMatrix";
	}

	std::unique_ptr<DrawAutomaticUniform> ModelViewRelativeToEyeMatrixUniformFactory::create(IUniform* uniform)
	{
		return std::unique_ptr<DrawAutomaticUniform>(
			new ModelViewRelativeToEyeMatrixUniform(uniform));
	}
}
